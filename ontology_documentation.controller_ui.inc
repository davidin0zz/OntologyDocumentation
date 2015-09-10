<?php
class OntologyDocumentationUIController extends EntityDefaultUIController
{
  public function hook_menu()
  {
    $items = parent::hook_menu();

    // $this->path = admin/ontology-documentation

    $items[$this->path]['access callback'] = 'entity_access';

    // Previene la visualizzare del pannello di amministrazione dell'entitÃ
    // all'utente autenticato con il solo permesso di 'view'

    $items[$this->path]['access arguments'] = array(
      'update',
      $this->entityType
    );
    $items[$this->path]['weight'] = - 10;

    // Disabilito gli hook_menu non necessari

    $menu_hooks = array_keys($items);
    foreach($menu_hooks as $menu_hook)
    {
      $url_array = explode('/', $menu_hook);
      if (in_array('list', $url_array) || in_array('clone', $url_array) || in_array('add', $url_array))
      {
        unset($items[$menu_hook]);
      }
    }

    // Ritorno i nuovi uri di menu

    return $items;
  }

  public function overviewTableHeaders($conditions, $rows, $additional_header = array())
  {

    // Aggiungo l'header per il nome

    $additional_header[] = array(
      'data' => 'NAME'
    );

    // Aggiunto l'header per il tipo

    $additional_header[] = array(
      'data' => 'TYPE'
    );

    // Aggiungo l'header per l'autore

    $additional_header[] = array(
      'data' => 'AUTHOR'
    );

    // Aggiungo l'header per la data di ultimo aggiornamento

    $additional_header[] = array(
      'data' => 'UPDATED'
    );

    // Aggiungo l'header per lo stato

    $additional_header[] = array(
      'data' => 'STATUS'
    );

    // Aggiungo l'header per le operazioni

    $additional_header[] = array(
      'data' => 'OPERATIONS',
      'colspan' => 2
    );

    // Ritorno l'header

    return $additional_header;
  }

  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array())
  {
    $row = array();
    $options = array();

    // Recupero il nome dell'entitÃ

    $entity_name = $entity->title;

    // Recupero l'uri dell'entitÃ

    $entity_uri = $entity->uri();

    // Recupero il tipo dell'entitÃ

    $entity_type = $entity->category;

    // Recupero l'user id dell'entitÃ

    $entity_user_owner = user_load($entity->uid)->name;

    // Creo la data di ultima modifica formattata dal timestamp

    $entity_updated_data = format_date($entity->changed, 'short');

    // Recupero lo stato

    if ($entity->old)
    {
      $entity_status = 'OLD';
    }
    else
    {
      $entity_status = $entity->status ? 'PUBLISHED' : 'UNPUBLISHED';
    }

    // Aggiungo il nome dell'entitÃ

    $row[] = array(
      'data' => array(
        '#type' => 'link',
        '#title' => $entity_name,
        '#href' => current($entity_uri) ,
      ) ,
      'class' => array(
        strtolower($entity_status)
      ) ,
    );

    // Aggiungo il tipo dell'entitÃ

    $row[] = ucwords($entity_type);

    // Aggiungo l'autore

    $row[] = $entity_user_owner;

    // Aggiungo la data di ultima modifica

    $row[] = $entity_updated_data;

    // Aggiungo lo stato e la classe per cambio colore con css

    $row[] = array(
      'data' => array(
        '#markup' => $entity_status,
      ) ,
      'class' => array(
        strtolower($entity_status)
      ) ,
    );

    // Aggiungo l'operazione di edit

    $row[] = l('edit', __documentation_admin_path__ . '/manage/' . $id);

    // Aggiunto l'operazione cancellazione

    $row[] = l('delete', __documentation_admin_path__ . '/manage/' . $id . '/delete');

    // Ritorna la riga

    return $row;
  }
}