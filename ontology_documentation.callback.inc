<?php

function ontology_documentation_term_view($entity) {
  drupal_add_library('system', 'drupal.collapse');
  drupal_set_title($entity->title);
  return entity_view(__documentation_type__, array($entity));
}

function ontology_documentation_config_form_cancel($form, &$form_state) {
  drupal_goto('admin/config');
}

function ontology_documentation_config_form($form, &$form_state) {
  return ontology_documentation_util('config_form');
}

function ontology_documentation_config_form_submit($form, &$form_state) {
  $form_value = $form_state['values'];

  if ($form_value['delete_check'] == 1) {
    if (ontology_documentation_delete()) {
      drupal_set_message('Task completed!');
    }
    else {
      drupal_set_message('Task error!', 'error');
    }
  }
  if ($form_state['values']['upgrade_check'] == 1) {
    try {
      ontology_documentation_create();
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
    }
    variable_set('empty_owl_documentation', FALSE);
    drupal_set_message('Task completed!');
  }
}