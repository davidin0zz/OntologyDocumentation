<?php
class OntologyDocumentationController extends EntityAPIController
{
  public function create(array $values = array())
  {
    $values+= array(
      'language' => 'en',
      'uid' => ontology_documentation_util('get_mastro_uid') ,
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'status' => 1,
      'old' => 0,
    );
    return new OntologyDocumentation($values, $this->entityType);
  }

  public function save($entity, DatabaseTransaction $transaction = NULL)
  {
    $cache_array = & drupal_static(__FUNCTION__, array());
    $report = & drupal_static('report_' . __FUNCTION__, array());
    $cache_is_valid = true;
    if (empty($cache_array))
    {
      $cache_array = ontology_documentation_cache(__documentation_type__, __documentation_type_cache__);
      if (is_object($cache_array) && isset($cache_array->data))
      {
        $cache_array = $cache_array->data;
      }
      else
      {
        $cache_array = array();
      }
    }

    if (empty($report))
    {
      $report = array(
        'PROCESSED' => array() ,
        'NEW' => array() ,
        'UPDATED' => array() ,
      );
    }

    $entity_name = $entity->label();
    $ui_update = $entity->created != $entity->changed;
    $is_old = $entity->old;
    $cache_hit = isset($cache_array[$entity_name]);
    $report['PROCESSED']+= array(
      $entity_name => $entity_name
    );

    // La data di creazione e di modifica dell'entitÃ  possono differire solamente
    // in caso di aggiornamento manuale da parte dell'utente attraverso la UI.
    // In fase di creazione le due date saranno necessariamente uguali.
    // Inoltre il form di modifica non permette il salvataggio dei campi dell'entitÃ
    // se almeno uno di essi non differisce dal valore corrente. Siamo quindi certi
    // di trovarci in uno stato di aggiornamento, possiamo cosÃ¬ ignorare la cache
    // e non spendere tempo alla ricerca delle differeze. Scriviamo subito i cambiamenti
    // e aggiorniamo la cache solamente se l'entitÃ  in esame non Ã¨ marcata come old,
    // avremmo altrimenti il reinserimento in cache della stessa, DA EVITARE!

    if ($ui_update)
    {
      parent::save($entity, $transaction);
      if (!$is_old)
      {
        $cache_array[$entity_name] = $entity;
        cache_set(__documentation_type__, $cache_array, __documentation_type_cache__);
      }
    }
    elseif (!$cache_hit)
    {
      parent::save($entity, $transaction);

      // Sovrascrivo la cache per sostituire la vecchia entitÃ  con la nuova.
      // Esistono due casi che necessitano di aggiornamento, il primo Ã¨ l'aggiunta
      // di un nuovo termine dell'ontologia, il secondo, piÃ¹ significativo, Ã¨
      // l'aggiornamento della cache a seguito dell'aggiunta di un termine non
      // presente nel db ma di cui ne esiste una vecchia versione in cache. Nel
      // secondo caso += array() non permette la corretta sostituzione della
      // vecchia con la nuova, perchÃ¨ l'unione della vecchia con la nuova, basandosi
      // sugli stessi indici associativi, previene la sovrascrittura della seconda.
      // CiÃ² creerebbe uno stato inconsistente tra la cache per l'entitÃ  e l'entitÃ
      // memorizzata nel db, DA EVITARE!!

      $cache_array[$entity_name] = $entity;

      // Non corriamo la stessa disavventura nell'usare l'operaore di unione in questo
      // caso, poichÃ¨ la variabile report non Ã¨ persistente ma statica durante il ciclo
      // di chiamata del metodo.

      $report['NEW']+= array(
        $entity_name => $entity_name
      );
    }

    // Se l'entitÃ  Ã¨ sia in cache che nel db, ispezioniamo le sue proprietÃ  alla ricerca
    // delle differenze con la versione meno recente in cache. Le differenze vengono
    // memorizzate in forma di $proprietÃ  => $valore , dove $proprietÃ  deve essere una
    // proprietÃ  palesata al wrapper attraverso l'hook_entity_property_info().

    else
    {
      $array_diff = array();
      $entity_cache = $cache_array[$entity_name];
      $entity_wrapper = ontology_documentation_wrapper(__documentation_type__, $entity);
      $cached_entity_wrapper = ontology_documentation_wrapper(__documentation_type__, $entity_cache);
      foreach(array_keys($entity_wrapper->getPropertyInfo()) as $property)
      {
        if (!in_array($property, $entity_wrapper->getCachedUncheckedWrapperProperties()))
        {
          $current_content = $cached_entity_wrapper->getFieldRawContent($property);
          $new_content = $entity_wrapper->getFieldRawContent($property);
          $match = $new_content === $current_content ? true : false;
          $cache_is_valid = $cache_is_valid && $match;

          // Se la proprietÃ  tra l'entitÃ  in cache e la sua versione corrente sono diverse,
          // aggiungo all'array delle differenze il suo nuovo valore indicizzato sulla proprietÃ .

          if (!$match)
          {
            $array_diff+= array(
              $property => array(
                'value' => $new_content
              )
            );
          }
        }
      }

      // Se la cache non Ã¨ valida perchÃ¨ la nuova entitÃ  Ã¨ diversa nei suoi campi significativi,
      // carico l'entitÃ  memorizzata nel db e ci applico le modifiche. Infine aggiorno la cache
      // e aggiorno il report di aggiornamento.

      if (!$cache_is_valid)
      {
        $stored_entity = entity_load_single(__documentation_type__, $cached_entity_wrapper->getFieldRawContent('id'));
        $stored_entity_wrapper = ontology_documentation_wrapper(__documentation_type__, $stored_entity);

        // Aggiorno l'entitÃ

        $properties = $array_diff + array(
          'modification_date' => array(
            'value' => REQUEST_TIME
          )
        );
        $stored_entity_wrapper->setFieldsContent($properties);
        parent::save($stored_entity, $transaction);
        $report['UPDATED'][$entity_name] = $array_diff;
        $cache_array[$entity_name] = $stored_entity;
      }
      else
      {

        // Termine giÃ  aggiornato

      }
    }

    return array(
      'cache' => $cache_array,
      'report' => $report
    );
  }
}
