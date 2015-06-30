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
    $items[$this->path]['title'] = 'Ontology types';
    $items[$this->path]['weight'] = -1;
    // Previene la visualizzare del pannello di amministrazione dell'entità
    // all'utente autenticato con il solo permesso di 'view'
    $items[$this->path]['access arguments'] = array('update', $this->entityType);
    return $items;
  }
}