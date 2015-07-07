<?php

class OntologyDocumentation extends Entity {
  public function defaultUri() {
    return array('path' => __documentation_view_path__.'/'.$this->category.'/'. str_replace('_', '-', $this->label()));
  }

  public function defaultLabel() {
    return $this->getTranslation($this->entityInfo['entity keys']['label']);
  }

  public function __construct(array $values = array(), $entity_type = NULL) {
    // NECESSARIO PER EVITARE CHE TALI CAMPI SARANNO SEMPRE SOVRASCRITTI
    // A CAUSA DELLA CHIAMATA AL COSTRUTTURE INATTESA DOPO entity_load
    if (!empty($values)) {
      $values += array(
        'language' => 'en',
        'uid' => ontology_documentation_util('get_mastro_uid'),
        'created' => REQUEST_TIME,
        'changed' => REQUEST_TIME,
        'status' => 1,
      );
    }
    parent::__construct($values, $entity_type);
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