<?php
// $Id$

/**
 * @file
 * Definition of ProductSiteTemplate.
 */

class ProductSiteTemplate extends DrupalSiteTemplate {
  /**
   * Site template callback for basic product site features.
   */
  public function product_template_base_feature_install(&$data, $features) {
    // Save the variables we will be modifying below.
    $data['variable_states'] = site_template_get_variable_states(array('theme_default'));

    // Enable the site slogan.
    variable_set('site_slogan', 'This is your site’s slogan. Edit, move or remove it as needed.');

    // Change the default theme.
    gardens_misc_replace_default_theme('impact');

    // Move the user login blocks to sidebar B and only display it on the front
    // page (note that forum pages have their own separate login link).
    $block = array(
      'module' => 'gardens_client',
      'delta' => 'login',
      'region' => 'sidebar_b',
      'pages' => array('<front>'),
    );
    site_template_enable_block($block);
    $block = array(
      'module' => 'user',
      'delta' => 'login',
      'region' => 'sidebar_b',
    );
    site_template_enable_block($block);

    // Move the search block to the preheader.
    $block = array(
      'module' => 'search',
      'delta' => 'form',
      'region' => 'preheader_first',
      'weight' => 20,
    );
    site_template_enable_block($block);

    // Move the site_slogan block to the precontent_bottom.
    $block = array(
      'module' => 'block_everything',
      'delta' => 'site_slogan',
      'region' => 'precontent_bottom',
      'weight' => 20,
      'visibility' => 1,
      'pages' => array('<front>'),
    );
    site_template_enable_block($block);
  }

  /**
   * Rollback function for basic product site features.
   */
  public function product_template_base_feature_rollback($data) {
    site_template_restore_variables_from_state($data['variable_states']);
  }

  /**
   * Site template preparation callback for the "Product Overview" page.
   */
  public function product_template_product_overview_page_install_prepare(&$data, $features) {

    // Create the page with an associated menu item.
    $body = <<<EOD
<p>This is a Basic page that can be used to highlight your product(s).</p>
<p>To change what content is displayed on your front page, click Configure > Site Information.<br />
To add another Basic page, click Add content > Basic page.</p>
EOD;

    $data['page'] = array(
      'type' => 'page',
      'title' => 'Product description',
      'body' => $body,
      'menu_link' => array(
        'menu_name' => 'main-menu',
        'link_title' => 'Overview',
        'weight' => -26,
      ),
    );
  }

  /**
   * Site template callback for the "Product Overview" page.
   */
  public function product_template_product_overview_page_install(&$data, $features) {
    // Create the node and set it to the front page.
    $node = site_template_add_basic_node($data['page']);
    variable_set('site_frontpage', "node/{$node->nid}");
  }

  /**
   * Rollback function the "Product Overview" page.
   */
  public function product_template_product_overview_page_rollback($data) {
    // TODO.
  }

  /**
   * Site template preparation callback for the "Testimonials" feature.
   */
  public function product_template_testimonials_feature_install_prepare(&$data, $features) {
    // Prepare the testimonial content type.
    $data['node_type_data'] = array(
      'type' => 'testimonial',
      'name' => 'Testimonial',
      'description' => "Use a <em>testimonial</em> to display a customer's quote about your site.",
      'body_label' => 'Quote',
    );

    // Prepare author and affiliation fields.
    $data['author_field'] = array(
      'field_name' => 'field_testimonial_author',
      'type' => 'text',
    );
    $data['author_field_instance'] = array(
      'field_name' => 'field_testimonial_author',
      'entity_type' => 'node',
      'label' => 'Author',
      'bundle' => 'testimonial',
      'description' => 'The author of the quote.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'weight' => 10,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'weight' => 10,
        ),
      ),
    );
    $data['affiliation_field'] = array(
      'field_name' => 'field_testimonial_affiliation',
      'type' => 'text',
    );
    $data['affiliation_field_instance'] = array(
      'field_name' => 'field_testimonial_affiliation',
      'label' => 'Affiliation',
      'description' => "The author's affiliation (e.g., company or organization).",
    ) + $data['author_field_instance'];

    // Create some sample content.
    $post = array(
      'type' => 'testimonial',
      'title' => 'Sample testimonial 1',
      'body' => "<p>This is a sample testimonial, a quote praising your product or service by a customer or partner. A Testimonial is a custom content type containing the quote, attributed person, and their company.</p><p>To add a testimonial, click Add content > Testimonial.</p><p><a href='http://www.drupalgardens.com/learnmore/testimonials' target='_blank'>Learn more</a></p>",
      'fields' => array(
        'field_testimonial_author' => array(
          array('value' => 'Jim Jones'),
        ),
        'field_testimonial_affiliation' => array(
          array('value' => 'Company ABC'),
        ),
      ),
    );
    $data['testimonials'][] = $post;
    $post['title'] = 'Sample testimonial 2';
    $post['body'] = 'This is another sample testimonial. This product is great.';
    $post['fields']['field_testimonial_author'][0]['value'] = 'Anonymous';
    $post['fields']['field_testimonial_affiliation'][0]['value'] = 'Company 123';
    $data['testimonials'][] = $post;
    $post['title'] = 'Sample testimonial 3';
    $post['body'] = 'This is another sample testimonial. Your service is so helpful.';
    $post['fields']['field_testimonial_author'][0]['value'] = 'Sarah Smith';
    $post['fields']['field_testimonial_affiliation'][0]['value'] = 'Company XYZ';
    $data['testimonials'][] = $post;

    // Prepare a listing page for it, with a block and menu item.
    $data['simpleview_to_create'] = array(
      'path' => 'testimonials',
      'title' => 'Testimonials',
      'filter' => 'node:testimonial',
      'style' => 'full',
      'sort' => 'newest',
      'rss' => 1,
      'page_items' => 10,
      'block' => 1,
      'block_items' => 1,
      'block_style' => 'full',
      'menu_link' => array(
        'link_title' => 'Testimonials',
        'menu_name' => 'main-menu',
        'weight' => -22,
      ),
    );

    // Prepare the block for the simpleview listing.
    $data['block_data'] = array(
      'module' => 'sqbs',
      'region' => site_template_prefooter_regions(),
      'title' => 'Overheard',
    );
  }

  /**
   * Site template callback for the "Testimonials" feature.
   */
  public function product_template_testimonials_feature_install(&$data, $features) {
    module_enable(array('simpleviews', 'sqbs'), FALSE);
    // Create the content type.
    site_template_add_node_type($data['node_type_data']);

    // Do not show the "Share this" links on it. This is just a small variable,
    // so it's worth doing even if the "Share this" feature wasn't installed
    // (since someone might install it later and we want to give them reasonable
    // default behavior).
    variable_set('addthis_nodetype_' . $data['node_type_data']['type'], 0);

    field_create_field($data['author_field']);
    field_create_instance($data['author_field_instance']);
    field_create_field($data['affiliation_field']);
    field_create_instance($data['affiliation_field_instance']);
    foreach ($data['testimonials'] as $testimonial) {
      site_template_add_basic_node($testimonial);
    }
    $simpleview = site_template_create_simpleview($data['simpleview_to_create']);
    $data['block_data']['delta'] = $simpleview['simpleview_id'];
    site_template_enable_block($data['block_data']);
  }

  /**
   * Rollback function the "Testimonials" feature.
   */
  public function product_template_testimonials_feature_rollback($data) {
    // TODO.
  }

  /**
   * Site template preparation callback for the "Customers" feature.
   */
  public function product_template_customers_feature_install_prepare(&$data, $features) {
    // Prepare the content type.
    $data['node_type_data'] = array(
      'type' => 'customer',
      'name' => 'Customer',
      'description' => "Use <em>customer</em> content to display a profile of one of your site's customers.",
      'title_label' => 'Customer name',
    );

    // Prepare to attach an image field to it.
    $data['image_instance'] = site_template_standard_image_instance('customer');
    $data['image_instance']['label'] = 'Customer icon';
    $data['image_instance']['description'] = 'Upload an icon for this customer.';

    // Create sample customer images.
    $data['customer_images'][] = array(
      'post' => array(
        'type' => 'customer',
        'title' => 'Example customer 1',
        'fields' => array(
          'field_image' => array(
            array(
              'title' => 'Example customer 1',
              'alt' => 'Example customer 1',
            ),
          ),
        ),
      ),
      'folder' => 'customers',
      'filename' => 'customer-sample-DrupalGardens.png',
      'public_subdir' => 'field/image',
    );
    $data['customer_images'][] = array(
      'post' => array(
        'type' => 'customer',
        'title' => 'Example customer 2',
        'fields' => array(
          'field_image' => array(
            array(
              'title' => 'Example customer 2',
              'alt' => 'Example customer 2',
            ),
          ),
        ),
      ),
      'folder' => 'customers',
      'filename' => 'customer-sample-acquia.png',
      'public_subdir' => 'field/image',
    );

    // Prepare a simpleview listing for them.
    $data['simpleview_to_create'] = array(
      'path' => 'customers',
      'title' => 'Customers',
      'filter' => 'node:customer',
      'style' => 'full',
      'sort' => 'newest',
      'rss' => 1,
      'page_items' => 10,
      'block' => 1,
      'block_items' => 15,
      'block_style' => 'teasers',
    );

    // Prepare the block for the simpleview listing.
    $data['block_data'] = array(
      'module' => 'sqbs',
      'region' => 'sidebar_b',
      'pages' => array('<front>'),
    );
  }

  /**
   * Site template callback for the "Customers" feature.
   */
  public function product_template_customers_feature_install(&$data, $features) {
    module_enable(array('simpleviews', 'sqbs'), FALSE);
    // Create the content type.
    site_template_add_node_type($data['node_type_data']);

    // Do not show the "Share this" links on it. This is just a small variable,
    // so it's worth doing even if the "Share this" feature wasn't installed
    // (since someone might install it later and we want to give them reasonable
    // default behavior).
    variable_set('addthis_nodetype_' . $data['node_type_data']['type'], 0);

    // Create the field instance.
    field_create_instance($data['image_instance']);

    // Copy each default image and create a node for it.
    foreach ($data['customer_images'] as $image) {
      $file = site_template_copy_asset($image['folder'], $image['filename'], $image['public_subdir']);
      $image['post']['fields']['field_image'][0]['fid'] = $file->fid;
      site_template_add_basic_node($image['post']);
    }

    // Create the simpleview.
    $simpleview = site_template_create_simpleview($data['simpleview_to_create']);

    // Create the block.
    $data['block_data']['delta'] = $simpleview['simpleview_id'];
    site_template_enable_block($data['block_data']);
  }

  /**
   * Rollback function the "Customers" feature.
   */
  public function product_template_customers_feature_rollback($data) {
    // TODO.
  }

  /**
   * Site template preparation callback for the "Product Feature Blocks" feature.
   */
  public function product_template_product_feature_blocks_feature_install_prepare(&$data, $features) {
    // Prepare the blocks.
    $block = array(
      'block' => array(
        'module' => 'block',
        'region' => 'precontent_first',
        'pages' => array('<front>'),
      ),
      'custom_block' => array(
        'info' => 'Product feature 1',
        'body' => '<p>You can use this space to call out the benefits of the product or service you are promoting. You can place text, images or embedded video in the block. Each product feature is defined by a custom block. You can change which region the block is displayed in and control when it appears on your site.</p><p><a href="http://www.drupalgardens.com/learnmore/productfeatureblocks" target="_blank">Learn more</a></p>',
        'format' => variable_get('gardens_safe_html_format'),
      ),
      'image' => array(
        'folder' => 'product_features',
        'filename' => 'lightbulb.png',
        // TODO: This is somewhat of a hackish way to get things in the format
        // Media module expects.
        'media_data' => array(
          'type' => 'media',
          'view_mode' => 'media_large',
          'attributes' => array(
            'typeof' => 'foaf:Image',
            'title' => 'Product feature 1',
            'class' => 'media-image',
            'alt' => 'Product feature 1',
          ),
        ),
      ),
      // Also a hack, but to theme the block correctly the title needs to be
      // part of the body.
      'inline_title' => 'Product feature 1',
    );
    $data['block_data'][] = $block;

    $block['block']['region'] = 'precontent_second';
    $block['inline_title'] = 'Product feature 2';
    $block['custom_block']['info'] = 'Product feature 2';
    $block['custom_block']['body'] = 'This is another block to describe a second feature or benefit.  It can be changed just like Product feature 1.  Note that instead of changing this content by editing this custom block, you could place a different block here, such as a Gallery.';
    $block['image']['filename'] = 'compass.png';
    $block['image']['media_data']['title'] = 'Product feature 2';
    $block['image']['media_data']['alt'] = 'Product feature 2';
    $data['block_data'][] = $block;

    $block['block']['region'] = 'precontent_third';
    $block['inline_title'] = 'Product feature 3';
    $block['custom_block']['info'] = 'Product feature 3';
    $block['custom_block']['body'] = 'This is another block to describe a third feature or benefit.  It can be changed just like Product feature 1 and 2.';
    $block['image']['filename'] = 'star.png';
    $block['image']['media_data']['title'] = 'Product feature 3';
    $block['image']['media_data']['alt'] = 'Product feature 3';
    $data['block_data'][] = $block;
  }

  /**
   * Site template callback for the "Product Feature Blocks" feature.
   */
  public function product_template_product_feature_blocks_feature_install(&$data, $features) {
    // Save the blocks.
    foreach ($data['block_data'] as $block_data) {
      // Copy the image and store the media data we need from it.
      $file = site_template_copy_asset($block_data['image']['folder'], $block_data['image']['filename']);
      $block_data['image']['media_data']['fid'] = $file->fid;
      // Prepend the image and title to the begin of the custom block body.
      $block_data['custom_block']['body'] = '[[' . drupal_json_encode($block_data['image']['media_data']) . ']]' . '<h2>' . $block_data['inline_title'] . '</h2>' . $block_data['custom_block']['body'];
      // Save the custom block and get its delta.
     $delta = db_insert('block_custom')
       ->fields(array(
        'body' => $block_data['custom_block']['body'],
        'info' => $block_data['custom_block']['info'],
        'format' => $block_data['custom_block']['format'],
      ))
      ->execute();
      // Enable the block.
      $block_data['block']['delta'] = $delta;
      site_template_enable_block($block_data['block']);
    }
  }

  /**
   * Rollback function the "Product Feature Blocks" feature.
   */
  public function product_template_product_feature_blocks_feature_rollback($data) {
    // TODO.
  }

  /**
   * Override the preparation step for adding the "Home" link.
   */
  public function site_template_add_home_link_install_prepare(&$data, $features) {
    // Only add a Home link if we aren't already adding the "Overview" link.
    if (!in_array('product_template_product_overview_page', $features)) {
      parent::site_template_add_home_link_install_prepare($data, $features);
    }
  }

  /**
   * Override the preparation step for the "Comment" feature.
   */
  public function site_template_comment_feature_install_prepare(&$data, $features) {
    parent::site_template_comment_feature_install_prepare($data, $features);
    // Restrict the recent comments block to forum and blog pages only.
    $data['block_data']['pages'] = array(
      'blog',
      'blog/*',
      'forum',
      'forum/*',
    );
    // Put the block in sidebar B.
    $data['block_data']['region'] = 'sidebar_b';
  }

  /**
   * Override the preparation step for the "About us" feature.
   */
  public function site_template_about_page_install_prepare(&$data, $features) {
    parent::site_template_about_page_install_prepare($data, $features);
    // Put the "About us" page in the user menu.
    $data['page']['menu_link']['menu_name'] = 'user-menu';
  }

  /**
   * Override the preparation step for the "Blog" feature.
   */
  public function site_template_blog_feature_install_prepare(&$data, $features) {
    parent::site_template_blog_feature_install_prepare($data, $features);
    // Put the Blog page in the user menu.
    $data['menu_link']['menu_name'] = 'user-menu';
    // Do not display the block.
    unset($data['block_data']);
  }

  /**
   * Override the preparation step for the "Contact" feature.
   */
  public function site_template_contact_feature_install_prepare(&$data, $features) {
    parent::site_template_contact_feature_install_prepare($data, $features);
    // Put the Contact page in the user menu.
    $data['menu_link']['menu_name'] = 'user-menu';
  }

  /**
   * Override the preparation step for the forum feature.
   */
  public function site_template_forum_feature_install_prepare(&$data, $features) {
    // Change the existing forum to be product-related, and add a new forum for
    // suggestions.
    parent::site_template_forum_feature_install_prepare($data, $features);
    $data['forums']['discussion_forum']['forum']->name = 'General discussion';
    $data['forums']['discussion_forum']['forum']->description = 'Post topics about our product here.';
    $data['forums']['discussion_forum']['forum_topic']['title'] = "Sample forum topic for the \"{$data['forums']['discussion_forum']['forum']->name}\" forum";
    $forum = (object) array(
      'name' => 'Suggestions',
      'description' => 'Suggest ideas to enhance our product here.',
      'weight' => -1,
    );
    $data['forums']['suggestions_forum']['forum'] = $forum;
    $data['forums']['suggestions_forum']['forum_topic'] = $data['forums']['discussion_forum']['forum_topic'];
    $data['forums']['suggestions_forum']['forum_topic']['title'] = "Sample forum topic for the \"{$data['forums']['suggestions_forum']['forum']->name}\" forum";
    $data['forums']['suggestions_forum']['comments'][] = site_template_prepare_basic_comment('Sample comment', "This is a sample comment to demonstrate how comments will appear on your site's forums.");
  }

  /**
   * Override the preparation step for the "Mailing list" feature.
   */
  public function site_template_mailing_list_feature_install_prepare(&$data, $features) {
    parent::site_template_mailing_list_feature_install_prepare($data, $features);
    // Install the mailing list block in the footer.
    $data['block_data']['region'] = site_template_prefooter_regions();
  }

  /**
   * Override the preparation step for the "Twitter" feature.
   */
  public function site_template_twitter_feature_install_prepare(&$data, $features) {
    parent::site_template_twitter_feature_install_prepare($data, $features);
    // Limit the Twitter block to the testimonials page if that feature was
    // selected.
    if (in_array('product_template_testimonials_feature', $features)) {
      $data['block_data']['pages'] = array('testimonials');
    }
  }
}

