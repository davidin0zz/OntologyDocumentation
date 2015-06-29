<?php

class OntologyDocumentationUtil {

  protected static $graphMLViewerPath = 'graphml-viewer';

  protected static $obdaTypes = array(
    'concept' => 0,
    'role' => 1,
    'attribute' => 2,
  );

  protected static $mastroAxiomTypes = array(
    'positive_inclusions' => array(0),
    'negative_inclusions' => array(1),
    'functional_and_ID_contraints' => array(2, 3),
    'denial_assertions' => array(4),
    'epistemic_constraints' => array(5),
  );

  protected static $mastroMethodsStructure = array(
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
   'assiomi' => array(
     'concept' => array(
       'keys' => array('conceptName', 'axiomType', 'format'),
       'operation' => 'getMastroAxiomsByConcept',
     ),
     'role' => array(
       'keys' => array('roleName', 'axiomType', 'format'),
       'operation' => 'getMastroAxiomsByRole',
     ),
     'attribute' => array(
       'keys' => array('attributeName', 'axiomType', 'format'),
       'operation'=> 'getMastroAxiomsByAttribute',
     ),
   ),
 );

  protected static $fieldsDataStructure = array(
    'label' => array(
      '#theme' => 'html_tag',
      '#tag' => 'h2',
      '#value' => '',
      '#printed' => TRUE,
    ),
    'page_link' =>array(
      '#type' => 'link',
      '#title' => '',
      '#href' => '',
      '#printed' => TRUE,
    ),
    'item_list' => array(
      '#theme' => 'item_list',
      '#items' => array(),
      '#type' => 'ul',
      '#pre_render' => array('ontology_documentation_item_list_child_render'),
      '#printed' => TRUE,
    ),
    'description' => array(
      '#markup' => '',
      //'#tag' => 'p',
      //'#value' => '',
      '#printed' => TRUE,
    ),
    'fieldset_box' => array(
      '#theme' => 'fieldset',
      '#title' => '',
      '#children' => array(
        'simple' => array(
          '#theme' => 'item_list',
          '#items' => array(),
          '#type' => 'ul',
          '#printed' => TRUE,
          '#pre_render' => array('ontology_documentation_item_list_child_render'),
        ),
        'axioms' => array(
          'positive_inclusions' => array(
            '#theme' => 'item_list',
            '#items' => array(),
            '#title' => '',
            '#type' => 'ul',
            '#printed' => TRUE,
            '#pre_render' => array('ontology_documentation_item_list_child_render'),
          ),
          'negative_inclusions' => array(
            '#theme' => 'item_list',
            '#items' => array(),
            '#title' => '',
            '#type' => 'ul',
            '#printed' => TRUE,
            '#pre_render' => array('ontology_documentation_item_list_child_render'),
          ),
          'denial_assertions' => array(
            '#theme' => 'item_list',
            '#items' => array(),
            '#title' => '',
            '#type' => 'ul',
            '#printed' => TRUE,
            '#pre_render' => array('ontology_documentation_item_list_child_render'),
           ),
          'functional_and_ID_contraints' => array(
            '#theme' => 'item_list',
            '#items' => array(),
            '#title' => '',
            '#type' => 'ul',
            '#printed' => TRUE,
            '#pre_render' => array('ontology_documentation_item_list_child_render'),
          ),
          'epistemic_constraints' => array(
            '#theme' => 'item_list',
            '#items' => array(),
            '#title' => '',
            '#type' => 'ul',
            '#printed' => TRUE,
            '#pre_render' => array('ontology_documentation_item_list_child_render'),
          ),
        ),
      ),
      '#attributes' => array(
        'class' => array(
          'collapsible',
          'collapsed',
        ),
      ),
      '#printed' => TRUE,
      '#pre_render' => array('ontology_documentation_fieldset_child_reder'),
    ),
  );

  public static function getConfigForm() {
    $config_form = array();

    $config_form['config_form_header'] = array(
      '#type' => 'html_tag',
      '#tag' => 'h2',
      '#options' => array('class' => 'onto_form_header'),
      '#value' => 'Ontology documentation upgrade form'
    );
    $config_form['config_form_delete_check'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete OWL documentation pages'),
      '#description' => t('All the documentation will drop off'),
      '#disabled' => variable_get('empty_owl_documentation', TRUE),
      '#default_value' => FALSE,
    );
    $config_form['config_form_upgrade_check'] = array(
      '#type' => 'checkbox',
      '#title' => t('Upgrade OWL documentation pages'),
      '#description' => t('Documentation pages will be upgraded'),
      '#default_value' => TRUE,
    );
    $config_form['config_form_submit'] = array(
      '#type' => 'submit',
      '#value' => 'Ok',
    );
    $config_form['config_form_submit_cancel'] = array(
      '#type' => 'submit',
      '#value' => 'Cancel',
      '#submit' => array('ontology_documentation_config_form_cancel'),
    );

    return $config_form;
  }

  public static function getEditTermForm() {
    // Dichiaro il vettore per il form
    $edit_form = array();

    $edit_form['entity_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Titolo'),
      '#default_value' => '',
      '#size' => 45,
      '#required' => TRUE,
    );
    $edit_form['entity_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Descrizione'),
      '#default_value' => '',
    );
    $edit_form['entity_status'] = array(
      '#type' => 'select',
      '#title' => t('Text Format'),
      '#default_value' => '',
      '#options' => array(
        0 => t('Plain text'),
        1 => t('Wiki text'),
        2 => t('Filtered html'),
        3 => t('Full html'),
       ),
    );
    $edit_form['entity_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $edit_form['entity_cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#submit' => array('ontology_documentation_edit_form_cancel'),
    );

    return $edit_form;
  }

  public static function getBundleEditForm() {
    // Dichiaro il vettore per il form
    $bundle_edit_form = array();

    // Definisco il campo per editare il machine_name del bundle
    $bundle_edit_form['bundle_type'] = array(
      '#type' => 'textfield',
      '#title' => t('Documentation bundle machine name'),
      '#description' => t('A unique machine-readable name of this documentation bundle.'),
      '#default_value' => '',
      '#size' => 55,
    );
    // Definisco il campo per editare il nome del bundle
    $bundle_edit_form['bundle_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Documentation bundle name'),
      '#description' => t('The human-readable name of this documentation bundle.'),
      '#size' => 55,
      '#default_value' => '',
      '#required' => TRUE,
    );
    // Definisco il campo per editare la descrizione del bundle
    $bundle_edit_form['bundle_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Documentation bundle description'),
      '#description' => t('A short description of this documentation bundle.'),
      '#default_value' => '',
      '#required' => TRUE,
    );
    $bundle_edit_form['bundle_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $bundle_edit_form['bundle_cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#submit callback' => array('ontology_documentation_edit_type_form_cancel'),
    );

    return $bundle_edit_form;
  }

  public static function getMastroMethodsStructure() {
    return OntologyDocumentationUtil::$mastroMethodsStructure;
  }

  public static function getGraphMLViewerPath() {
    return OntologyDocumentationUtil::$graphMLViewerPath;
  }

  public static function getMastroAxiomTypes() {
    return OntologyDocumentationUtil::$mastroAxiomTypes;
  }

  public static function getObdaTypes() {
    return OntologyDocumentationUtil::$obdaTypes;
  }

  public static function getFieldsDataStructure() {
    return OntologyDocumentationUtil::$fieldsDataStructure;
  }
}