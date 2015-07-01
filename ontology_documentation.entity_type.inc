<?php

class OntologyDocumentationType extends Entity {

  public function defaultUri() {
    return array('path' => __documentation_bundle_admin_path__);
  }

}