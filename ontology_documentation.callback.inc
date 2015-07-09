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
    try {
      ontology_documentation_delete(__documentation_type__);
      drupal_set_message('Entire ontology documentation has been deleted!');
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
    }
  }
  if ($form_value['delete_old'] == 1) {
    try {
      ontology_documentation_delete(__documentation_type__, false);
      drupal_set_message('Pages labeled as old has been removed!');
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
    }
  }
  if ($form_state['values']['upgrade_check'] == 1) {
    $ontology_state = variable_get('syncronized_owl_documentation', true);
    try {
      if ($ontology_state) {
        ontology_documentation_create();
        drupal_set_message('Pages has been updated and status report has been automatically generated!');
      }
      else {
        drupal_set_message('Before starting the upgrade process, the ontology must be syncronized!', 'warning');
      }
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
    }
  }
}