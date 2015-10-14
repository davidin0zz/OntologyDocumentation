<?php

function ontology_documentation_type_form($form, &$form_state, $entity, $op)
{

  // L'operazione richiamata da entity_ui_get_form Ã¨ edit

  if ($op === 'edit')
  {
    $fields = array(
      'category',
      'name',
      'description',
    );

    // Creo il wrapper da passare al frontend

    $form_wrapper = ontology_documentation_wrapper(__documentation_type_bundle__, $entity);

    // Recupero il template del form

    $bundle_edit_form = ontology_documentation_util('bundle_edit_form');

    // Assegno i valori ai campi

    foreach($fields as $field)
    {
      $description = trim($form_wrapper->getFieldRawContent($field));
      $description = html_entity_decode($description, ENT_QUOTES | ENT_XHTML, 'UTF-8');
      $bundle_edit_form['fieldset'][$field]['#default_value'] = $description;
    }

    // Ritorno il form completo

    return $bundle_edit_form;
  }
}

function ontology_documentation_form($form, &$form_state, $entity, $op)
{

  // L'operazione richiamata da entity_ui_get_form Ã¨ edit

  if ($op === 'edit')
  {
    $fields = array(
      'name',
      'descrizione',
      'state',
    );

    // Creo il wrapper

    $edit_form_wrapper = ontology_documentation_wrapper(__documentation_type__, $entity);

    // Recupero il template del form

    $edit_form = ontology_documentation_util('entity_edit_form');
    foreach($fields as $field)
    {
      if ($field === 'descrizione')
      {
        $description = trim(str_replace('<h2>Descrizione</h2>', '', $edit_form_wrapper->getFieldRawContent($field)));
        $description = html_entity_decode($description, ENT_QUOTES | ENT_XHTML, 'UTF-8');
        $edit_form['fieldset'][$field]['#default_value'] = $description;
      }
      else
      {
        $edit_form['fieldset'][$field]['#default_value'] = $edit_form_wrapper->getFieldRawContent($field);
      }
    }

    // Ritorno il form completo

    return $edit_form;
  }
}

function ontology_documentation_edit_form_validate($form, &$form_state)
{
  $changed_fields = array();
  $current_values = $form_state['values'];
  foreach($current_values as $field_name => $field)
  {
    if (isset($form['fieldset'][$field_name]))
    {
      if ($field !== $form['fieldset'][$field_name]['#default_value'])
      {
        switch ($field_name)
        {
        case 'name':
          $changed_fields+= array(
            $field_name => array(
              'value' => htmlentities(trim($field), ENT_QUOTES | ENT_XHTML, 'UTF-8'),
            )
          );
          break;

        case 'category':
          $changed_fields+= array(
            $field_name => array(
              'value' => htmlentities(trim($field), ENT_QUOTES | ENT_XHTML, 'UTF-8'),
            )
          );
          break;

        case 'description':
          $changed_fields+= array(
            $field_name => array(
              'value' => htmlentities(trim($field), ENT_QUOTES | ENT_XHTML, 'UTF-8'),
            )
          );
          break;

        case 'descrizione':
          $changed_fields+= array(
            $field_name => array(
              'value' => '<h2>Descrizione</h2>' . "\r\n" .
                htmlentities(trim($field), ENT_QUOTES | ENT_XHTML, 'UTF-8'),
            )
          );
          break;

        case 'state':
          $changed_fields+= array(
            $field_name => array(
              'value' => $field
            )
          );
          break;
        }
      }
    }
  }

  if (!empty($changed_fields))
  {
    $form_state['modified fields'] = $changed_fields;
  }
  else
  {
    form_set_error('name', 'Title is the same as current title!');
    if (isset($form['fieldset']['description']))
    {
      form_set_error('description', 'Description is the same as current description!');
    }

    if (isset($form['fieldset']['descrizione']))
    {
      form_set_error('descrizione', 'Description is the same as current description!');
    }

    if (isset($form['fieldset']['state']))
    {
      form_set_error('state', 'State is the same as current state!');
    }
  }
}

function ontology_documentation_type_form_submit($form, &$form_state)
{
  $fields = $form_state['modified fields'];

  // Recupero il bundle passato al form

  $bundle = $form_state[__documentation_type_bundle__];

  // Creo il wrapper

  $form_wrapper = ontology_documentation_wrapper(__documentation_type_bundle__, $bundle);

  // Setto il valore dei campi cambiati

  $form_wrapper->setFieldsContent($fields);

  // Salvo le modifiche

  $form_wrapper->save();

  // Definisco il messaggio d''uscita

  $form_state['redirect'] = $bundle->uri();
}

function ontology_documentation_form_submit($form, &$form_state)
{
  global $user;
  if ($form_state['op'] === 'edit')
  {
    $fields = $form_state['modified fields'];

    // Aggiungo il campo relativo all'utente modificante e la data di modifica

    $fields+= array(
      'uid' => array(
        'value' => $user->uid
      ) ,
      'modification_date' => array(
        'value' => REQUEST_TIME
      ) ,
    );

    // Recupero l'entitÃ  passata al form

    $entity = $form_state[__documentation_type__];

    // Creo il wrapper

    $form_submit_wrapper = ontology_documentation_wrapper(__documentation_type__, $entity);

    // Assegno i valori all'entitÃ

    $form_submit_wrapper->setFieldsContent($fields);

    // Salvo il contenuto

    $form_submit_wrapper->save();

    // Rindirizzo l'utente alla pagina del termine

    $form_state['redirect'] = $entity->uri();
  }
}