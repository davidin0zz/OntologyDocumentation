<?php

class OntologyDocumentationWrapper extends EntityDrupalWrapper {

  public function getCachedUncheckedWrapperProperties() {
    $properties = array(
      'id',
      'rid',
      'descrizione',
      'creation_date',
      'modification_date',
      'state',
      'valid',
    );
    return $properties;
  }

  public function getFieldRawContent($field) {
    if ($this->{$field} instanceof EntityStructureWrapper) {
      return $this->{$field}->value->raw();
    }
    else {
      return $this->{$field}->raw();
    }
  }

  public function setFieldsContent(array $data) {
    foreach ($data as $field => $content) {
      if ($this->{$field} instanceof EntityStructureWrapper) {
        $this->{$field}->value->set($content);
      }
      else {
        $this->{$field}->set($content);
      }
    }
  }

}