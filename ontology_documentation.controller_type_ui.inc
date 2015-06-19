<?php

class OntologyDocumentationUIController extends EntityDefaultUIController {
  function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
     array_unshift($additional_header, t('NAME'));
     $additional_header[] = array(
       'data' => 'OPERATIONS',
       'colspan' => $this->operationCount(),
     );
    return $additional_header;
  }
  //function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
  //  $entity_info = $entity->entityInfo;
  //
  //
  //
  //  );
  //
  //}

}