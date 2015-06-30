<?php

class OntologyDocumentationTypeUIController extends EntityDefaultUIController {
  function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
     array_unshift($additional_header, t('NAME'));
     $additional_header[] = array(
       'data' => 'OPERATIONS',
       'colspan' => $this->operationCount(),
     );
    return $additional_header;
  }
  function hook_menu() {
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
    return $items;
  }
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = array();
    $label = $entity->title;
    $description = $entity->description;

    // Aggiungo il nome del bundle e la descrizione
    $row[] = array(
      'data' => array(
        'label' => array(
          '#markup' => $label,
        ),
        'description' => array(
          '#type'=> 'html_tag',
          '#tag' => 'div',
          '#value' => $description,
          '#attributes' => array('class' => 'description'),
        )
      ),
    );
    // Aggiungo l'operazione di edit
    $row[] = l('edit',__documentation_bundle_admin_path__.'/manage/'.$id);
    // Aggiunto l'operazione di manage fields
    $row[] = l('manage fields',__documentation_bundle_admin_path__.'/manage/'.$id.'/fields');
    // Aggiunto l'operazione di manage displays
    $row[] = l('manage display',__documentation_bundle_admin_path__.'/manage/'.$id.'/display');

    return $row;
  }
}