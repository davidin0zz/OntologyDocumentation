<?php

class OntologyDocumentation extends Entity {
  public function defaultUri(){
    return array('path' => 'ontology-documentation/'. $this->category . '/' . str_replace('_', '-', $this->label()));
  }
  public function defaultLabel(){
    return $this->getTranslation($this->entityInfo['entity keys']['label']);
  }
}