<?php

class OntologyDocumentationTypeUIController extends EntityDefaultUIController {

  public function hook_menu() {
    $items = parent::hook_menu();
    //$this->path = 'admin/ontology-documentation
    $items[$this->path]['access callback'] = 'entity_access';
    // Previene la visualizzare del pannello di amministrazione dell'entità
    // all'utente autenticato con il solo permesso di 'view'
    $items[$this->path]['access arguments'] = array('update', $this->entityType);
    $items[$this->path]['weight'] = -10;
    // Disabilito gli hook_menu non necessari
    // list,clone,add,delete (%)
    $menu_hooks = array_keys($items);
    foreach ($menu_hooks as $menu_hook) {
      $url_array = explode('/', $menu_hook);
      if (in_array('list', $url_array) || in_array('clone', $url_array) ||
          in_array('add', $url_array)  || in_array('%', $url_array)) {
        unset($items[$menu_hook]);
      }
    }
    // Ritorno i nuovi uri di menu
    return $items;
  }

  public function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    // Aggiunto l'header per il nome
    $additional_header[] = t('NAME');
    // Aggiungo l'header per le operazioni
    $additional_header[] = array(
      'data' => 'OPERATIONS',
      'colspan' => $this->operationCount(),
    );
    // Ritorno l'header
    return $additional_header;
  }

  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = array();
    // Recupero il nome del bundle
    $bundle_name = $entity->title;
    // Recupero la uri del bundle
    $bundle_uri = $entity->uri();
    // Recupero la descrizione del bundle
    $bundle_description = $entity->description;

    // Aggiungo il nome del bundle e la descrizione
    $row[] = array(
      'data' => array(
        'label' => array(
          '#markup' => $bundle_name,
        ),
        'description' => array(
          '#type'=> 'html_tag',
          '#tag' => 'div',
          '#value' => $bundle_description,
          '#attributes' => array('class' => 'description'),
        )
      ),
    );
    // Aggiungo l'operazione di edit
    $row[] = l('edit',current($bundle_uri).'/manage/'.$id);
    // Aggiunto l'operazione di manage fields
    $row[] = l('manage fields', current($bundle_uri).'/manage/'.$id.'/fields');
    // Aggiunto l'operazione di manage displays
    $row[] = l('manage display', current($bundle_uri).'/manage/'.$id.'/display');
    // Ritorno la riga
    return $row;
  }
}