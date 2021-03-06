@api @javascript @insulated @campaign
Feature: Personalize elements variations can be edited for an existing campaign.
  In order to manage element variations in context
  As a site marketer
  I want the ability to add, edit, and delete existing personalize element variations in context.

  Scenario: Add personalize elements to a campaign
    # I have a campaign.
    # I login with the marketer role.
    # I am on an article page.
    Given "acquia_lift_target" agents:
      | machine_name                    | label                           |
      | testing-campaign-add-variations | Testing campaign add variations |
    And I am logged in as a user with the "access administration pages,access toolbar,administer visitor actions,manage personalized content" permission
    And I am viewing an "Article" content:
      | title | Test Article Title - Original |
    When I click "Acquia Lift" in the "menu" region
    And I wait for AJAX to finish
    Then I should visibly see the link "Personalizations" in the "lift_tray" region

    # I open the variation set's menu.
    When I hover over "Personalizations" in the "lift_tray" region
    And I click "Testing campaign add variations" in the "lift_tray" region
    Then I should visibly see the link "What" in the "lift_tray" region
    And I should see "0" for the "variation set" count

    # I bring up the "Add variation set" interface.
    When I hover over "What" in the "lift_tray" region
    Then I should visibly see the link "Add variation set" in the "lift_tray" region
    When I click "Add variation set"
    Then I should see the modal with title "Add a variation set"
    And I should visibly see the link "Webpage elements" in the "modal_content" region
    And I should visibly see the link "Drupal blocks" in the "modal_content" region
    When I click "Webpage elements" in the "modal_content" region
    Then I should not see the modal

    # I add a new variation set.
    When I click "#page-title" element in the "page_content" region
    Then I should see the text "<H1>" in the "dialog_variation_type" region
    When I click "Edit text" in the "dialog_variation_type" region
    Then I should not see the variation type dialog
    And I should see the text "Edit text: <H1>" in the "dialog_variation_type_form" region
    And the "title" field should contain ""
    And the "option_label" field should contain ""
    And the "personalize_elements_content" field should contain text that has "Test Article Title - Original"

    # Edit selector to invalid and verify response
    When I click "#acquia-lift-selector-edit" element in the "page" region
    Then the "Selector" field should contain "#page-title"
    When I fill in "#my-invalid-id-for-behat-tests" for "Selector"
    And I press "Save" in the "dialog_variation_type_form" region
    Then I should see the text "The selector does not match any DOM elements" in the "dialog_variation_type_form" region

    # Save the new variation set.
    When I fill in "Test Article Title - Updated 1" for "personalize_elements_content"
    And I fill in "#page-title" for "Selector"
    And I fill in "My variation set #1" for "title"
    And I fill in "My variation #1" for "option_label"
    And I press "Save" in the "dialog_variation_type_form" region

    # I verify my variation set is created.
    Then I should see the message "The variation set has been created." in the messagebox
    Then I should not see the variation type form dialog
    And I should see the text "Test Article Title" in the "page_content" region
    And I should see "1" for the "variation set" count

    # I bring up the "Add variation" interface.
    When I hover over "What" in the "lift_tray" region
    Then I should see the text "My variation set #1" in the "lift_tray" region
    And I should visibly see the link "Control variation" in the "lift_tray" region
    And I should visibly see the link "My variation #1" in the "lift_tray" region
    And I should visibly see the link "Add variation" in the "lift_tray" region
    And I should visibly see the link "Add variation set" in the "lift_tray" region

    # I add a new variation to to the existing variation set.
    When I click "Add variation" in the "lift_tray" region
    Then I should see "#page-title" element in the "page_content" region is "highlighted" for editing
    And I should see the text "Edit text: <H1>" in the "dialog_variation_type_form" region
    And the "personalize_elements_content" field should contain "Test Article Title - Updated 1"
    And I should not see the link "Edit selector" in the "dialog_variation_type_form" region
    And the "title" field should contain "My variation set #1"
    And the "option_label" field should contain ""
    When I fill in "Test Article Title - Updated 2" for "personalize_elements_content"
    And I fill in "Variation set renamed" for "title"
    And I fill in "Variation renamed" for "option_label"
    And I press "Add another" in the "dialog_variation_type_form" region

    # I should immediately see the dialog to add a new variation.
    Then I should see the text "Edit text: <H1>" in the "dialog_variation_type_form" region
    And the "personalize_elements_content" field should contain text that has "Test Article Title - Updated 1"
    And I should not see the link "Edit selector" in the "dialog_variation_type_form" region

    # The unibar should have updated in the background.
    When I hover over "What" in the "lift_tray" region
    Then I should see the text "Variation set renamed" in the "lift_tray" region
    And I should visibly see the link "Control variation" in the "lift_tray" region
    And I should visibly see the link "My variation #1" in the "lift_tray" region
    And I should visibly see the link "Variation renamed" in the "lift_tray" region
    And I should visibly see the link "Add variation" in the "lift_tray" region
    And I should visibly see the link "Add variation set" in the "lift_tray" region

    # I add the third variation.
    And the "title" field should contain "Variation set renamed"
    And the "option_label" field should contain ""
    When I fill in "Test Article Title - Updated 3" for "personalize_elements_content"
    And I fill in "My variation #3" for "option_label"
    And I press "Add another" in the "dialog_variation_type_form" region

    # I cancel out of adding a fourth.
    Then I should see the text "Edit text: <H1>" in the "dialog_variation_type_form" region
    And the "personalize_elements_content" field should contain text that has "Test Article Title - Updated 1"
    And the "title" field should contain "Variation set renamed"
    And the "option_label" field should contain ""
    And I should not see the link "Edit selector" in the "dialog_variation_type_form" region
    When I press "Cancel" in the "dialog_variation_type_form" region

    # I verify my new variation is added.
    Then I should not see the variation type form dialog
    And I should see the text "Test Article Title - Updated 1" in the "page_content" region
    And I should see "1" for the "variation set" count
    When I hover over "What" in the "lift_tray" region
    Then I should see the text "Variation set renamed" in the "lift_tray" region
    And I should visibly see the link "My variation #1" in the "lift_tray" region
    And I should visibly see the link "Variation renamed" in the "lift_tray" region
    And I should visibly see the link "My variation #3" in the "lift_tray" region
    And I should not visibly see the link "Variation #4" in the "lift_tray" region

  Scenario: Edit existing personalize elements for an acquia_lift campaign.
    # I have a campaign and a variation set.
    # I login with the marketer role.
    # I am on an article page.
    Given "acquia_lift_target" agents:
      | machine_name                     | label                            |
      | testing-campaign-edit-variations | Testing campaign edit variations |
    And personalized elements:
      | label              | agent                            | selector    | type     | content                |
      | Page title updated | testing-campaign-edit-variations | #page-title | editText | The Rainbow Connection |
    And I am logged in as a user with the "access administration pages,access toolbar,administer visitor actions,manage personalized content" permission
    And I am viewing an "Article" content:
      | title | Test Article Title - Original |
    When I click "Acquia Lift" in the "menu" region
    Then I should visibly see the link "Personalizations" in the "lift_tray" region

    # I open the variation set's menu.
    When I hover over "Personalizations" in the "lift_tray" region
    And I click "Testing campaign edit variations" in the "lift_tray" region
    Then I should visibly see the link "What" in the "lift_tray" region
    And I should see "1" for the "variation set" count

    # I bring up the "Edit" variation set interface.
    When I hover over "What" in the "lift_tray" region
    Then I should see the text "Page title updated" in the "lift_tray" region
    And I should visibly see the link "Option A" in the "lift_tray" region
    And "Page title updated" set "Control variation" variation should not have the "Edit" link
    And "Page title updated" set "Control variation" variation should not have the "Delete" link
    And "Page title updated" set "Option A" variation should have the "Edit" link
    And "Page title updated" set "Option A" variation should have the "Delete" link

    # I edit the variation set.
    When I click "Edit" link for the "Page title updated" set "Option A" variation
    Then I should see the text "Edit text: <H1>" in the "dialog_variation_type_form" region
    And the "personalize_elements_content" field should contain "The Rainbow Connection"
    And the "title" field should contain "Page title updated"
    And the "option_label" field should contain "Option A"
    When I fill in "Variation 1" for "option_label"
    And I fill in "Moving Right Along" for "personalize_elements_content"
    And I fill in "Muppet songs" for "title"
    And I press "Save" in the "dialog_variation_type_form" region

    # I verify my variation is updated.
    Then I should see the message "The variation has been updated." in the messagebox
    Then I should not see the variation type form dialog
    And I should see the text "Moving Right Along" in the "page_content" region
    And I should see "1" for the "variation set" count
    When I hover over "What" in the "lift_tray" region
    Then I should see the text "Muppet songs" in the "lift_tray" region
    And I should visibly see the link "Variation 1" in the "lift_tray" region

  Scenario: Delete an existing personalize element variation for an acquia_lift campaign.
    # I have a campaign and a variation set.
    # I login with the marketer role.
    # I am on an article page.
    Given "acquia_lift_target" agents:
      | machine_name                       | label                              |
      | testing-campaign-delete-variations | Testing campaign delete variations |
    And personalized elements:
      | label              | agent                              | selector    | type     | content                                    |
      | Page title updated | testing-campaign-delete-variations | #page-title | editText | The Rainbow Connection, Moving Right Along |
    And I am logged in as a user with the "access administration pages,access toolbar,administer visitor actions,manage personalized content" permission
    And I am viewing an "Article" content:
      | title | Test Article Title - Original |
    When I click "Acquia Lift" in the "menu" region
    Then I should visibly see the link "Personalizations" in the "lift_tray" region

    # I open the variation set's menu.
    When I hover over "Personalizations" in the "lift_tray" region
    And I click "Testing campaign delete variations" in the "lift_tray" region
    Then I should visibly see the link "What" in the "lift_tray" region
    And I should see "1" for the "variation set" count

    # I bring up the "Delete" variation set interface.
    When I hover over "What" in the "lift_tray" region
    Then I should see the text "Page title updated" in the "lift_tray" region
    And I should visibly see the link "Option A" in the "lift_tray" region
    And I should visibly see the link "Option B" in the "lift_tray" region
    And "Page title updated" set "Control variation" variation should not have the "Edit" link
    And "Page title updated" set "Control variation" variation should not have the "Delete" link
    And "Page title updated" set "Option A" variation should have the "Edit" link
    And "Page title updated" set "Option A" variation should have the "Delete" link
    And "Page title updated" set "Option B" variation should have the "Edit" link
    And "Page title updated" set "Option B" variation should have the "Delete" link

    # I delete a variation, "Option A".
    When I click "Delete" link for the "Page title updated" set "Option A" variation
    Then I should see the modal with title "Delete variation"
    When I press "Delete"

    # I verify the variation "Option A" is deleted.
    Then I should see the message "The variation has been deleted." in the messagebox
    Then I should see "1" for the "variation set" count
    When I hover over "What" in the "lift_tray" region
    Then I should see the text "Page title updated" in the "lift_tray" region
    And I should not visibly see the link "Option A" in the "lift_tray" region
    And I should visibly see the link "Option B" in the "lift_tray" region
    And "Page title updated" set "Control variation" variation should not have the "Edit" link
    And "Page title updated" set "Control variation" variation should not have the "Delete" link
    And "Page title updated" set "Option B" variation should have the "Edit" link
    And "Page title updated" set "Option B" variation should have the "Delete" link

    # I delete another variation, "Option B".
    When I click "Delete" link for the "Page title updated" set "Option B" variation
    Then I should see the modal with title "Delete variation"
    When I press "Delete"

    # I verify the variation "Option B" is deleted.
    Then I should see the message "The variation set has been deleted." in the messagebox
    Then I should see "0" for the "variation set" count

    # I verify both variations are deleted and there is none left.
    When I hover over "What" in the "lift_tray" region
    Then I should not see the text "Page title updated" in the "lift_tray" region
    And I should not visibly see the link "Option B" in the "lift_tray" region
    And I should not visibly see the link "Control variation" in the "lift_tray" region
    And I should see the text "No variations" in the "lift_tray" region


  Scenario: Add a new element variation set to a campaign and be automatically
    and continue through campaign workflow.
    # I have a campaign and a variation set.
    # I login with the marketer role.
    # I have an article page.
    Given "acquia_lift_target" agents:
      | machine_name               | label                                  |
      | testing-campaign-roundtrip | Testing campaign round trip variations |
    And "article" content:
      | title            | author     | status |
      | My article title | Joe Editor | 1      |

    And I am logged in as a user with the "access administration pages,access toolbar,administer visitor actions,manage personalized content" permission
    And I am on "admin/structure/personalize/manage/testing-campaign-roundtrip/variations"

    # I add a new personalized elements variation.
    When I press "Add variation set" in the "campaign_workflow_form" region
    Then I should see the text "Webpage elements" in the "campaign_workflow_form" region

    When I check the "Webpage elements" radio button
    And I wait for AJAX to finish
    # Todo create a custom assertion for clicking a radio button list option
    And I fill in "node" for "variations[editing][new][0][element][content][url]"
    And I press "Go" in the "campaign_workflow_form" region

    # I select an element to personalize.
    When I click "My article title" in the "page_content" region
    Then I should see the text "<A>" in the "dialog_variation_type" region
    When I click "Edit text" in the "dialog_variation_type" region
    Then I should not see the variation type dialog

    # I enter a new variation and choose to "continue".
    And I should see the text "Edit text: <A>" in the "dialog_variation_type_form" region
    And the "title" field should contain ""
    And the "option_label" field should contain ""
    And the "personalize_elements_content" field should contain text that has "My article title"
    When I fill in "My Article Title - Updated 1" for "personalize_elements_content"
    And I fill in "Test variation set" for "title"
    And I fill in "My variation" for "option_label"
    And I press "Save" in the "dialog_variation_type_form" region

    # I should be redirected back to the campaign page.
    Then I should be on "admin/structure/personalize/manage/testing-campaign-roundtrip/variations"

  Scenario: Create a campaign with testing for audiences and then select a winner
    for the test.
    # I have a campaign, audiences, and variation sets with targeting.
    # I log in with the marketer role.
    Given "acquia_lift_target" agents:
      | machine_name                   | label                            |
      | testing-campaign-select-winner | Testing campaign select a winner |
    And personalized elements:
      | label                | agent                          | selector    | type     | content                                                          |
      | Muppet title updated | testing-campaign-select-winner | #page-title | editText | The Rainbow Connection, Movin Right Along, Landstrider, Skeksis  |
      And audiences:
      | label         | agent                          |
      | Muppet fans   | testing-campaign-select-winner |
      | Creature fans | testing-campaign-select-winner |
    And targeting:
      | agent                          | audience      | options                    |
      | testing-campaign-select-winner | muppet-fans   | option-1,option-2          |
      | testing-campaign-select-winner | creature-fans | option-3,option-4          |
      | testing-campaign-select-winner | everyone-else | option-1,option-2,option-4 |
    And the "testing-campaign-select-winner" personalization has the "Running" status
    And I am logged in as a user with the "access administration pages,access toolbar,administer visitor actions,manage personalized content" permission

    # I should see buttons to set the winner for all audiences
    When I am on "admin/structure/personalize/manage/testing-campaign-select-winner/targeting"
    Then I should see the button "acquia-lift-complete-muppet-fans" in the "campaign_workflow_form" region
    And I should see the button "acquia-lift-complete-creature-fans" in the "campaign_workflow_form" region
    And I should see the button "acquia-lift-complete-everyone-else" in the "campaign_workflow_form" region

    # When I open the form to select a winner
    When I press "acquia-lift-complete-muppet-fans" in the "campaign_workflow_form" region
    Then I should see the modal with title "Choose test winner"
    And I should see the text "Option A" in the "modal_content" region
    And I should see the text "Option B" in the "modal_content" region
    And I should not see the text "Option C" in the "modal_content" region
    And I should not see the text "Option D" in the "modal_content" region

    # When I select a winner
    When I check the "Option A" radio button
    And I press "Complete test" in the "modal_content" region

    # The page should refresh.
    Then I should not see the modal

    # Pause the campaign so that we can "see" all of the targeting elements
    And I should not see the "#acquia-lift-complete-muppet-fans" element in the "campaign_workflow_form" region
    And I should see the button "acquia-lift-complete-creature-fans" in the "campaign_workflow_form" region
    And I should see the button "acquia-lift-complete-everyone-else" in the "campaign_workflow_form" region
    And "Option A" variation should be assigned to the "Muppet fans" audience
    And "Option B" variation should not be assigned to the "Muppet fans" audience
