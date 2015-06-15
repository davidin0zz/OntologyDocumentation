<?php

// Form chiamato per la UI di Amministrazione
function ontology_documentation_form($form, &$form_state, $profile_type, $op = 'edit', $entity_type = NULL){
  if ($op == 'clone') {
    // Only label is provided for cloned entities.
    $profile_type->label .= ' (cloned)';
    $profile_type->type = $entity_type . '_clone';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($profile_type->label) ? $profile_type->label : '',
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($profile_type->type) ? $profile_type->type : '',
    '#disabled' => $profile_type->isLocked(),
    '#machine_name' => array(
      'exists' => 'profile2_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this profile type. It must only contain lowercase letters, numbers, and underscores.'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save profile type'),
    '#weight' => 40,
  );
  return $form;
}

function ontology_documentation_form_submit(&$form, &$form_state){
 $profile_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  //$profile_type->save();
  $form_state['redirect'] = 'admin/structure/profiles';
}