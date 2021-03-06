<?php
class OntologyDocumentationUtil
{
  protected static $graphMLViewerPath = 'graphml-viewer';
  protected static $obdaTypes = array(
    'concept' => 0,
    'role' => 1,
    'attribute' => 2,
  );

  protected static $fieldsType = array(
    'categoria' => 'full_html',
    'descrizione' => 'wiki_text',
    'occorrenza' => 'full_html',
    'generalizzazioni' => 'full_html',
    'specializzazioni' => 'full_html',
    'tipizzazione' => 'full_html',
    'ruoli_ril_non_obbligatori' => 'full_html',
    'ruoli_ril_obbligatori' => 'full_html',
    'attributi_ril_non_obbligatori' => 'full_html',
    'attributi_ril_obbligatori' => 'full_html',
    'assiomi' => 'full_html',
    'link_obda' => 'full_html',
    'mappings' => 'full_html',
  );
  protected static $mastroAxiomTypes = array(
    'positive_inclusions' => array(
      0
    ) ,
    'negative_inclusions' => array(
      1
    ) ,
    'functional_and_ID_constraints' => array(
      2,
      3
    ) ,
    'denial_assertions' => array(
      4
    ) ,
    'epistemic_constraints' => array(
      5
    ) ,
  );
  protected static $mastroMethodsStructure = array(
    'occorrenza' => array(
      'concept' => array(
        'keys' => array(
          'concept'
        ) ,
        'operation' => 'getConceptGraphMLLinks',
      ) ,
      'role' => array(
        'keys' => array(
          'role'
        ) ,
        'operation' => 'getRoleGraphMLLinks',
      ) ,
      'attribute' => array(
        'keys' => array(
          'attribute'
        ) ,
        'operation' => 'getAttributeGraphMLLinks',
      ) ,
    ) ,
    'descrizione' => array(
      'role' => array(
        'keys' => array(
          'roleName'
        ) ,
        'operation' => 'isFunctionalRole',
      ) ,
      'attribute' => array(
        'keys' => array(
          'attributeName'
        ) ,
        'operation' => 'isFunctionalAttribute',
      ) ,
    ) ,
    'descrizione_inverse' => array(
      'role' => array(
        'keys' => array(
          'roleName'
        ) ,
        'operation' => 'isInverseFunctionalRole',
      ) ,
    ) ,
    'specializzazioni' => array(
      'concept' => array(
        'keys' => array(
          'conceptName',
          'renderingMode'
        ) ,
        'operation' => 'getAssertedSubAtomicConcepts',
      ) ,
      'role' => array(
        'keys' => array(
          'roleName',
          'renderingMode'
        ) ,
        'operation' => 'getAssertedSubAtomicRoles',
      ) ,
      'attribute' => array(
        'keys' => array(
          'attributeName',
          'renderingMode'
        ) ,
        'operation' => 'getAssertedSubAtomicConceptAttributes',
      ) ,
    ) ,
    'generalizzazioni' => array(
      'concept' => array(
        'keys' => array(
          'conceptName',
          'renderingMode'
        ) ,
        'operation' => 'getAssertedSuperAtomicConcepts',
      ) ,
      'role' => array(
        'keys' => array(
          'roleName',
          'renderingMode'
        ) ,
        'operation' => 'getAssertedSuperAtomicRoles',
      ) ,
      'attribute' => array(
        'keys' => array(
          'attributeName',
          'renderingMode'
        ) ,
        'operation' => 'getAssertedSuperAtomicConceptAttributes',
      ) ,
    ) ,
    'tipizzazione_domain' => array(
      'role' => array(
        'keys' => array(
          'roleName'
        ) ,
        'operation' => 'getRoleDomain',
      ) ,
      'attribute' => array(
        'keys' => array(
          'attributeName'
        ) ,
        'operation' => 'getAttributeDomain',
      ) ,
    ) ,
    'tipizzazione_range' => array(
      'role' => array(
        'keys' => array(
          'roleName'
        ) ,
        'operation' => 'getRoleRange',
      ) ,
      'attribute' => array(
        'keys' => array(
          'attributeName'
        ) ,
        'operation' => 'getAttributeRange',
      ) ,
    ) ,
    'attributi_ril_obbligatori' => array(
      'concept' => array(
        'keys' => array(
          'conceptName'
        ) ,
        'operation' => 'getRelevantMandatoryAttributes',
      ) ,
    ) ,
    'attributi_ril_non_obbligatori' => array(
      'concept' => array(
        'keys' => array(
          'conceptName'
        ) ,
        'operation' => 'getRelevantOptionalAttributes',
      ) ,
    ) ,
    'ruoli_ril_obbligatori' => array(
      'concept' => array(
        'keys' => array(
          'conceptName'
        ) ,
        'operation' => 'getRelevantMandatoryRoles',
      ) ,
    ) ,
    'ruoli_ril_non_obbligatori' => array(
      'concept' => array(
        'keys' => array(
          'conceptName'
        ) ,
        'operation' => 'getRelevantOptionalRoles',
      ) ,
    ) ,
    'assiomi' => array(
      'concept' => array(
        'keys' => array(
          'conceptName',
          'axiomType',
          'format'
        ) ,
        'operation' => 'getMastroAxiomsByConcept',
      ) ,
      'role' => array(
        'keys' => array(
          'roleName',
          'axiomType',
          'format'
        ) ,
        'operation' => 'getMastroAxiomsByRole',
      ) ,
      'attribute' => array(
        'keys' => array(
          'attributeName',
          'axiomType',
          'format'
        ) ,
        'operation' => 'getMastroAxiomsByAttribute',
      ) ,
    ) ,
  );
  protected static $fieldsDataStructure = array(
    'label' => array(
      '#theme' => 'html_tag',
      '#tag' => 'h2',
      '#value' => '',
      '#printed' => true,
    ) ,
    'page_link' => array(
      '#type' => 'link',
      '#title' => '',
      '#href' => '',
      '#printed' => true,
    ) ,
    'item_list' => array(
      '#theme' => 'item_list',
      '#items' => array() ,
      '#type' => 'ul',
      '#pre_render' => array(
        'ontology_documentation_item_list_child_render'
      ) ,
      '#printed' => true,
    ) ,
    'description' => array(
      '#markup' => '',

      // '#tag' => 'p',
      // '#value' => '',

      '#printed' => true,
    ) ,
    'fieldset_box' => array(
      '#theme' => 'fieldset',
      '#title' => '',
      '#children' => array(
        'simple' => array(
          '#theme' => 'item_list',
          '#items' => array() ,
          '#type' => 'ul',
          '#printed' => true,
          '#pre_render' => array(
            'ontology_documentation_item_list_child_render'
          ) ,
        ) ,
        'axioms' => array(
          'positive_inclusions' => array(
            '#theme' => 'item_list',
            '#items' => array() ,
            '#title' => 'Positive Inclusions',
            '#type' => 'ul',
            '#printed' => true,
            '#pre_render' => array(
              'ontology_documentation_item_list_child_render'
            ) ,
          ) ,
          'negative_inclusions' => array(
            '#theme' => 'item_list',
            '#items' => array() ,
            '#title' => 'Negative Inclusions',
            '#type' => 'ul',
            '#printed' => true,
            '#pre_render' => array(
              'ontology_documentation_item_list_child_render'
            ) ,
          ) ,
          'denial_assertions' => array(
            '#theme' => 'item_list',
            '#items' => array() ,
            '#title' => 'Denial Assertions',
            '#type' => 'ul',
            '#printed' => true,
            '#pre_render' => array(
              'ontology_documentation_item_list_child_render'
            ) ,
          ) ,
          'functional_and_ID_constraints' => array(
            '#theme' => 'item_list',
            '#items' => array() ,
            '#title' => 'Functional and ID Constraints',
            '#type' => 'ul',
            '#printed' => true,
            '#pre_render' => array(
              'ontology_documentation_item_list_child_render'
            ) ,
          ) ,
          'epistemic_constraints' => array(
            '#theme' => 'item_list',
            '#items' => array() ,
            '#title' => 'Epistemic Constraints',
            '#type' => 'ul',
            '#printed' => true,
            '#pre_render' => array(
              'ontology_documentation_item_list_child_render'
            ) ,
          ) ,
        ) ,
      ) ,
      '#attributes' => array(
        'class' => array(
          'collapsible',
          'collapsed',
        ) ,
      ) ,
      '#printed' => true,
      '#pre_render' => array(
        'ontology_documentation_fieldset_child_reder'
      ) ,
    ) ,
  );
  public static function getMastroUid()
  {
    return get_mastro_uid();
  }

  public static function getBundles($condition = 'ALL')
  {
    $bundles = & drupal_static(__FUNCTION__, array());
    if (empty($bundles))
    {
      $bundles+= array(
        'concept' => array(
          'machine_name' => 'concept',
          'human_label' => 'Concept',
          'description' => 'Describe a specific owl concept',
        ) ,
        'role' => array(
          'machine_name' => 'role',
          'human_label' => 'Role',
          'description' => 'Describe a specific owl role',
        ) ,
        'attribute' => array(
          'machine_name' => 'attribute',
          'human_label' => 'Attribute',
          'description' => 'Describe a sepcific owl attribute',
        ) ,
      );
    }

    if ($condition == 'ALL')
    {
      return array_slice($bundles, 0, 3);
    }
    elseif ($condition == 'WITH_DOMAIN_AND_CODOMAIN')
    {
      return array_slice($bundles, 1, 2);
    }
    else
    {
      return array_slice($bundles, 0, 1);
    }
  }

  public static function getConfigForm()
  {
    $config_form = array();
    $config_form['header'] = array(
      '#type' => 'html_tag',
      '#tag' => 'h2',
      '#options' => array(
        'class' => 'onto_form_header'
      ) ,
      '#value' => 'Ontology documentation upgrade form'
    );
    $config_form['delete_check'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete OWL documentation pages') ,
      '#description' => t('All the documentation will drop off') ,
      '#disabled' => variable_get('empty_owl_documentation', true) ,
      '#default_value' => false,
    );
    $config_form['delete_old'] = array(
      '#type' => 'checkbox',
      '#title' => t('Syncronize OWL documentation pages') ,
      '#description' => t('Old documentation will drop off') ,
      '#disabled' => variable_get('syncronized_owl_documentation', true) ,
      '#default_value' => false,
    );
    $config_form['upgrade_check'] = array(
      '#type' => 'checkbox',
      '#title' => t('Update OWL documentation pages') ,
      '#description' => t('Documentation pages will be upgraded') ,
      '#default_value' => true,
    );
    $config_form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Ok',
    );
    $config_form['submit_cancel'] = array(
      '#type' => 'submit',
      '#value' => 'Cancel',
      '#submit' => array(
        'ontology_documentation_config_form_cancel'
      ) ,
    );
    return $config_form;
  }

  public static function getEntityEditForm()
  {

    // Dichiaro il vettore per il form

    $edit_form = array();
    $edit_form['fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Edit entity') ,
    );
    $edit_form['fieldset']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Title') ,
      '#default_value' => '',
      '#disabled' => true,
      '#size' => 45,
    );
    $edit_form['fieldset']['descrizione'] = array(
      '#type' => 'textarea',
      '#title' => t('Description') ,
      '#rows' => 12,
      '#default_value' => '',
    );
    $edit_form['fieldset']['state'] = array(
      '#type' => 'select',
      '#title' => t('Page status:') ,
      '#attributes' => array(
        'class' => array(
          'entity-state'
        ) ,
      ) ,
      '#default_value' => '',
      '#options' => array(
        0 => t('Unpublished') ,
        1 => t('Published') ,
      ) ,
    );
    $edit_form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save') ,
      '#validate' => array(
        'ontology_documentation_edit_form_validate'
      ) ,
    );

    // Appendo il foglio di stile

    $edit_form['fieldset']['#attached']['css'] = array(
      __module_path__ . '/css/entity_edit_form.css',
    );
    return $edit_form;
  }

  public static function getEntityRevisionForm()
  {

    // Dichiaro il vettore per il from

    $entity_revision_form = array();
    $entity_revision_form['fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => 'Create new revision',
    );
    $entity_revision_form['fieldset']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Title') ,
      '#description' => t('Entity title for the new revision.') ,
      '#default_value' => '',
      '#size' => 35,
      '#required' => true,
    );
    $entity_revision_form['fieldset']['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Description') ,
      '#description' => t('Entity description for the new revision.') ,
      '#rows' => 12,
    );
    $entity_revision_form['fieldset']['comment'] = array(
      '#type' => 'textarea',
      '#title' => t('Revision comment') ,
      '#description' => t('A comment for the new revision.') ,
      '#default_value' => 'A fantastic new revision',
      '#rows' => 5,
      '#attributes' => array(
        'class' => array(
          'entity-revision-comment'
        )
      ) ,
      '#required' => true,
    );

    // Appendo il foglio di stile

    $entity_revision_form['fieldset']['#attached']['css'] = array(
      __module_path__ . '/css/entity_revision_form.css',
    );
    $entity_revision_form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create') ,
      '#validate' => array(
        'ontology_documentation_edit_form_validate'
      ) ,
    );
    return $entity_revision_form;
  }

  public static function getBundleEditForm()
  {

    // Dichiaro il vettore per il form

    $bundle_edit_form = array();
    $bundle_edit_form['fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Edit bundle') ,
    );

    // Definisco il campo per editare il machine_name del bundle

    $bundle_edit_form['fieldset']['category'] = array(
      '#type' => 'textfield',
      '#title' => t('Documentation bundle machine name') ,
      '#description' => t('A unique machine-readable name of this documentation bundle.') ,
      '#default_value' => '',
      '#size' => 35,
      '#disabled' => true,
    );

    // Definisco il campo per editare il nome del bundle

    $bundle_edit_form['fieldset']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Documentation bundle name') ,
      '#description' => t('The human-readable name of this documentation bundle.') ,
      '#size' => 35,
      '#default_value' => '',
      '#required' => true,
    );

    // Definisco il campo per editare la descrizione del bundle

    $bundle_edit_form['fieldset']['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Documentation bundle description') ,
      '#description' => t('A short description of this documentation bundle.') ,
      '#rows' => 4,
      '#default_value' => '',
      '#required' => true,
    );
    $bundle_edit_form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save') ,
      '#validate' => array(
        'ontology_documentation_edit_form_validate'
      ) ,
    );
    return $bundle_edit_form;
  }

  public static function replaceWikiLink($subject)
  {
    $matches = array();
    $mediaWikiRegex = '/\[\[([a-zA-Z0-9_]+)\]\]/';

    preg_match_all($mediaWikiRegex, $subject, $matches, PREG_SET_ORDER);

    foreach ($matches as $match)
    {
      $subject = str_replace(current($match), l($match[1], __documentation_view_path__ . '/' . str_replace('_', '-', $match[1])), $subject);
    }

    return $subject;
  }

  public static function getFieldsType()
  {
    return OntologyDocumentationUtil::$fieldsType;
  }

  public static function getMediaWikiRegex()
  {
    return OntologyDocumentationUtil::$mediaWikiRegex;
  }
  public static function getMediaWikiSubstr()
  {
    return OntologyDocumentationUtil::$mediaWikiSubstr;
  }
  public static function getMastroMethodsStructure()
  {
    return OntologyDocumentationUtil::$mastroMethodsStructure;
  }

  public static function getGraphMLViewerPath()
  {
    return OntologyDocumentationUtil::$graphMLViewerPath;
  }

  public static function getMastroAxiomTypes()
  {
    return OntologyDocumentationUtil::$mastroAxiomTypes;
  }

  public static function getObdaTypes()
  {
    return OntologyDocumentationUtil::$obdaTypes;
  }

  public static function getFieldsDataStructure()
  {
    return OntologyDocumentationUtil::$fieldsDataStructure;
  }
}
