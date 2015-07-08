<?php

class OntologyDocumentationController extends EntityAPIController {

  public function create(array $values = array()) {
    $values += array(
      'language' => 'en',
      'uid' => ontology_documentation_util('get_mastro_uid'),
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'status' => 1,
    );
    return new OntologyDocumentation($values, $this->entityType);
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $cache_array = &drupal_static(__FUNCTION__, array());
    $report = &drupal_static('report_'.__FUNCTION__, array());
    $query = &drupal_static('query_'.__FUNCTION__);
    $cache_is_valid = true;

    if (empty($cache_array)) {
      $cache_array = ontology_documentation_cache(__documentation_type__, __documentation_type_cache__);
      if (is_object($cache_array) && isset($cache_array->data)) {
        $cache_array = $cache_array->data;
      }
      else {
        $cache_array = array();
      }
    }

    if (empty($report)) {
      $report = array(
        'new' => array(),
        'processed' => array(),
        'updated' => array(),
      );
    }

    if (!isset($query)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', __documentation_type__);
    }

    $entity_name = $entity->label();
    $ui_update = $entity->created != $entity->changed ? true : false;
    $cache_hit = isset($cache_array[$entity_name]) ? true : false;
    $report['processed'] += array($entity_name => $entity);
    $query->propertyConditions = array();
    $query->propertyCondition('title', $entity_name);
    $stored_entity_idkey = array_keys($query->execute());
    $term_is_on_db = !empty($stored_entity_idkey);
    //dpm($term_is_on_db, $entity_name.' is on db?');
    //dpm($cache_hit, $entity_name.' hit cache?');
    //dpm($cache_array,'current');

    if ($ui_update) {
      dpm('TERM UPDATE FROM USER');
      parent::save($entity, $transaction);
      $cache_array[$entity_name] = $entity;
      cache_set(__documentation_type__, $cache_array, __documentation_type_cache__);
    }
    elseif (!$cache_hit || !$term_is_on_db) {
      dpm('NEW TERM: '.$entity_name. ' created');
      parent::save($entity, $transaction);
      $report['new'] += array($entity_name => $entity_name);
      $cache_array += array($entity_name => $entity);
    }
    else {
      dpm("CACHE CHECKING");
      $array_diff = array();
      $entity_cache = $cache_array[$entity_name];
      $entity_wrapper = ontology_documentation_wrapper(__documentation_type__, $entity);
      $cached_entity_wrapper = ontology_documentation_wrapper(__documentation_type__, $entity_cache);
      foreach ($entity_wrapper->getPropertyInfo() as $property => $value) {
        if (!in_array($property, $entity_wrapper->getCachedUncheckedWrapperProperties())) {
          $cached_content = $cached_entity_wrapper->getFieldRawContent($property);
          $entity_content = $entity_wrapper->getFieldRawContent($property);
          $match = $entity_content == $cached_content ? true : false ;
          $cache_is_valid = $cache_is_valid && $match;
          // Aggiorno la cache
          if (!$match) {
            //dpm($property, 'non match campo');
            //dpm($entity_content, 'valore entità');
            //dpm($cached_content, 'valore cache');
            $array_diff += array($property => $entity_content);
          }
        }
      }
      if (!$cache_is_valid) {
        dpm('TERM UPDATE FROM MASTRO');
        $array_diff += array('modification_date' => REQUEST_TIME);
        $stored_entity = entity_load_single(__documentation_type__, $cached_entity_wrapper->id->value());
        $stored_entity_wrapper = ontology_documentation_wrapper(__documentation_type__, $stored_entity);
        // Aggiorno l'entità
        $stored_entity_wrapper->setFieldsContent($array_diff);
        parent::save($stored_entity, $transaction);
        $report['updated'][$entity_name] = $array_diff;
        $cache_array[$entity_name] = $stored_entity;
      }
      else {
        dpm('TERM ALREADY UPDATED');
      }
    }

    /*
    elseif (!$cache_hit && $term_is_on_db) {
      //entity_delete(__documentation_type__, current($stored_entity_idkey));
      //parent::save($entity, $transaction);
      //$cache_array['report']
      throw new Exception("$entity_name: cache corrupted!");
    }*/

    return array('cache' => $cache_array, 'report' => $report);
  }
}