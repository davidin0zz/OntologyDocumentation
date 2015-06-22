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
  return entity_view(ENTITY_TYPE, array($entity));
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
   switch($mode) {
    case 'NEW':
      try {
        ontology_documentation_create_concepts_page(array('title'=>'', 'category' => 'concept'));
        //ontology_documentation_create_roles_page(array('title'=>'', 'category' => 'role'));
        //ontology_documentation_create_attributess_page(array('title'=>'', 'category' => 'attribute'));
      }
      catch(Exception $e) {
        drupal_set_message($e->getMessage(), 'error');
      }
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

function ontology_documentation_mastro_get_data($operation, $parameters = NULL) {
  $wsdl = &drupal_static(__FUNCTION__);
  if(!isset($wsdl)) {
    $wsdl = array(
        'url' => variable_get('ws_url'),
        'options' => array(
          'cache_wsdl' => WSDL_CACHE_NONE,
          'trace' => TRUE,
        ),
    );
    $wsdl['soap'] = new SoapClient($wsdl['url'], $wsdl['options']);
  }

  if(!isset($parameters))
    return call_user_func(array($wsdl['soap'], $operation))->return;
  else
    call_user_func_array(array($wsdl['soap'], $operation), $parameters)->return;
}

function  ontology_documentation_save(&$wrapper) {
  $wrapper->save();
}

function ontology_documentation_get_wiki_term_description($term) {
  $search_info = &drupal_static('search' . __FUNCTION__, array());
  $entity_field_query = &drupal_static('query' . __FUNCTION__);

  if(empty($search_info)) {
    $search_info['start string'] = '== Descrizione ==';
    $search_info['end string'] = '==';
    $search_info['start offset'] = strlen($search_info['start string']);
    $search_info['end offset'] = - 1;
  }
  if(!isset($entity_field_query)) {
    $entity_field_query = new EntityFieldQuery();
  }

  dpm($search_info);
  //Azzero le precedenti condizioni
  $entity_field_query->propertyConditions = array();
  // Cerco l'id del nodo del wiki dell'omonimo termine
  $query_response = $entity_field_query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'wiki')
    ->propertyCondition('title', $term)
    ->execute();
  // Ottengo l'id del nodo
  $wiki_node_id = array_keys($query_response['node']);
  // Carico il nodo
  $wiki_node = entity_load('node', $wiki_node_id);
  if($wiki_node) {
  // Creo il wrapper per il nodo
  $wiki_wrapper = entity_metadata_wrapper('node', current($wiki_node));
  // Ottengo il contenuto del body non interpretato
  $wiki_description = $wiki_wrapper->body->value->raw();
  // Filtro solo la descrizione
  $start = strpos($wiki_description, $search_info['start string']) + $search_info['start offset'];
  if($start !== FALSE) {
    $fin = strpos($wiki_description, $search_info['end string'], $start) + $search_info['end offset'];
    if($fin !== FALSE) {
      $wiki_description = trim(substr($wiki_description, $start, $fin - $start));
      return $wiki_description;
    }
    else
      return '';
  }
  else
    return '';
  }
  else
    throw new Exception('entity_create(): Entity Controller not found!');
}

function ontology_documentation_create_concepts_page($data) {
  // Carico i campi per il particolare bundle
  $fields = field_info_instances(ENTITY_TYPE, $data['category']);
  // Ottengo tutti i concetti
  $mastro_concepts = ontology_documentation_mastro_get_data('getAllConcepts');
  // Itero sui concetti
  foreach($mastro_concepts as $concept) {
    // Assegno il titolo alle proprietà dell'entità
    $data['title'] = $concept;
    // Creo una nuova entità con le proprietà di base
    $entity = entity_create(ENTITY_TYPE, $data);
    if($entity) {
      // Creo il wrapper per manipolare i campi 'attaccati' all'entità
      $entity_wrapper = entity_metadata_wrapper(ENTITY_TYPE, $entity);
      // Itero sui campi da assegnare
      foreach($fields as $field){
        // Configuro i campi
        ontology_documentation_setup_field($field['field_name'], 'full_html', $entity_wrapper);
      }
      // Salvo la pagina
      ontology_documentation_save($entity_wrapper);
    }
    else
      throw new Exception('entity_create(): Entity Controller not found!');
  }
}

function ontology_documentation_setup_field($field_name, $text_format = 'plain_text', &$wrapper) {
  // Definisco variabili utili
  $entity_type = $wrapper->type();
  $bundle = $wrapper->getBundle();
  $term = $wrapper->value()->title;

  // Definisco la struttura dell'etichetta uguale per tutti i campi
  $label = array('label_content' => array('#type' => 'html_tag','#tag' => 'h2','#value' => '',));

  switch($field_name) {
    case 'categoria':
      $label['label_content']['#value'] = ucwords($field_name);
      $content_field = array('link' => array('#type' => 'link','#title' => '','#href' => '',));
      $content_field['link']['#title'] = $bundle . 's';
      $content_field['link']['#href'] = DOCUMENTATION_BASE_URL . '/' . $bundle . 's';
      $wrapper->$field_name->format->set($text_format);
      $wrapper->$field_name->value->set(drupal_render(array_merge($label, $content_field)));
      break;
    case 'descrizione':
      $label['label_content']['#value'] = ucwords($field_name);
      $content_field = array('field_content' => array('#type' => 'html_tag','#tag' => 'em','#value' => '',));
      $description = ontology_documentation_get_wiki_term_description($term);
      $content_field['field_content']['#value'] = $description;
      $wrapper->$field_name->format->set($text_format);
      $wrapper->$field_name->value->set(drupal_render(array_merge($label, $content_field)));
      break;
    case 'occorrenza':

      break;
    case 'etichetta':
      break;
    case 'specializzazioni':
      break;
    case 'generalizzazioni':
      break;
    case 'dominio':
      break;
    case 'codominio':
      break;
    case 'attributi_obbligatori':
      break;
    case 'attributi_non_obbligatori':
      break;
    case 'ruoli_obbligatori':
      break;
    case 'ruoli_non_obbligatori':
      break;
    case 'assiomi':
      break;
    case 'link_obda':
      break;
    case 'mappings':
      break;
  }
}