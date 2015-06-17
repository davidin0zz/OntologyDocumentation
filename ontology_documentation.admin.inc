<?php

function ontology_documentation_admin_edit_form($entity) {

  $form['title'] = array(
    '#type' => 'html_tag',
    '#tag' => 'h1',
    '#value' => 'Edit ' . $entity->label(),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#title' => 'Ok',
  );

  return $form;
}

function ontology_documentation_form($form, &$form_state, $project = NULL) {

  $form['name'] = array(
    '#title' => t('Project name'),
    '#type' => 'textfield',
    '#default_value' => isset($project->name) ? $project->name : '',
    '#required' => TRUE,
  );

  $form['description'] = array(
    '#title' => t('Project description'),
    '#type' => 'textarea',
    '#default_value' => isset($project->description) ? $project->description : '',
    '#required' => TRUE,
  );

  $form['deadline'] = array(
    '#title' => t('Project deadline'),
    '#type' => 'textfield',
    '#default_value' => isset($project->deadline) ? $project->deadline : '',
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($project->id) ? t('Update project') : t('Save project'),
    '#weight' => 50,
  );

  return $form;
}

/**
 * Submit handler for the project add/edit form.
 */
function ontology_documentation_form_submit($form, &$form_state) {
  $project = entity_ui_form_submit_build_entity($form, $form_state);
  $project->save();
  drupal_set_message(t('The project: @name has been saved.', array('@name' => $project->name)));
  $form_state['redirect'] = 'admin/projects';
}