<?php

function ontology_documentation_type_form($form, &$form_state, $entity, $op) {
  // L'operazione richiamata da entity_ui_get_form è edit
  if ($op == 'edit') {
    // Creo il wrapper da passare al frontend
    $form_wrapper = entity_metadata_wrapper(__documentation_type_bundle__, $entity);
    // Recupero il template del form
    $bundle_edit_form = ontology_documentation_util('bundle_edit_form');
    // Imposto il tipo del bundle
    $bundle_edit_form['fieldset']['type']['#default_value'] = $form_wrapper->type->value();
    // Imposto il nome del bundle
    $bundle_edit_form['fieldset']['name']['#default_value'] = $form_wrapper->name->value();
    // Imposto la descrizione del bundle
    $bundle_edit_form['fieldset']['description']['#default_value'] = $form_wrapper->description->value();
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
    $edit_form['fieldset']['name']['#default_value'] = $edit_form_wrapper->name->value();
    // Imposto la descriione
    $edit_form['fieldset']['description']['#default_value'] = trim($entity_description);
    // Imposto lo stato
    $edit_form['fieldset']['state']['#default_value'] = $edit_form_wrapper->state->value();
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

function ontology_documentation_edit_form_validate($form, &$form_state) {
  $changed_field = array();
  $current_values = $form_state['values'];

  foreach ($current_values as $field_name => $field) {
    if (isset($form['fieldset'][$field_name])) {
      if ($field != $form['fieldset'][$field_name]['#default_value']) {
        switch ($field_name) {
          case 'name':
            // Ripulisco il nome da possibili tag
            $form_state['values'][$field_name] = strip_tags($form_state['values'][$field_name]);
          case 'description':
            // Ripulisco nome e descrizione da spazi e altri caratteri iniziali e finali
            $form_state['values'][$field_name] = trim($form_state['values'][$field_name]);
            break;
        }
        $changed_field += array($field_name => $field_name);
      }
    }
  }

  if (!empty($changed_field)) {
    $form_state['modified fields'] = $changed_field;
  }
  else {
    form_set_error('name', 'Title is the same as current title!');
    form_set_error('description', 'Description is the same as current description!');
    if (isset($form['fieldset']['state'])) {
      form_set_error('state', 'State is the same as current state!');
    }
  }
}

function ontology_documentation_type_form_submit($form, &$form_state) {
  // Recupero il bundle passato al form
  $bundle = $form_state[__documentation_type_bundle__];
  // Creo il wrapper
  $form_wrapper = entity_metadata_wrapper(__documentation_type_bundle__, $bundle);

  // Setto il valore dei campi cambiati
  foreach ($form_state['modified fields'] as $field) {
    $form_wrapper->{$field}->set($form_state['values'][$field]);
  }

  // Salvo le modifiche
  $form_wrapper->save();
  // Definisco il messaggio d''uscita
  $form_state['redirect'] = $bundle->uri();
}

function ontology_documentation_form_submit($form, &$form_state) {
  global $user;

  if ($form_state['op'] == 'edit') {
    // Recupero l'entità passata al form
    $entity = $form_state[__documentation_type__];
    // Creo il wrapper
    $form_submit_wrapper = entity_metadata_wrapper(__documentation_type__, $entity);

    //Setto il valore dei cammpi cambiati
    foreach ($form_state['modified fields'] as $field) {
     if ($field == 'description') {
       $form_submit_wrapper->descrizione->value->set('<h2>Descrizione</h2>'.$form_state['values'][$field]);
     }
     else {
       $form_submit_wrapper->{$field}->set($form_state['values'][$field]);
     }
    }

    // Setto il nuovo autore della modifica
    $form_submit_wrapper->uid->set($user->uid);
    //  Setto la nuova data di modifica
    $form_submit_wrapper->modification_date->set(REQUEST_TIME);
    // Salvo il contenuto
    $form_submit_wrapper->save();
    // Rindirizzo l'utente alla pagina del termine
    $form_state['redirect'] = $entity->uri();
  }

}

