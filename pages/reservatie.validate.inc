<?php
require_once('reservatie.mail.inc');

function materiaal_reservatie_validate($form,$form_state){
	$header = array(t('Name'), t('Member?'), t('Pickup date'),t('Return date'),t('Approve'));

	$form['table'] = array(
	'#type'=> 'formtable',
	'#header'=> $header,
	'#tree' => true,
	'#title' => t('Requested Reservations'),
	);

	foreach(materiaal_aanvraag_reservatie() as $aanvraag){
		$persoon = materiaal_get_persoon($aanvraag->persoon_id);
		
		if(!materiaal_is_goedgekeurd($aanvraag->reservatie_id)){
		 	$form['table'][$aanvraag->reservatie_id] = array(	
			'name' => array('#type' => 'item', '#markup'=> l($persoon->voornaam . '  ' . $persoon->achternaam,'materiaal/validate_reservatie/'. $aanvraag->reservatie_id)),
			'lid' => array('#type'=> 'item', '#markup'=> materiaal_is_lid($persoon->persoon_id) == true ? t('Yes') : t('No')),
			'pickup' => array('#type'=> 'item', '#markup'=> date('d-m-Y h:i',$aanvraag->tijdstip_afhalen)),
			'return' => array('#type' => 'item', '#markup'=> date('d-m-Y h:i',$aanvraag->tijdstip_teruggave)),
			'c'=> array(
				'#type' => 'checkbox',
				'#default_value'=> 'n',
				),
			);
		}
	} 
	$form['submit'] = array(
	'#type'=> 'submit',
	'#value' => t('Allow reservation'),
	);
	return $form;
}

function materiaal_reservatie_validate_submit($form,$form_state){
	foreach($form_state['values']['table'] as $key => $val){	

		if($val['c'] !='n'){
			db_insert('materiaal_reservatie')
			->fields(array(
			'reservatie_id' => $key,
			))->execute();

		$params['reservatie_id'] = $key;
			if(materiaal_reservatie_load($key)->presidium_activiteit == 'n'){
				$persoon = ldb_persoon_load(materiaal_reservatie_load($key)->persoon_id); 
				drupal_mail('materiaal','aanvraag_goedgekeurd','yanice@wina.be',null,$params,'materiaal@wina.be',true);
				drupal_mail('materiaal','aanvraag_gang','yanice@wina.be',null,$params,'materiaal@wina.be',true);
			 }
	  	}
	} 
}
function materiaal_get_max_transactie(){
	$q = db_select('kassa_transactie');
	$q->addExpression('Max(transactie_id)');
	return $q->execute()->fetchField();
}	
