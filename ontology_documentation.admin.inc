<?php

function ontology_documentation_type_form($form, &$form_state, $entity_type, $op) {
  if($op == 'edit') {
    $form['category'] = array(
      '#type' => 'textfield',
      '#title' => t('Documentation type machine name'),
      '#description' => t('A unique machine-readable name of this documentation type.'),
      '#default_value' => isset($entity_type->category) ? $entity_type->category : '',
      '#size' => '50',
      '#required' => TRUE,
    );
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Documentation type name'),
      '#description' => t('The human-readable name of this documentation type.'),
      '#size' => '50',
      '#default_value' => isset($entity_type->title) ? $entity_type->title : '',
      '#required' => TRUE,
    );
    $form['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Documentation type description'),
      '#description' => t('A short description of this documentation type.'),
      '#default_value' => isset($entity_type->description) ? $entity_type->description : '',
      '#required' => TRUE,
    );
    $form['locked'] = array(
      '#type' => 'radios',
      '#title' => t('Is locked type?'),
      '#description' => t('Enable deletion of this documentation type.'),
      '#options' => array(
        t('no'),
        t('yes'),
      ),
      '#default_value' => isset($entity_type->locked) ? $entity_type->locked : 0,
      '#required' => TRUE,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 50,
    );
    $form['cancel'] = array(
      '#type' => 'button',
      '#value' => t('Cancel'),
      '#weight' => 50,
    );
 }

  return $form;
}

function ontology_documentation_form($form, &$form_state, $entity_type, $op) {
  $form['button'] = array(
    '#type' => 'button',
    '#value' => 'Working progress',
  );

  return $form;
}

function ontology_documentation_type_form_submit($form, &$form_state) {
  $entity_type = entity_ui_form_submit_build_entity($form, $form_state);
  $entity_type->save();
  drupal_set_message(
    t('Ontology documentation type: @title has been saved.',
    array('@title' => $entity_type->title)
  ));
  $form_state['redirect'] = 'admin/structure/ontology-documentation-types';
}
