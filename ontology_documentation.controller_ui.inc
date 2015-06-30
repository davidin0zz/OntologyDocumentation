<?php

class OntologyDocumentationUIController extends EntityDefaultUIController {
  function hook_menu() {
    $items = parent::hook_menu();
    // $this->path = admin/ontology-documentation
    $items[$this->path]['title'] = 'Ontology';
    $items[$this->path]['weight'] = -10;
    // Previene la visualizzare del pannello di amministrazione dell'entità
    // all'utente autenticato con il solo permesso di 'view'
    $items[$this->path]['access arguments'] = array('update', $this->entityType);
    return $items;
  }
}