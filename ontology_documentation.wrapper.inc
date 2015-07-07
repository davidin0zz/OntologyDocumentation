<?php

class OntologyDocumentationWrapper extends EntityDrupalWrapper {
  public function getCachedUncheckedWrapperProperties() {
    return array('id', 'rid', 'descrizione', 'creation_date', 'modification_date' , 'state');
  }
}