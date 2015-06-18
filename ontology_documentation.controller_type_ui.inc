<?php

class OntologyDocumentationUIController extends EntityDefaultUIController {

  function __construct($entity_type, $entity_info) {
    parent::__construct($entity_type, $entity_info);
  }

  function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
     array_unshift($additional_header, t('NAME'));
     $additional_header[] = array(
       'data' => 'OPERATIONS',
       'colspan' => $this->operationCount(),
     );

     return $additional_header;
  }

  //function overviewTable() {

  //}

}