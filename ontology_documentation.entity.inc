<?php

class OntologyDocumentation extends Entity {
  public function defaultUri() {
    return array('path' => __documentation_view_path__.'/'.$this->category.'/'. str_replace('_', '-', $this->label()));
  }

  public function defaultLabel() {
    return $this->getTranslation($this->entityInfo['entity keys']['label']);
  }

  public function __construct(array $values = array(), $entity_type = NULL) {
    parent::__construct($values, $entity_type);
  }

  public function setAsOld() {
    $this->status = 0;
    $this->old = 1;
    $this->changed = REQUEST_TIME;
  }

  public function unsetExtra() {
    if (isset($this->is_new)) {
      unset($this->is_new);
    }
    elseif (isset($this->revision) && isset($this->default_revision)) {
      unset($this->revision);
      unset($this->default_revision);
    }
  }

  public function getTitle() {
    return $this->title;
  }

  public function getDescrition() {
    return $this->wrapper()->descrizione->value->raw();
  }

  public function getCreationDate() {
    return $this->created;
  }

  public function getModificationDate() {
    return $this->changed;
  }

  public function getAuthor() {
    return user_load($this->uid)->name;
  }
}