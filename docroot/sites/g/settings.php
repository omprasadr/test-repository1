<?php

/**
 * PHP settings:
 *
 * To see what PHP settings are possible, including whether they can be set at
 * runtime (by using ini_set()), read the PHP documentation:
 * http://www.php.net/manual/ini.list.php
 * See drupal_environment_initialize() in includes/bootstrap.inc for required
 * runtime settings and the .htaccess file for non-runtime settings. Settings
 * defined there should not be duplicated here so as to avoid conflict issues.
 */

/**
 * Some distributions of Linux (most notably Debian) ship their PHP
 * installations with garbage collection (gc) disabled. Since Drupal depends on
 * PHP's garbage collection for clearing sessions, ensure that garbage
 * collection occurs by using the most common settings.
 */
ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 100);

/**
 * Set session lifetime (in seconds), i.e. the time from the user's last visit
 * to the active session may be deleted by the session garbage collector. When
 * a session is deleted, authenticated users are logged out, and the contents
 * of the user's $_SESSION variable is discarded.
 */
ini_set('session.gc_maxlifetime', 200000);

/**
 * Set session cookie lifetime (in seconds), i.e. the time from the session is
 * created to the cookie expires, i.e. when the browser is expected to discard
 * the cookie. The value 0 means "until the browser is closed".
 */
ini_set('session.cookie_lifetime', 2000000);

/**
 * If you encounter a situation where users post a large amount of text, and
 * the result is stripped out upon viewing but can still be edited, Drupal's
 * output filter may not have sufficient memory to process it.  If you
 * experience this issue, you may wish to uncomment the following two lines
 * and increase the limits of these variables.  For more information, see
 * http://php.net/manual/pcre.configuration.php.
 */
# ini_set('pcre.backtrack_limit', 200000);
# ini_set('pcre.recursion_limit', 200000);

/**
 * Drupal automatically generates a unique session cookie name for each site
 * based on its full domain name. If you have multiple domains pointing at the
 * same Drupal site, you can either redirect them all to a single domain (see
 * comment in .htaccess), or uncomment the line below and specify their shared
 * base domain. Doing so assures that users remain logged in as they cross
 * between your various domains. Make sure to always start the $cookie_domain
 * with a leading dot, as per RFC 2109.
 */
# $cookie_domain = '.example.com';

/**
 * Acquia Cloud Site Factory specific settings.
 */
if (file_exists('/var/www/site-php')) {
  // The DB role will be the same as the gardens site directory name
  $role = basename(conf_path());
  // This global is set in sites.php. It's used to reference the
  // live environment DB setting even when running on the update env.
  $site_settings = !empty($GLOBALS['gardens_site_settings']) ? $GLOBALS['gardens_site_settings'] : array('site' => '', 'env' => '');
  $site = $site_settings['site'];
  $env = $site_settings['env'];

  $settings_inc = "/var/www/site-php/{$site}.{$env}/D7-{$env}-{$role}-settings.inc";
  if (file_exists($settings_inc)) {
    include($settings_inc);
  }
  elseif (!isset($_SERVER['SERVER_SOFTWARE']) && (php_sapi_name() == 'cli' || (is_numeric($_SERVER['argc']) && $_SERVER['argc'] > 0))) {
    throw new Exception('No database connection file was found for DB {$role}.');
  }
  else {
    syslog(LOG_ERR, 'GardensError: AN-22471 - No database connection file was found for DB {$role}.');
    header($_SERVER['SERVER_PROTOCOL'] .' 503 Service unavailable');
    print 'The website encountered an unexpected error. Please try again later.';
    exit;
  }
  if (!class_exists('DrupalFakeCache')) {
    $conf['cache_backends'][] = 'includes/cache-install.inc';
  }
  // Rely on the external Varnish cache for page caching.
  $conf['cache_class_cache_page'] = 'DrupalFakeCache';
  $conf['cache'] = 1;
  $conf['page_cache_maximum_age'] = 300;
  // We can't use an external cache if we are trying to invoke these hooks.
  $conf['page_cache_invoke_hooks'] = FALSE;

  if (!empty($site_settings['flags']['memcache']) && !empty($site_settings['memcache_inc'])) {
    // @todo setup memcache.
    $conf['cache_backends'][] = $site_settings['memcache_inc'];
  }
  if (!empty($site_settings['flags']['slackerland'])) {
    // @todo render site inoperative.
  }
  if (!empty($site_settings['conf'])) {
    foreach ((array) $site_settings['conf'] as $key => $value) {
      $conf[$key] = $value;
    }
  }
}
