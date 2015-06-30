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
}