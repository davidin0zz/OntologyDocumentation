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
  $wsdl = variable_get('ws_url');

  $options = array(
    'cache_wsdl' => WSDL_CACHE_NONE,
    'trace' => TRUE,
  );

  $terms_base_data = array(
    'title' => '',
    'category' => '',
  );

  $category = array(
     'label' => array(
       '#type' => 'html_tag',
       '#tag' => 'h2',
       '#value' => 'Categoria',
     ),
     'link' => array(
      '#type' => 'link',
      '#title' => '',
      '#href' => '',
    ),
  );

  $soap = new SoapClient($wsdl, $options);
  $ontology_terms_types = array(
    'concepts' => mastro_callWSWithArrayResponse('getAllConcepts'),
    'roles' => mastro_callWSWithArrayResponse('getAllRoles'),
    'attributes' => mastro_callWSWithArrayResponse('getAllAttributes'),
  );

   switch($mode) {
    case 'NEW':
      $bundles = ontology_documentation_get_bundles();
      //dpm($bundles);
      foreach($bundles as $bundle){
        $bundle_name = $bundle['machine_name'];
        $terms_base_data['category'] = $bundle_name;
        //dpm($bundle_fields);
          foreach($ontology_terms_types as $ontology_terms_type => $ontology_terms) {
            foreach($ontology_terms as $ontology_term) {
              $terms_base_data['title'] = $ontology_term;
              // Creo l'entità
              $entity = entity_create(ENTITY_TYPE, $terms_base_data);
              // Creo il wrapper
              $entity_wrapper = entity_metadata_wrapper(ENTITY_TYPE, $entity, array('language' => 'en'));
              // Imposto il text format per i campo categoria
              $entity_wrapper->categoria->format->set('full_html');
              switch($bundle_name) {
                case 'concept':
                  // Imposto il valore del campo categoria
                  $category['link']['#title'] = $bundle['human_label'].'s';
                  $category['link']['#href'] = DOCUMENTATION_BASE_URL . '/' . $bundle['machine_name'] . 's';
                  $entity_wrapper->categoria->value->set(drupal_render($category));
                  // Imposto il valore del campo occorrenza


                  //Salvo l'entità
                  $entity_wrapper->save();


                  break;
                case 'role':
                  break;
                case 'attribute':
                  break;
              }
              break;
            }
            break;
          }
        break;
      }

      break;
    case 'UPDATE':
      $en = entity_load_single('ontology_documentation', 16);
      dpm($en->identifier());
      $e2 = entity_load_single('ontology_documentation',$en->internalIdentifier());
      dpm($en2);
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

function getDiagramsLinksBox_($entity,$type){
  drupal_add_library('system', 'drupal.collapse');

  $links = array(
    '#title'       => 'Occorrenze nei diagrammi',
	'#children'    => '',
	'#collapsible' => true,
	'#collapsed'   => false,
	'#attributes'  => array(
    'class' => array('collapsible', 'collapsed')
	),
  );

  $links['#children'] = '<ul>'.getDiagramsLinks($entity,$type).'</ul>';

  if(variable_get('show_axioms', ''))
    return theme ('fieldset', array('element' => $links));
  else return '';
}
