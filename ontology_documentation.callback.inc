<?php

function ontology_documentation_term_view($entity){
  $return = array(
    '#type' => 'html_tag',
    '#tag' => 'h2',
    '#attributes' => array(
      'id' => 'term_edit'
    ),
    '#value' => 'View for term page',
  );
  return(drupal_render($return));
}

function ontology_documentation_edit($array) {
  drupal_set_title('Pagina di edit per '.$array['termine']);
  $return = array(
    '#type' => 'html_tag',
    '#tag' => 'h2',
    '#attributes' => array(
      'id' => 'term_edit'
    ),
    '#value' => 'Edit '.$array['categoria'].' '.$array['termine'],
  );
  return(drupal_render($return));
}

function ontology_documentation_config_form($form, &$form_state){
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

function ontology_documentation_config_form_submit($form, &$form_state){
  if($form_state['values']['form_delete_check'] == 1 || $form_state['values']['form_upgrade_check'] ==1) {
   $documentation_ids = ontology_documentation_get_id();
   if($form_state['values']['form_delete_check'] == 1)
     if($documentation_ids){
       ontology_documentation_delete($documentation_ids);
       drupal_set_message('Task deleting has been succesfully completed!');
     }
     else{
       variable_set('empty_owl_documentation', TRUE);
       drupal_set_message('Error during deleting: entity_load()!', 'error');
     }
   if($form_state['values']['form_upgrade_check'] == 1) {
    drupal_set_message('AGGIORNAMENTO', 'warning');
    if($documentation_ids){
      ontology_documentation_create('UPDATE');
    }
    else{
      ontology_documentation_create('NEW');
    }
   }
 }
 else
  drupal_set_message('NESSUNA SELEZIONE', 'error');
}

function ontology_documentation_get_id($ontology_type = 'ontology_documentation', $id = FALSE){
  $return = entity_load($ontology_type, $id);
  return !empty($return) ? $return : FALSE;
}

function ontology_documentation_delete($ids, $entity_type = 'ontology_documentation'){
  entity_delete_multiple($entity_type, array_keys($ids));
  variable_set('empty_owl_documentation', TRUE);
  return TRUE;
}

function ontology_documentation_create($mode){
  $options = array();
  $data = array();

  $options['cache_wsdl'] = WSDL_CACHE_NONE;
  $options['trace'] = TRUE;
  $wsdl = variable_get('ws_url');
  $soap = new SoapClient($wsdl, $options);

  $data['title'] = '';
  $data['category'] = '';
  $data['language'] = 'en';
  $data['uid'] = get_mastro_uid();
  $data['created'] = REQUEST_TIME;
  $data['changed'] = REQUEST_TIME;

  $concepts = mastro_callWSWithArrayResponse('getAllConcepts');
  $roles = mastro_callWSWithArrayResponse('getAllRoles');
  $attributes = mastro_callWSWithArrayResponse('getAllAttributes');

  $term_data = &$data;
  switch($mode){
    case 'NEW':
      // Creo concetti
      $term_data['category'] = 'concept';
      foreach($concepts as $concept){
        $term_data['title'] = $concept;
        if(($new_entity = entity_create(entity_type, $term_data))){
          $entity_wrapper = entity_metadata_wrapper(entity_type, $new_entity);
          $entity_wrapper->categoria->set('Concept');
          $entity_wrapper->save();
        }
        else{
          drupal_set_message('Error: entity_create()!', 'error');
          return;
        }

      }
      drupal_set_message('NEW concepts have been created!');

      break;
    case 'UPDATE':
      break;
  }
  variable_set('empty_owl_documentation', FALSE);
}
