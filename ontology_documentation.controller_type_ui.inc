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
    return $items;
  }
}