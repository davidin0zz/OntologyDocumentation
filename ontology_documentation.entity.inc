<?php

class OntologyDocumentation extends Entity {
  public function defaultUri() {
    return array('path' => __documentation_view_path__.'/'.$this->category.'/'. str_replace('_', '-', $this->label()));
  }
  public function defaultLabel() {
    return $this->getTranslation($this->entityInfo['entity keys']['label']);
  }
  public function __construct(array $values = array(), $entity_type = NULL) {
    $values += array(
        'language' => 'en',
        'uid' => ontology_documentation_util('get_mastro_uid'),
        'created' => REQUEST_TIME,
        'changed' => REQUEST_TIME,
        'status' => 1,
    );
    parent::__construct($values, $entity_type);
  }
}