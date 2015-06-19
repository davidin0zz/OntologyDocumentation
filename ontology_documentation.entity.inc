<?php

class OntologyDocumentation extends Entity {
  public function defaultUri() {
    return array('path' => 'ontology-documentation/'. $this->category . '/' . str_replace('_', '-', $this->label()));
  }
  public function defaultLabel() {
    return $this->getTranslation($this->entityInfo['entity keys']['label']);
  }
  public function __construct(array $values = array(), $entity_type = NULL) {
    $ontologyDocumentationDataArray = array(
      'language' => 'en',
      'uid' => get_mastro_uid(),
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'status' => 1,
    );
    parent::__construct(array_merge($ontologyDocumentationDataArray, $values), $entity_type);
  }
}