<?php

function ontology_documentation_type_form($form, &$form_state, $entity, $op) {
  // L'operazione richiamata da entity_ui_get_form è edit
  if ($op == 'edit') {
    // Creo il wrapper da passare al frontend
    $form_wrapper = entity_metadata_wrapper(__documentation_type_type__, $entity);
    // Recupero il template del form
    $bundle_edit_form = ontology_documentation_util('bundle_edit_form');
    // Recupero il tipo del bundle
    $bundle_type = $form_wrapper->type->value();
    // Recupero il nome del bundle
    $bundle_name = $form_wrapper->name->value();
    // Recupero la descrizione del bundle
    $bundle_description = $form_wrapper->description->value();
    // Imposto il tipo del bundle
    $bundle_edit_form['bundle_type']['#default_value'] = $bundle_type;
    // Imposto il nome del bundle
    $bundle_edit_form['bundle_name']['#default_value'] = $bundle_name;
    // Imposto la descrizione del bundle
    $bundle_edit_form['bundle_description']['#default_value'] = $bundle_description;
    // Ritorno il form completo
    return $bundle_edit_form;
  }
}

function ontology_documentation_edit_type_form_cancel($form, &$form_state) {
  drupal_goto(__documentation_type_base_url__);
}

function ontology_documentation_form($form, &$form_state, $entity, $op) {
  // L'operazione richiamata da entity_ui_get_form è edit
  if ($op == 'edit') {
    // Recupero il template del form
    $edit_form = ontology_documentation_util('entity_edit_form');
    // Creo il wrapper da passare al frontend
    $edit_form_wrapper = entity_metadata_wrapper(__documentation_type__, $entity);
    // Recupero il titolo
    $entity_name = $edit_form_wrapper->name->value();
    // Recupero la descrizione
    $entity_description = str_replace('<h2>Descrizione</h2>', '', $edit_form_wrapper->descrizione->value->raw());
    // Recupero lo stato
    $state = $edit_form_wrapper->state->value();
    // Imposto il titolo
    $edit_form['entity_name']['#default_value'] = $entity_name;
    // Imposto la descriione
    $edit_form['entity_description']['#default_value'] = trim($entity_description);
    // Imposto lo stato
    $edit_form['entity_state']['#default_value'] = $state;
    // Ritorno il form completo
    return $edit_form;
  }
}

function ontology_documentation_edit_form_cancel($form, &$form_state) {
  // Recupero l'entità
  $edit_entity = $form_state[__documentation_type__];
  // Creo il wrapper
  $edit_submit_wrapper = entity_metadata_wrapper(__documentation_type__, $edit_entity);
  // Recuper il bundle
  $bundle = $edit_submit_wrapper->type->value();
  // Recupero il nome del termine
  $term_name = str_replace('_', '-', $edit_submit_wrapper->name->value());
  // Ritorno alla pagina di visualizzazione termine
  drupal_goto(__documentation_base_url__.'/'.$bundle.'/'.$term_name);
}

function ontology_documentation_type_form_submit($form, &$form_state) {
  $entity_type = entity_ui_form_submit_build_entity($form, $form_state);
  $entity_type->save();
  drupal_set_message(t('Ontology documentation type: @title has been saved.',
    array('@title' => $entity_type->title)
  ));
  $form_state['redirect'] = 'admin/structure/ontology-documentation-types';
}

function ontology_documentation_form_submit($form, &$form_state) {
  // Recupero l'entità passata al form
  $entity = $form_state[__documentation_type__];
  // Creo il wrapper
  $form_submit_wrapper = entity_metadata_wrapper(__documentation_type__, $entity);
  // Recupero il titolo del termine
  $title = strip_tags($form_state['values']['entity_name']);
  // Recupero il valore del testo e ci appendo l'eticchetta del campo
  $description = '<h2>Descrizione</h2>'.trim($form_state['values']['entity_description']);
  // Recupero lo stato
  $state = $form_state['values']['entity_state'];
  // Setto il titolo
  $form_submit_wrapper->name->set($title);
  // Setto data
  $form_submit_wrapper->descrizione->value->set($description);
  // Setto lo stato
  $form_submit_wrapper->state->set($state);
  // Salvo il contenuto
  $form_submit_wrapper->save();
  // Rindirizzo l'utente alla pagina del termine
  $form_state['redirect'] = 'ontology-documentation/'.$form_submit_wrapper->getBundle().'/'.str_replace('_', '-', $form_submit_wrapper->name->value());
}

