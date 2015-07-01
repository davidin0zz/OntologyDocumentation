<?php

function ontology_documentation_type_form($form, &$form_state, $entity, $op) {
  // L'operazione richiamata da entity_ui_get_form è edit
  if ($op == 'edit') {
    // Creo il wrapper da passare al frontend
    $form_wrapper = entity_metadata_wrapper(__documentation_type_bundle__, $entity);
    // Recupero il template del form
    $bundle_edit_form = ontology_documentation_util('bundle_edit_form');
    // Imposto il tipo del bundle
    $bundle_edit_form['fieldset']['bundle_type']['#default_value'] = $form_wrapper->type->value();
    // Imposto il nome del bundle
    $bundle_edit_form['fieldset']['bundle_name']['#default_value'] = $form_wrapper->name->value();;
    // Imposto la descrizione del bundle
    $bundle_edit_form['fieldset']['bundle_description']['#default_value'] = $form_wrapper->description->value();
    // Ritorno il form completo
    return $bundle_edit_form;
  }
}

function ontology_documentation_form($form, &$form_state, $entity, $op) {
  // L'operazione richiamata da entity_ui_get_form è edit
  if ($op == 'edit') {
    // Creo il wrapper
    $edit_form_wrapper = entity_metadata_wrapper(__documentation_type__, $entity);
    // Recupero il template del form
    $edit_form = ontology_documentation_util('entity_edit_form');
    // Recupero la descrizione
    $entity_description = str_replace('<h2>Descrizione</h2>', '', $edit_form_wrapper->descrizione->value->raw());
    // Imposto il titolo
    $edit_form['fieldset']['entity_name']['#default_value'] = $edit_form_wrapper->name->value();
    // Imposto la descriione
    $edit_form['fieldset']['entity_description']['#default_value'] = trim($entity_description);
    // Imposto lo stato
    $edit_form['fieldset']['entity_state']['#default_value'] = $edit_form_wrapper->state->value();
    // Ritorno il form completo
    return $edit_form;
  }
  // Form per la creazione della revisione
  if ($op == 'create') {
    // Creo il weapper
    $entity_revision_wrapper = entity_metadata_wrapper(__documentation_type__, $entity);
    // Recupero il template del form
    $entity_revision_form = ontology_documentation_util('entity_revision_form');
    // Imposto il nome dell'entità
    $entity_revision_form['fieldset']['entity_revision_name']['#default_value'] = $entity_revision_wrapper->name->value();
    // Imposto la desrizione dell'entità
    $entity_revision_form['fieldset']['entity_revision_description']['#default_value'] = str_replace('<h2>Descrizione</h2>', '', $entity_revision_wrapper->descrizione->value->raw());

    return $entity_revision_form;
  }
}

function ontology_documentation_type_form_submit($form, &$form_state) {
  // Recupero il bundle passato al form
  $bundle = $form_state[__documentation_type_bundle__];
  // Creo il wrapper
  $form_wrapper = entity_metadata_wrapper(__documentation_type_bundle__, $bundle);
  // Imposto il nome del bundle
  $form_wrapper->name->set(strip_tags($form_state['values']['bundle_name']));
  // Imposto la descrizione del bundle
  $form_wrapper->description->set(strip_tags($form_state['values']['bundle_description']));
  // Salvo le modifiche
  $form_wrapper->save();
  // Definisco il messaggio d''uscita
  $form_state['redirect'] = __documentation_bundle_admin_path__;
}

function ontology_documentation_form_submit($form, &$form_state) {
  global $user;
  dpm($form_state);
  // Recupero l'entità passata al form
  $entity = $form_state[__documentation_type__];
  // Creo il wrapper
  $form_submit_wrapper = entity_metadata_wrapper(__documentation_type__, $entity);
  // Recupero il nome del bundle dell'entità chiamante
  $entity_bundle_name = $form_submit_wrapper->type->value();
  // Recupero il titolo dell'entità
  $entity_title = strip_tags($form_state['values']['entity_name']);
  // Recupero il valore del testo e ci appendo l'eticchetta del campo
  $description = '<h2>Descrizione</h2>'.trim($form_state['values']['entity_description']);
  // Setto il nuovo autore della modifica
  $form_submit_wrapper->uid->set($user->uid);
  //  Setto la nuova data di modifica
  $form_submit_wrapper->modification_date->set(REQUEST_TIME);
  // Setto il titolo
  $form_submit_wrapper->name->set(strip_tags($form_state['values']['entity_name']));
  // Setto data
  $form_submit_wrapper->descrizione->value->set($description);
  // Setto lo stato
  $form_submit_wrapper->state->set($form_state['values']['entity_state']);
  // Salvo il contenuto
  $form_submit_wrapper->save();
  // Rindirizzo l'utente alla pagina del termine
  $form_state['redirect'] = __documentation_view_path__.'/'.$entity_bundle_name.'/'.str_replace('_', '-', $entity_title);
}

