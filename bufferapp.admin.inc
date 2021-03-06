<?php

/**
 * @file Administration form for Buffer module
 */

/**
 * Declare Buffer settings form.
 */
function bufferapp_settings_form($form) {

  $form['connection'] = array(
      '#type' => 'fieldset',
      '#title' => t('Connection'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
  );

  $form['connection']['bufferapp_access_token'] = array(
      '#type' => 'textfield',
      '#title' => t('Access token'),
      '#default_value' => config_get('bufferapp.settings', 'bufferapp_access_token'),
      '#description' => t("Insert your access token. For more info please read <a href='@link'>BufferApp API documentation</a>.", array('@link' => 'https://bufferapp.com/developers/api/oauth')),
  );


  if (config_get('bufferapp.settings', 'bufferapp_access_token') != '') {
    $options = array();
    $bufferapp_profiles = bufferapp_profiles();
    $profiles_description = t('There are not profiles associated with this access token. Please verify it.');
    if (is_array($bufferapp_profiles)) {
      $profiles_description = t('Select profiles that will receive updates');
      foreach (bufferapp_profiles() as $profile) {
        $options[$profile->id] = $profile->formatted_username . ' [' . $profile->formatted_service . ']';
      }
    }

    $form['profiles'] = array(
        '#type' => 'fieldset',
        '#title' => t('Profiles'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#description' => $profiles_description,
        '#states' => array(
            'invisible' => array(
                ':input[name="access_token"]' => array('empty' => TRUE),
            ),
        ),
    );

    $form['profiles']['bufferapp_selected_profiles'] = array(
        '#type' => 'checkboxes',
        '#options' => $options,
        '#default_value' => config_get('bufferapp.settings', 'bufferapp_selected_profiles'),
    );

    $form['options'] = array(
        '#type' => 'fieldset',
        '#title' => t('Options'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
    );

    $form['options']['bufferapp_log'] = array(
        '#type' => 'checkbox',
        '#title' => t('Log'),
        '#default_value' => config_get('bufferapp.settings', 'bufferapp_log'),
        '#description' => t('Enable logging of error and updates.'),
    );

    $form['options']['bufferapp_debug'] = array(
        '#type' => 'checkbox',
        '#title' => t('Debug'),
        '#default_value' => config_get('bufferapp.settings', 'bufferapp_debug'),
        '#description' => t('Enable debug mode. Updates are not sent to Buffer, instead are written to watchdog.'),
    );
  }

  $form['actions']['#type']  = 'actions';
    $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save configuration')
    );

return $form;
}

function bufferapp_settings_form_submit($form, &$form_state)
{
foreach ($form_state['values'] as $key => $value) {
        config_set('bufferapp.settings', $key, $value);
    }
    backdrop_set_message(t('The configuration options have been saved.'));
}
