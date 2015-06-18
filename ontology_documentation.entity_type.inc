<?php

class OntologyDocumentationType extends Entity {

  public function defaultUri() {
    return array('path' => 'admin/structure/ontology-documentation-types');
  }

  public function __construct(array $values = array(), $entity_type = NULL) {
    $ontologyDocumentationTypeDataArray = array(
      'locked' => '1'
    );
    parent::__construct(array_merge($ontologyDocumentationTypeDataArray, $values), $entity_type);
  }
}