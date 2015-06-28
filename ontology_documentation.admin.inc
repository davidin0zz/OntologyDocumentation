<?php

function ontology_documentation_type_form($form, &$form_state, $entity_type, $op) {
  if ($op == 'edit') {
    $form['category'] = array(
      '#type' => 'textfield',
      '#title' => t('Documentation type machine name'),
      '#description' => t('A unique machine-readable name of this documentation type.'),
      '#default_value' => isset($entity_type->category) ? $entity_type->category : '',
      '#size' => '50',
      '#disabled' => TRUE,
    );
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Documentation type name'),
      '#description' => t('The human-readable name of this documentation type.'),
      '#size' => '50',
      '#default_value' => isset($entity_type->title) ? $entity_type->title : '',
      '#required' => TRUE,
    );
    $form['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Documentation type description'),
      '#description' => t('A short description of this documentation type.'),
      '#default_value' => isset($entity_type->description) ? $entity_type->description : '',
      '#required' => TRUE,
    );
    $form['locked'] = array(
      '#type' => 'radios',
      '#title' => t('Is locked type?'),
      '#description' => t('Enable deletion of this documentation type.'),
      '#options' => array(
        t('no'),
        t('yes'),
      ),
      '#default_value' => isset($entity_type->locked) ? $entity_type->locked : 0,
      '#required' => TRUE,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 50,
    );
    $form['cancel'] = array(
      '#type' => 'button',
      '#value' => t('Cancel'),
      '#weight' => 50,
    );
 }

  return $form;
}

function ontology_documentation_form($form, &$form_state, $entity, $op) {
  // L'operazione richiamata da entity_ui_get_form è edit
  if ($op == 'edit') {
    // Definisco il vettore per il form
    $edit_form = array();
    // Definisco il vettore dei patterns
    $patterns = array();
    // Definisco il vettore dei replacements
    $replacements = array();
    // Pattern per l'etichetta del campo
    $patterns[0] = '/(<.+?>)(Descrizione)(<.+?>)\s*/';
    // Patter per il ritorno a capo
    $patterns[1] = '/\n/';
    // Elimino l'etichetta del campo
    $replacements[0] =  '';
    // Elimino ritorno a capo
    $replacements[1] =  '';

    // Creo il wrapper per accedere al valore corrente del campo descrizione
    $entity_wrapper = entity_metadata_wrapper(__documentation_type__, $entity);
    // Titolo
    $title = $entity_wrapper->name->value();
    // Estraggo il valore corrente
    $current_description = preg_replace($patterns, $replacements, $entity_wrapper->descrizione->value->raw());

    $edit_form['title'] = array(
      '#type' => 'textfield',
      '#title' => 'Titolo',
      '#default_value' => $title,
      '#size' => 45,
      '#required' => TRUE,
    );
    $edit_form['description'] = array(
      '#type' => 'textarea',
      '#title' => 'Descrizione',
      '#description' => t('Empty line in description will be deleted!'),
      '#default_value' => $current_description,
    );
    $edit_form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Save',
    );
    $edit_form['cancel'] = array(
      '#type' => 'button',
      '#value' => 'Cancel',
    );

    return $edit_form;
  }
}

function ontology_documentation_form_submit($form, &$form_state) {
  // Definisco il vettore dei patterns
  $patterns = array();
  // Definisco il vettore dei replacements
  $replacements = array();
  // Pattern per gli spazi bianchi iniziali e finali del testo immesso
  $patterns[0] = "/(^\s*)(.*?)(\s*$)/";
  // Sostituisco con il gruppo $2
  $replacements[0] = "$2";
  // Pattern per gli spazi tra i tag
  $patterns[1] = "/(<[^>]*>)(\s*)(\w*)(\s*)(<[^\/>]*\/.+?>)/";
  // Sostituisco con i gruppi $1,$3,$5
  $replacements[1] = "$1$3$5";

  // Definisco la struttura per memorizzare il valore del testo
  $data = array(
    'value' => array(
      'title' => array(
        '#type' => 'html_tag',
        '#tag' => 'h2',
        '#value' => 'Descrizione',
      ),
      'content' => array(
        '#markup' => '',
      ),
    ),
    'format' => 'wiki_text',
  );

  // Recupero l'entità passata al form
  $entity = $form_state[__documentation_type__];
  // Creo il wrapper
  $wrapper = entity_metadata_wrapper(__documentation_type__, $entity);
  // Recupero il titolo del termine
  $title = strip_tags($form_state['values']['title']);
  // Recupero il valore del testo immesso dall'utente eliminando le linee vuote
  $description = preg_replace($patterns, $replacements, $form_state['values']['description']);
  // Assegno il testo alla struttura
  $data['value']['content']['#markup'] = $description;
  // Renderizzo i tag
  $data['value'] = drupal_render($data['value']);
  // Setto il titolo
  $wrapper->name->set($title);
  // Setto data
  $wrapper->descrizione->set($data);
  // Salvo il contenuto
  $wrapper->save();
  // Rindirizzo l'utente alla pagina del termine
  $form_state['redirect'] = 'ontology-documentation/'.$wrapper->getBundle().'/'.str_replace('_', '-', $wrapper->name->value());
}

function ontology_documentation_type_form_submit($form, &$form_state) {
  $entity_type = entity_ui_form_submit_build_entity($form, $form_state);
  $entity_type->save();
  drupal_set_message(t('Ontology documentation type: @title has been saved.',
    array('@title' => $entity_type->title)
  ));
  $form_state['redirect'] = 'admin/structure/ontology-documentation-types';
}
