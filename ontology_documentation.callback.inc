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
  drupal_add_library('system', 'drupal.collapse');
  return entity_view(ENTITY_TYPE, array($entity));
}

function ontology_documentation_term_edit($entity) {
  drupal_set_title($entity->title);
  $link = array(
    array(
      'data' => array(
        '#type' => 'link',
        '#title' => 'prova',
        '#href' => 'http://exmample.org',
      ),
    )
  );
  return drupal_render($link);
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
        ontology_documentation_create_terms_page('Concepts');
        ontology_documentation_create_terms_page('Roles');
        ontology_documentation_create_terms_page('Attributes');
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

function ontology_documentation_mastro_get_data($operation = NULL, $parameters = array(), $field = NULL, $bundle = NULL) {
  $mastro_functions_per_ontology_field = &drupal_static('functions' . __FUNCTION__, array());
  $wsdl = &drupal_static('wsdl' . __FUNCTION__);

  if(empty($mastro_functions_per_ontology_field)) {
    $mastro_functions_per_ontology_field = array(
      'occorrenza' => array(
        'concept' => array(
           'keys' => array('concept'),
           'operation' => 'getConceptGraphMLLinks',
        ),
        'role' => array(
          'keys' => array('role'),
          'operation' => 'getRoleGraphMLLinks',
        ),
        'attribute' => array(
          'keys' => array('attribute'),
          'operation' => 'getAttributeGraphMLLinks',
        ),
      ),
      'specializzazioni' => array(
        'concept' => array(
          'keys' => array('conceptName', 'renderingMode'),
          'operation' => 'getAssertedSubAtomicConcepts',
        ),
        'role' => array(
          'keys' => array('roleName', 'renderingMode'),
          'operation' => 'getAssertedSubAtomicRoles',
        ),
      ),
      'generalizzazioni' => array(
        'concept' => array(
          'keys' => array('conceptName', 'renderingMode'),
          'operation' => 'getAssertedSuperAtomicConcepts',
        ),
        'role' => array(
          'keys' => array('roleName', 'renderingMode'),
          'operation' => 'getAssertedSuperAtomicRoles',
        ),
      ),
      'tipizzazione_domain' => array(
        'role' => array(
          'keys' => array('roleName'),
          'operation' => 'getRoleDomain',
        ),
        'attribute' => array(
          'keys' => array('attributeName'),
          'operation' => 'getAttributeDomain',
        ),
      ),
      'tipizzazione_range' => array(
        'role' => array(
          'keys' => array('roleName'),
          'operation' => 'getRoleRange',
        ),
        'attribute' => array(
          'keys' => array('attributeName'),
          'operation' => 'getAttributeRange',
        ),
      ),
      'attributi_obbligatori' => array(
        'concept' => array(
          'keys' => array('conceptName'),
          'operation' => 'getRelevantMandatoryAttributes',
        ),
      ),
      'attributi_non_obbligatori' => array(
        'concept' => array(
          'keys' => array('conceptName'),
          'operation' => 'getRelevantOptionalAttributes',
        ),
      ),
      'ruoli_obbligatori' => array(
        'concept' => array(
          'keys' => array('conceptName'),
          'operation' => 'getRelevantMandatoryRoles',
        ),
      ),
      'ruoli_non_obbligatori' => array(
        'concept' => array(
          'keys' => array('conceptName'),
          'operation' => 'getRelevantOptionalRoles',
        ),
      ),
    );
  }
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

  $operation = isset($field) && isset($bundle) ? $mastro_functions_per_ontology_field[$field][$bundle]['operation'] : $operation;
  $array_keys = isset($field) && isset($bundle) ? $mastro_functions_per_ontology_field[$field][$bundle]['keys'] : range(0,100);
  if(count($array_keys) == count($parameters)) {
    $parameters = array(array_combine($array_keys, $parameters));
  }
  $parameters = is_array(current($parameters)) ? $parameters : array($parameters);
  $result = call_user_func_array(array($wsdl['soap'], $operation), $parameters);
  if(property_exists($result, 'return')) {
    return is_array($result->return) ? $result->return : array($result->return);
  }
  return array();
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

function ontology_documentation_create_terms_page($type) {
  $data = array(
    'title' => '',
    'category' => '',
  );
  switch($type) {
    case 'Attributes':
      $data['category'] = 'attribute';
    case 'Roles':
      $data['category'] = empty($data['category']) ? 'role' : $data['category'];
    case 'Concepts':
      $data['category'] = empty($data['category']) ? 'concept' : $data['category'];
      // Ottengo tutti i campi per specifico bundle
      $fields = field_info_instances(ENTITY_TYPE, $data['category']);
      // Ottengo tutti i concetti
      $mastro_terms = ontology_documentation_mastro_get_data('getAll' . $type);
      // Nel caso limite di un solo termine , crea un array che lo contiene
      if(!is_array($mastro_terms)) {
        if(!empty($mastro_terms)) {
          $mastro_terms = array($mastro_terms);
        }
        else {
          throw new Exception('Ontology ' . $type . ' not found!');
        }
      }
      // Itero sui termini
      foreach($mastro_terms as $term) {
        // Assegno il titolo alle proprietà dell'entità
        $data['title'] = $term;
        // Creo una nuova entità con le proprietà di base
        $entity = entity_create(ENTITY_TYPE, $data);
        if($entity) {
          // Creo il wrapper per manipolare i campi 'attaccati' all'entità
          $entity_wrapper = entity_metadata_wrapper(ENTITY_TYPE, $entity);
          // Itero sui campi da assegnare
          foreach($fields as $field){
            // Configuro i campi
            ontology_documentation_setup_field($field['field_name'], 'wiki_text', $entity_wrapper);
          }
          // Salvo la pagina
          ontology_documentation_save($entity_wrapper);
        }
        else
          throw new Exception('entity_create(): Entity Controller not found!');
      }
      break;
  }
}

function ontology_documentation_item_list_child_render($render_element) {
  foreach($render_element['#items'] as $key => $content) {
    if(isset($render_element['#items'][$key]['data'])) {
      $render_element['#items'][$key]['data'] = drupal_render($render_element['#items'][$key]['data']);
    }
  }
  return $render_element;
}

function ontology_documentation_fieldset_child_reder($render_element) {
  if(isset($render_element['#children'])) {
    $render_element['#children'] = drupal_render($render_element['#children']);
    return $render_element;
  }
  return $render_element;
}

function ontology_documentation_setup_field($field_name, $text_format = 'plain_text', &$wrapper) {
  // Definisco variabili utili
  $entity_type = $wrapper->type();
  $bundle = $wrapper->getBundle();
  $term = $wrapper->value()->title;
  $graph_viewer = 'graphml-viewer';

  $field_data = array(
    'label' => array(
      '#theme' => 'html_tag',
      '#tag' => 'h2',
      '#value' => ucwords(str_replace('_', ' ', $field_name)),
      '#printed' => true,
    ),
    'page_link' =>array(
      '#type' => 'link',
      '#title' => '',
      '#href' => '',
      '#printed' => true,
    ),
    'item_list' => array(
      '#theme' => 'item_list',
      '#items' => array(),
      '#type' => 'ul',
      '#pre_render' => array('ontology_documentation_item_list_child_render'),
      '#printed' => true,
    ),
    'description' => array(
      '#type' => 'html_tag',
      '#tag' => 'p',
      '#value' => '',
      '#printed' => true,
    ),
    'fieldset_box' => array(
      '#theme' => 'fieldset',
      '#title' => '',
      '#children' => array(
        '#theme' => 'item_list',
        '#items' => array(),
        '#type' => 'ul',
        '#pre_render' => array('ontology_documentation_item_list_child_render'),
      ),
      '#attributes' => array(
        'class' => array(
          'collapsible',
          'collapsed',
        ),
      ),
      '#printed' => true,
      '#pre_render' => array('ontology_documentation_fieldset_child_reder'),
    ),
  );


  // Imposto il tipo di formato del testo
  $wrapper->$field_name->format->set($text_format);

  switch($field_name) {
    case 'categoria':
      $field_data['label']['#printed'] = false;
      $field_data['page_link']['#printed'] = false;
      $field_data['page_link']['#title'] = $bundle.'s';
      $field_data['page_link']['#href'] = DOCUMENTATION_BASE_URL.'/'.$bundle.'s';
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    case 'descrizione':
      $field_data['label']['#printed'] = false;
      $field_data['description']['#printed'] = false;
      $field_data['description']['#value'] = ontology_documentation_get_wiki_term_description($term);
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    case 'occorrenza':
      $field_data['fieldset_box']['#printed'] = false;
      $field_data['page_link']['#printed'] = false;
      $field_data['fieldset_box']['#title'] = t('Occorrenze nei diagrammi');
      $diagram_links = ontology_documentation_mastro_get_data(NULL, array($term), $field_name, $bundle);
      foreach($diagram_links as $index => $diagram_link) {
        $text = explode('.', $diagram_link);
        $field_data['page_link']['#title'] = current(array_slice($text, 0, 1)).': '.($index + 1);
        $field_data['page_link']['#href'] = $graph_viewer.'/'.$diagram_link;
        $field_data['fieldset_box']['#children']['#items'][$index] = array('data'=> $field_data['page_link']);
      }
      $field_data['page_link']['#printed'] = true;
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    // 'generalizzazioni' esegue lo stesso setup di 'specializzazioni'
    case 'generalizzazioni':
    case 'specializzazioni':
      $field_data['label']['#printed'] = false;
      $specializations = ontology_documentation_mastro_get_data(NULL, array($term, 'FUNCTIONAL_STYLE_SYNTAX'), $field_name, $bundle);
      if(!empty($specializations)) {
        $field_data['item_list']['#printed'] = false;
        $field_data['page_link']['#printed'] = false;
        foreach($specializations as $index => $item) {
          $field_data['page_link']['#title'] = $item;
          $field_data['page_link']['#href'] = DOCUMENTATION_BASE_URL.'/'.$bundle.'/'.str_replace('_', '-', $item);
          $field_data['item_list']['#items'][$index] = array('data' => $field_data['page_link']);
        }
        $field_data['page_link']['#printed'] = true;
      }
      else {
        $field_data['description']['#printed'] = false;
        $field_data['description']['#value'] = 'Non sono definite specializzazioni per questo ' . $bundle;
      }
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    case 'tipizzazione':
      $field_data['label']['#printed'] = false;
      $field_data['item_list']['#printed'] = false;
      $domain = ontology_documentation_mastro_get_data(NULL, array($term), $field_name.'_domain', $bundle);
      $domain = !empty($domain) ? 'Dominio: '.current($domain).'<p></p>' : 'Dominio: non tipizzato';
      $field_data['item_list']['#items'][] = $domain;
      $range = ontology_documentation_mastro_get_data(NULL, array($term), $field_name.'_range', $bundle);
      $range = !empty($range) ? 'Codominio: '.current($range) : 'Codominio: non tipizzato';
      $field_data['item_list']['#items'][] = $range;
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    // Per entrampi i campi è definito lo stesso setup
    case 'ruoli_non_obbligatori':
    case 'ruoli_obbligatori':
    case 'attributi_non_obbligatori':
    case 'attributi_obbligatori':
      $field_data['label']['#printed'] = false;
      $current_field_type = current(array_slice(explode('_', $field_name), 0,1));
      $current_field_bundle = !strcmp($current_field_type, 'attributi') ? 'attribute' : 'role';
      $mandatory_attributes = ontology_documentation_mastro_get_data(NULL, array($term), $field_name, $bundle);
      if(!empty($mandatory_attributes)) {
        $field_data['item_list']['#printed'] = false;
        $field_data['page_link']['#printed'] = false;
        foreach($mandatory_attributes as $index => $attribute) {
          $field_data['page_link']['#title'] = $attribute;
          $field_data['page_link']['#href'] = DOCUMENTATION_BASE_URL.'/'.$current_field_bundle.'/'.str_replace('_', '-', $attribute);
          $field_data['item_list']['#items'][$index] = array('data' => $field_data['page_link']);
        }
        $field_data['page_link']['#printed'] = true;
      }
      else {
        $field_data['description']['#printed'] = false;
        $field_data['description']['#value'] = array_search('non', explode('_', $field_name)) === false ?
          'Non sono definiti '.$current_field_type.' rilevanti obbligatori per questo concetto' :
          'Non sono definiti '.$current_field_type.' rilevanti non obbligatori per questo concetto';
      }
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    case 'assiomi':

      break;
    case 'link_obda':
      break;
    case 'mappings':
      break;
  }
}