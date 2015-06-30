<?php

class OntologyDocumentationType extends Entity {

  public function defaultUri() {
    return array('path' => __documentation_bundle_admin_path__);
  }

  public function __construct(array $values = array(), $entity_type = NULL) {
    $ontologyDocumentationTypeDataArray = array();
    parent::__construct(array_merge($ontologyDocumentationTypeDataArray, $values), $entity_type);
  }
}