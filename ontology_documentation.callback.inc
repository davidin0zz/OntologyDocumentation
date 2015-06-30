<?php

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
  drupal_add_library('system', 'drupal.collapse');
  drupal_set_title($entity->title);
  return entity_view(__documentation_type__, array($entity));
}

function ontology_documentation_add_revision($form, &$form_state, $entity) {
  return array();
}

function ontology_documentation_config_form_cancel($form, &$form_state) {
  drupal_goto('admin/config');
}

function ontology_documentation_config_form($form, &$form_state) {
  return ontology_documentation_util('config_form');
}

function ontology_documentation_config_form_submit($form, &$form_state) {
  if ($form_state['values']['config_form_delete_check'] == 1 || $form_state['values']['config_form_upgrade_check'] ==1) {
    $documentation_ids = ontology_documentation_get_id();
    if ($form_state['values']['config_form_delete_check'] == 1) {
     ontology_documentation_delete($documentation_ids);
     drupal_set_message('Selected \'delete\' task has been succesfully completed!');
     variable_set('empty_owl_documentation', TRUE);
   }
   if ($form_state['values']['config_form_upgrade_check'] == 1) {
    if (!empty($documentation_ids)) {
      ontology_documentation_create('UPDATE');
    }
    else {
      ontology_documentation_create('NEW');
      drupal_set_message('Selected \'update\' task has been succesfully completed!');
    }
   }
 }
 else
  drupal_set_message('No options are selected!', 'error');
}

function ontology_documentation_get_id($ontology_type = __documentation_type__, $id = FALSE) {
  $entity_ids = entity_load($ontology_type, $id);
  return !empty($entity_ids) ? $entity_ids : array();
}

function ontology_documentation_delete($ids, $entity_type = __documentation_type__) {
  entity_delete_multiple($entity_type, array_keys($ids));
  variable_set('empty_owl_documentation', TRUE);
  return TRUE;
}

function ontology_documentation_create($mode) {
   switch ($mode) {
    case 'NEW':
      try {
        ontology_documentation_create_terms_page('Concepts');
        ontology_documentation_create_terms_page('Roles');
        ontology_documentation_create_terms_page('Attributes');
      }
      catch (Exception $e) {
        drupal_set_message($e->getMessage(), 'error');
      }
      break;
    case 'UPDATE':
      break;
  }
  variable_set('empty_owl_documentation', FALSE);
}

function ontology_documentation_mastro_get_data($operation = NULL, $parameters = array(), $field = NULL, $bundle = NULL) {
  // Dichiaro statica la variabile
  $wsdl = &drupal_static('wsdl' . __FUNCTION__);
  // Ottengo la struttura con i nomi dei metodi di mastro e le chiavi dei parametri
  $mastro_methods_per_ontology_field = ontology_documentation_util('mastro_methods_structure');
  // Se non è definito il wdsl, lo definisco
  if (!isset($wsdl)) {
    $wsdl = array(
        'url' => variable_get('ws_url'),
        'options' => array(
          'cache_wsdl' => WSDL_CACHE_NONE,
          'trace' => TRUE,
        ),
    );
    $wsdl['soap'] = new SoapClient($wsdl['url'], $wsdl['options']);
  }

  $operation = isset($field) && isset($bundle) ? $mastro_methods_per_ontology_field[$field][$bundle]['operation'] : $operation;
  $array_keys = isset($field) && isset($bundle) ? $mastro_methods_per_ontology_field[$field][$bundle]['keys'] : range(0,100);
  if (count($array_keys) == count($parameters)) {
    $parameters = array(array_combine($array_keys, $parameters));
  }
  $parameters = is_array(current($parameters)) ? $parameters : array($parameters);
  $result = call_user_func_array(array($wsdl['soap'], $operation), $parameters);
  if (property_exists($result, 'return')) {
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

  if (empty($search_info)) {
    $search_info['start string'] = '== Descrizione ==';
    $search_info['end string'] = '==';
    $search_info['start offset'] = strlen($search_info['start string']);
    $search_info['end offset'] = - 1;
  }
  if (!isset($entity_field_query)) {
    $entity_field_query = new EntityFieldQuery();
  }
  //Azzero le precedenti condizioni
  $entity_field_query->propertyConditions = array();
  // Cerco l'id del nodo del wiki dell'omonimo termine
  $query_response = $entity_field_query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'wiki')
    ->propertyCondition('title', $term)
    ->execute();
  if (!empty($query_response)) {
    // Ottengo l'id del nodo
    $wiki_node_id = array_keys($query_response['node']);
    // Carico il nodo
    $wiki_node = entity_load('node', $wiki_node_id);
    if ($wiki_node) {
      // Creo il wrapper per il nodo
      $wiki_wrapper = entity_metadata_wrapper('node', current($wiki_node));
      // Ottengo il contenuto del body non interpretato
      $wiki_description = $wiki_wrapper->body->value->raw();
      // Filtro solo la descrizione
      $start = strpos($wiki_description, $search_info['start string']) + $search_info['start offset'];
      if ($start !== FALSE) {
        $fin = strpos($wiki_description, $search_info['end string'], $start) + $search_info['end offset'];
        if ($fin !== FALSE) {
          $wiki_description = trim(substr($wiki_description, $start, $fin - $start));
          return $wiki_description;
        }
      }
    }
    else
      throw new Exception('entity_create(): Entity Controller not found!');
  }
  return '';
}

function ontology_documentation_create_terms_page($type) {
  $data = array(
    'title' => '',
    'category' => '',
  );
  switch ($type) {
    case 'Attributes':
      $data['category'] = 'attribute';
    case 'Roles':
      $data['category'] = empty($data['category']) ? 'role' : $data['category'];
    case 'Concepts':
      $data['category'] = empty($data['category']) ? 'concept' : $data['category'];
      // Ottengo tutti i campi per specifico bundle
      $fields = field_info_instances(__documentation_type__, $data['category']);
      // Ottengo tutti i concetti
      $mastro_terms = ontology_documentation_mastro_get_data('getAll' . $type);
      // Nel caso limite di un solo termine , crea un array che lo contiene
      if (!is_array($mastro_terms)) {
        if (!empty($mastro_terms)) {
          $mastro_terms = array($mastro_terms);
        }
        else {
          throw new Exception('Ontology ' . $type . ' not found!');
        }
      }
      // Itero sui termini
      foreach ($mastro_terms as $term) {
        // Assegno il titolo alle proprietà dell'entità
        $data['title'] = $term;
        // Creo una nuova entità con le proprietà di base
        $entity = entity_create(__documentation_type__, $data);
        if ($entity) {
          // Creo il wrapper per manipolare i campi 'attaccati' all'entità
          $entity_wrapper = entity_metadata_wrapper(__documentation_type__, $entity);
          // Itero sui campi da assegnare
          foreach ($fields as $field) {
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
  foreach ($render_element['#items'] as $key => $content) {
    if (isset($render_element['#items'][$key]['data'])) {
      $render_element['#items'][$key]['data'] = drupal_render($render_element['#items'][$key]['data']);
    }
  }
  return $render_element;
}

function ontology_documentation_fieldset_child_reder($render_element) {
  if (isset($render_element['#children'])) {
    $render_element['#children'] = drupal_render($render_element['#children']);
    return $render_element;
  }
  return $render_element;
}

function ontology_documentation_setup_field($field_name, $text_format = 'plain_text', &$wrapper) {
  // Definisco le variabili utili
  $entity_type = $wrapper->type();
  $bundle = $wrapper->getBundle();
  $term = $wrapper->name->value();
  // Recupero il percorso di base del graphml viewer
  $graph_viewer =  ontology_documentation_util('graphml_viewer_path');
  // Recupero i tipi di assiomi
  $axiom_type = ontology_documentation_util('mastro_axiom_types');
  // Recupero i tipi di link
  $link_obda_type = ontology_documentation_util('obda_types');
  // Recupero la struttura dati dei campi
  $field_data = ontology_documentation_util('fields_data_structure');
  // Imposto il valore dell'etichetta del campo
  $field_data['label']['#value'] = ucwords(str_replace('_', ' ', $field_name));
  // Imposto il tipo di formato del testo
  $wrapper->$field_name->format->set($text_format);

  switch ($field_name) {
    case 'categoria':
      $field_data['label']['#printed'] = FALSE;
      $field_data['page_link']['#printed'] = FALSE;
      $field_data['page_link']['#title'] = $bundle.'s';
      $field_data['page_link']['#href'] = __documentation_view_path__.'/'.$bundle.'s';
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    case 'descrizione':
      $field_data['label']['#printed'] = FALSE;
      $field_data['description']['#printed'] = FALSE;
      $field_data['description']['#markup'] = ontology_documentation_get_wiki_term_description($term);
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    case 'occorrenza':
      $field_data['fieldset_box']['#printed'] = FALSE;
      $field_data['fieldset_box']['#children']['simple']['#printed'] = FALSE;
      $field_data['page_link']['#printed'] = FALSE;
      $field_data['fieldset_box']['#title'] = t('Occorrenze nei diagrammi');
      $diagram_links = ontology_documentation_mastro_get_data(NULL, array($term), $field_name, $bundle);
      foreach ($diagram_links as $index => $diagram_link) {
        $text = explode('.', $diagram_link);
        $field_data['page_link']['#title'] = current(array_slice($text, 0, 1)).': '.($index + 1);
        $field_data['page_link']['#href'] = $graph_viewer.'/'.$diagram_link;
        $field_data['fieldset_box']['#children']['simple']['#items'][$index] = array('data'=> $field_data['page_link']);
      }
      $field_data['page_link']['#printed'] = TRUE;
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    // 'generalizzazioni' esegue lo stesso setup di 'specializzazioni'
    case 'generalizzazioni':
    case 'specializzazioni':
      $field_data['label']['#printed'] = FALSE;
      $specializations = ontology_documentation_mastro_get_data(NULL, array($term, 'FUNCTIONAL_STYLE_SYNTAX'), $field_name, $bundle);
      if (!empty($specializations)) {
        $field_data['item_list']['#printed'] = FALSE;
        $field_data['page_link']['#printed'] = FALSE;
        foreach ($specializations as $index => $item) {
          $field_data['page_link']['#title'] = $item;
          $field_data['page_link']['#href'] = __documentation_view_path__.'/'.$bundle.'/'.str_replace('_', '-', $item);
          $field_data['item_list']['#items'][$index] = array('data' => $field_data['page_link']);
        }
        $field_data['page_link']['#printed'] = TRUE;
      }
      else {
        $field_data['description']['#printed'] = FALSE;
        $field_data['description']['#value'] = 'Non sono definite specializzazioni per questo ' . $bundle;
      }
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    case 'tipizzazione':
      $field_data['label']['#printed'] = FALSE;
      $field_data['item_list']['#printed'] = FALSE;
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
      $field_data['label']['#printed'] = FALSE;
      $current_field_type = current(array_slice(explode('_', $field_name), 0,1));
      $current_field_bundle = !strcmp($current_field_type, 'attributi') ? 'attribute' : 'role';
      $mandatory_attributes = ontology_documentation_mastro_get_data(NULL, array($term), $field_name, $bundle);
      if (!empty($mandatory_attributes)) {
        $field_data['item_list']['#printed'] = FALSE;
        $field_data['page_link']['#printed'] = FALSE;
        foreach ($mandatory_attributes as $index => $attribute) {
          $field_data['page_link']['#title'] = $attribute;
          $field_data['page_link']['#href'] = __documentation_view_path__.'/'.$current_field_bundle.'/'.str_replace('_', '-', $attribute);
          $field_data['item_list']['#items'][$index] = array('data' => $field_data['page_link']);
        }
        $field_data['page_link']['#printed'] = TRUE;
      }
      else {
        $field_data['description']['#printed'] = FALSE;
        $field_data['description']['#value'] = array_search ('non', explode('_', $field_name)) === FALSE ?
          'Non sono definiti '.$current_field_type.' rilevanti obbligatori per questo concetto' :
          'Non sono definiti '.$current_field_type.' rilevanti non obbligatori per questo concetto';
      }
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    case 'assiomi':
      $field_data['fieldset_box']['#printed'] = FALSE;
      $field_data['fieldset_box']['#title'] = t('Axioms');
      $field_data['fieldset_box']['#children']['axioms']['positive_inclusions']['#printed'] = FALSE;
      $field_data['fieldset_box']['#children']['axioms']['negative_inclusions']['#printed'] = FALSE;
      $field_data['fieldset_box']['#children']['axioms']['denial_assertions']['#printed'] = FALSE;
      $field_data['fieldset_box']['#children']['axioms']['functional_and_ID_contraints']['#printed'] = FALSE;
      $field_data['fieldset_box']['#children']['axioms']['epistemic_constraints']['#printed'] = FALSE;
      foreach ($field_data['fieldset_box']['#children']['axioms'] as $axiom_type_key => $axiom) {
        $field_data['fieldset_box']['#children']['axioms'][$axiom_type_key]['#title'] = t(ucwords(str_replace('_', ' ', $axiom_type_key)));
        foreach ($axiom_type[$axiom_type_key] as $axiom_type_integer) {
          $axioms_result = ontology_documentation_mastro_get_data(NULL, array($term, $axiom_type_integer, FALSE), $field_name, $bundle);
          $field_data['fieldset_box']['#children']['axioms'][$axiom_type_key]['#items'] = $axioms_result;
        }
      }
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    case 'link_obda':
      $field_data['fieldset_box']['#printed'] = FALSE;
      $field_data['fieldset_box']['#title'] = t('Links to OBDA Analysis');
      $field_data['fieldset_box']['#children']['simple']['#printed'] = FALSE;
      $field_data['page_link']['#printed'] = FALSE;
      $field_data['page_link']['#title'] = 'Inferred subsumptions of '.$term;
      $field_data['page_link']['#href'] = __reasoning_relative_path__.'/'.str_replace('_', '-', $term).'/'.$link_obda_type[$bundle];
      $field_data['fieldset_box']['#children']['simple']['#items'][] = array('data' => $field_data['page_link']);
      $field_data['page_link']['#title'] = 'Mappings of '.$term;
      $field_data['page_link']['#href'] = __mappings_relative_path__.'/'.str_replace('_', '-', $term);
      $field_data['fieldset_box']['#children']['simple']['#items'][] = array('data' => $field_data['page_link']);
      $field_data['page_link']['#printed'] = TRUE;
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
    case 'mappings':
      $field_data['fieldset_box']['#printed'] = FALSE;
      $field_data['fieldset_box']['#title'] = t('Mappings');
      $wrapper->$field_name->value->set(drupal_render($field_data));
      break;
  }
}