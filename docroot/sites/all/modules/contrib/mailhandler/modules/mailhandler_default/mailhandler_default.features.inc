<?php
/**
 * @file
 * mailhandler_default.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mailhandler_default_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "mailhandler" && $api == "mailhandler_mailbox") {
    return array("version" => "2");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function mailhandler_default_node_info() {
  $items = array(
    'mailhandler_source' => array(
      'name' => t('Mailhandler source'),
      'base' => 'node_content',
      'description' => t('A source node for a Mailhandler importer, allowing for automatic retrieval on cron.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
