<?php

function ontology_documentation_view() {
  return 'Table view';
}

function ontology_documentation_concept_create() {
  return 'Concept create';
}

function ontology_documentation_role_create() {
  return 'Role create';
}

function ontology_documentation_attribute_create() {
  return 'Attribute create';
}

function ontology_documentation_term_view($entity) {
  drupal_set_title($entity->title);
  return 'In VIEW';
}

function ontology_documentation_term_edit($entity) {
  drupal_set_title($entity->title);
  return 'In EDIT';
}

function ontology_documentation_config_form($form, &$form_state) {
  $form['form_header'] = array(
    '#type' => 'html_tag',
    '#tag' => 'h2',
    '#options' => array('class' => 'onto_form_header'),
    '#value' => 'Ontology documentation upgrade form'
  );
  $form['form_delete_check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete OWL documentation pages'),
    '#description' => t('All the documentation will drop off'),
    '#disabled' => variable_get('empty_owl_documentation', FALSE),
    '#default_value' => FALSE,
  );
  $form['form_upgrade_check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Upgrade OWL documentation pages'),
    '#description' => t('Documentation pages will be upgraded'),
    '#default_value' => TRUE,
  );
  $form['form_submit'] = array(
    '#type' => 'submit',
    '#value' => 'Ok',
  );
  $form['form_submit_cancel'] = array(
    '#type' => 'button',
    '#value' => 'Cancel',
  );
  return $form;
}

function ontology_documentation_config_form_submit($form, &$form_state) {
  if($form_state['values']['form_delete_check'] == 1 || $form_state['values']['form_upgrade_check'] ==1) {
   $documentation_ids = ontology_documentation_get_id();
   if($form_state['values']['form_delete_check'] == 1)
     if($documentation_ids) {
       ontology_documentation_delete($documentation_ids);
       drupal_set_message('Task deleting has been succesfully completed!');
     }
     else {
       variable_set('empty_owl_documentation', TRUE);
       drupal_set_message('Error during deleting: entity_load()!', 'error');
     }
   if($form_state['values']['form_upgrade_check'] == 1) {
    drupal_set_message('AGGIORNAMENTO', 'warning');
    if($documentation_ids) {
      ontology_documentation_create('UPDATE');
    }
    else {
      ontology_documentation_create('NEW');
    }
   }
 }
 else
  drupal_set_message('NESSUNA SELEZIONE', 'error');
}

function ontology_documentation_get_id($ontology_type = 'ontology_documentation', $id = FALSE) {
  $return = entity_load($ontology_type, $id);
  return !empty($return) ? $return : FALSE;
}

function ontology_documentation_delete($ids, $entity_type = ENTITY_TYPE) {
  entity_delete_multiple($entity_type, array_keys($ids));
  variable_set('empty_owl_documentation', TRUE);
  return TRUE;
}

function ontology_documentation_create($mode) {
  $wsdl = variable_get('ws_url');

  $options = array(
    'cache_wsdl' => WSDL_CACHE_NONE,
    'trace' => TRUE,
  );

  $data = array(
    'title' => '',
    'category' => '',
  );

  $soap = new SoapClient($wsdl, $options);
  $concepts = mastro_callWSWithArrayResponse('getAllConcepts');
  $roles = mastro_callWSWithArrayResponse('getAllRoles');
  $attributes = mastro_callWSWithArrayResponse('getAllAttributes');

  $term_data = &$data;
  switch($mode) {
    case 'NEW':
      // Creo concetti
      $term_data['category'] = 'concept';
      foreach($concepts as $concept) {
        $term_data['title'] = $concept;
        if(($new_entity = entity_create(ENTITY_TYPE, $term_data))) {

          // CREO I CONCETTI
          //$entity_wrapper = entity_metadata_wrapper(ENTITY_TYPE, $new_entity);
          //$entity_wrapper->categoria->set('Concept');
          //$entity_wrapper->save();
          $new_entity->dominio = 'prova';
          $new_entity->save();

        }
        else {
          drupal_set_message('Error: entity_create()!', 'error');
          return;
        }

      }
      drupal_set_message('NEW concepts have been created!');

      $term_data['category'] = 'role';
      foreach($roles as $role) {
        $term_data['title'] = $role;
        if(($new_entity = entity_create('ontology_documentation', $term_data))) {
          // CREO I CONCETTI
          //$entity_wrapper = entity_metadata_wrapper(ENTITY_TYPE, $new_entity);
          //$entity_wrapper->categoria->set('Concept');
          //$entity_wrapper->save();
          //$entity_wrapper->field_categoria->set('prova valore');
          //$entity_wrapper->save();
          //drupal_set_message('<pre>'.print_r($new_entity,TRUE).'</pre>');
          //$new_entity->categoria = 'CONCETTO';
          $new_entity->save();

        }
        else {
          drupal_set_message('Error: entity_create()!', 'error');
          return;
        }
      }
      drupal_set_message('NEW roles have been created!');
      break;
    case 'UPDATE':
      break;
  }
  variable_set('empty_owl_documentation', FALSE);
}

//CHIAMATA DA entity_access(), funzione implementata da Entity API
function ontology_documentation_access($op, $entity, $account, $entity_type) {
  global $user;

  if(!isset($account)) {
    $account = $user;
  }
  // CONTROLLARE QUI!!
  if(isset($entity->uid))
    $entity_owner = $entity->uid;
  else
    $entity_owner = FALSE;

  if(!empty($op)) {
    if($op == 'create')
      return user_access('administer ontology_documentation entities', $account) ||
             user_access('create ontology_documentation entities', $account);
    elseif($op == 'view')
      return user_access('administer ontology_documentation entities', $account) ||
             user_access('view ontology_documentation entities', $account);
    // Per la gestione della UI di amministrazione
    elseif($op == 'manage' || $op == 'update' || $op == 'delete')
      return user_access('administer ontology_documentation entities', $account);
    elseif($op == 'edit')
      return user_access('administer ontology_documentation entities', $account) ||
             user_access('edit any ontology_documentation entities', $account)   ||
             user_access('edit own ontology_documentation entities', $account)   &&
             $account->uid == $entity_owner && $entity->entityType() == $entity_type;
  }
  else
    return FALSE;
}

function ontology_documentation_type_access($op, $entity, $account, $entity_type) {

}