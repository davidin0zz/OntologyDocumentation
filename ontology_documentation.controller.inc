<?php

class OntologyDocumentationController extends EntityAPIController {

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);
  }
}