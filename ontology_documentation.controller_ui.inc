<?php

class OntologyDocumentationUIController extends EntityDefaultUIController {
  function hook_menu(){
    $items = parent::hook_menu();
    $items[$this->path]['title'] = 'Ontology';
    $items[$this->path]['weight'] = -10;
    return $items;
  }
}