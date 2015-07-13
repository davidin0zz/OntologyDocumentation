<?php

class OntologyDocumentationWrapper extends EntityDrupalWrapper {

  public function getCachedUncheckedWrapperProperties() {
    $properties = array(
      'id',
      'rid',
      'descrizione',
      'creation_date',
      'uid',
      'modification_date',
      'state',
      'valid',
    );
    return $properties;
  }

  public function getFieldRawContent($field) {
    if (!isset($this->{$field})) {
      throw new Exception('Trying to access non existent property');
    }
    elseif ($this->{$field} instanceof EntityStructureWrapper) {
      return $this->{$field}->value->raw();
    }
    elseif ($this->{$field} instanceof EntityValueWrapper) {
      return $this->{$field}->raw();
    }
    else {
      return $this->{$field};
    }
  }

  public function setFieldsContent(array $data) {
    foreach ($data as $field => $content) {
      if (!isset($this->{$field})) {
        throw new Exception('Trying to set non existent property');
      }
      elseif ($this->{$field} instanceof EntityStructureWrapper) {
        $this->{$field}->{key($content)}->set(current($content));
      }
      elseif ($this->{$field} instanceof EntityValueWrapper) {
        $this->{$field}->set(current($content));
      }
      else {
        $this->{$field} = current($content);
      }
    }
  }

}