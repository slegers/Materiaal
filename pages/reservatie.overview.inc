<?php


function materiaal_overview_form($form,$form_state){
	if( variable_get('disable_reservations')){
                drupal_set_message(t('We aren\'t currently accepting any reservations.'));
        }
   	$header = array(
    			array('data' => 'Pickup date', 'field' => 'tijdstip_afhalen', 'sort' => 'ASC'),
			array('data' => 'Return date', 'field' => 'tijdstip_teruggave', 'sort' => 'ASC'),
			array('data' => t('What')), 	
  		);
	

	$select = db_select('materiaal_aanvraag_reservatie','m')
		->extend('PagerDefault')
              	->extend('TableSort');
	global $user;	
	$persoon = $user;
	if(strcmp('materiaal/overview',current_path()) == 0){
		$select->condition('persoon_id',$persoon->persoon_id)
		 ->condition('tijdstip_teruggave',time(),'>=')
        	 ->fields('m', array('tijdstip_afhalen', 'tijdstip_teruggave', 'reservatie_id'))
 	         ->limit(20)
        	 ->orderByHeader($header)
 	         ->orderBy('tijdstip_teruggave', 'DESC');
	}else{
		$select->fields('m', array('tijdstip_afhalen', 'tijdstip_teruggave', 'reservatie_id'))
		 ->condition('tijdstip_afhalen',time(),'>=')
                 ->limit(20)
                 ->orderByHeader($header)
                 ->orderBy('tijdstip_teruggave', 'DESC');
	}
	$results = $select->execute();

	$rows = array();
	$materiaal = array();

	  foreach($results as $row) {
		foreach(materiaal_get_gereserveerd_materiaal($row->reservatie_id) as $reservatie){
			if(count(materiaal_get_gereserveerd_materiaal($row->reservatie_id)) == 1 && $reservatie->verhuurbaar_aan == 'P'){

			}else{
				array_push($materiaal,$reservatie->naam); 
			}
	
		}

	if(materiaal_is_goedgekeurd($row->reservatie_id)){
	    $rows[] = array(
                    date('d-m-Y H:i',$row->tijdstip_afhalen),
                    date('d-m-Y H:i',$row->tijdstip_teruggave),
		    implode('<br>',$materiaal),
    		);
	}
		$materiaal = array();
  	}

 	 $output = theme('table', array('header' => $header,
        	                         'rows' => $rows ));

	  # add the pager
	$output .= theme('pager');
	
	$form['tabel'] =  array(
	'#type'=> 'item',
	'#markup'=> $output,
	);	
	return $form;
}
