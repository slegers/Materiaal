<?php

require_once('reservatie.mail.inc');


function materiaal_form($form,$form_state,$reservatie = null){

	global $user;
	$delete = true;
	if(is_null($reservatie)){
		$reservatie = new StdClass();
		$reservatie->reservatie_id = null;
		$reservatie->reden = null;
		$reservatie->tijdstip_aanvraag = date('d m Y h i');	
		$reservatie->persoon_id = null;
		$reservatie->tijdstip_afhalen = date('d m Y h i');
		$reservatie->tijdstip_teruggave = ''; 
		$reservatie->presidium_activiteit = 'n';
		$reservatie->is_lid = false;
		$form['#reservatie'] = false;
	$delete = false;
	}else{
		$form ['#reservatie'] = $reservatie;
	}	
if($user->uid != null){
	if(!isset($reservatie->persoon_id)){
		$persoon = materiaal_get_persoon($user->persoon_id);
		$form['#persoon'] = $persoon;
	}else{
		$persoon = materiaal_get_persoon($reservatie->persoon_id);
                $form['#persoon'] = $persoon;
	}

        $form['gsm'] = array(
	'#type'=> 'textfield',
	'#default_value'=> isset($persoon->gsm)? $persoon->gsm : '',
	'#size'=> 50,
	'#title'=> t('GSM'),
	'#required'=> true,
	);
	$form['mail'] = array(
	'#type'=> 'textfield',
	'#size'=> 50,
	'#default_value'=> isset($persoon->mail) ? $persoon->mail : '',
	'#title'=> t('Mail'),
	'#required'=> true,
	);


        $form['tijdstip_afhalen'] = array(
        '#type'=> 'date_select',
        '#title'=> t('Pickup date'),
        '#default_value' => isset($reservatie->tijdstip_afhalen) ? date($reservatie->tijdstip_afhalen) : date('d m Y h i'),
        '#date_label_position' => 'within',
        '#date_year_range' => '0:+1',
        '#required'=> true,
        );
        $form['tijdstip_teruggave'] = array(
        '#type'=> 'date_select',
        '#title'=> t('Return date'),
        '#default_value' => isset($reservatie->tijdstip_teruggave) ? date($reservatie->tijdstip_teruggave) : date('d m Y h i'),
        '#date_label_position' => 'within',
        '#date_year_range' => '0:+1',
        '#required'=> true,
        );
	
	$form['reden'] = array(
	'#type'=> 'textarea',
	'#title'=> 'reason',
	'#required'=> 'true',
	'#description'=> t('The reason why you want to hire these materials.'),
	'#default_value' => isset($reservatie->reden) ? $reservatie->reden : '',
	);

	$query = db_select('materiaal_ding','md');
        $query->fields('md',array('ding_id','naam','prijs_leden','prijs_niet_leden','waarborg'))
        ->Orderby('naam','asc');
        if(!materiaal_is_presidium($user->persoon_id)){
                $query->condition('verhuurbaar_aan','P','<>');
	}else{	

	$form['presidium'] = array(
	'#type'=> 'radios',
	'#title'=> t('Presidium activity?'),
	'#options'=> array(
		'y' => t('Yes'),
		'n' => t('No'),
		),
	'#required'=> true,
	'#default_value'=> isset($reservatie->presidium_activiteit) ? 'y' : 'n',
	);
        }

        $header = array(t('Name'),t('Member price'),t('Non member price'), t('Guarantee'),t('Order?'));
        $form['table'] = array(
        '#type'=> 'formtable',
        '#title'=> t('Material'),
        '#tree'=> true,
        '#header'=> $header,
	
        );
        foreach($query->execute()->fetchAll() as $ding){
                $form['table'][$ding->ding_id] = array(
                'naam'=> array('#markup'=>l($ding->naam,'materiaal/edit/'.$ding->ding_id )), 
                'pl'=> array('#type'=> 'item', '#markup'=> '<p>' .materiaal_price_to_euro($ding->prijs_leden) . '</p>'),
                'pnl'=> array('#type'=> 'item', '#markup'=> '<p>' .materiaal_price_to_euro($ding->prijs_niet_leden) . '</p>'),
                'w'=> array('#type'=> 'item', '#markup'=> '<p>' . materiaal_price_to_euro($ding->waarborg). '</p>'),
                );
		
                $form['table'][$ding->ding_id]['checkbox'] = array(
                '#type'=> 'checkbox',
		'#default_value' => materiaal_behoort_tot_reservatie($ding->ding_id,$reservatie->reservatie_id),
                );

        }
	$form['submit'] = array(
        	'#type'=> 'submit',
	        '#value'=> t('Order?'),
		'#disabled' => variable_get('disable_reservations'),
        	);
	if( variable_get('disable_reservations')){
		drupal_set_message(t('We aren\'t currently accepting any reservations.'));
	}
	if($delete){
		$form['reject'] = array(
		'#type'=> 'submit',
		'#value' => t('Reject'),
		);

		$form['reject_reason'] = array(
		'#type' => 'textarea',
		'#description' => t('Give an explanation why the reservatioin isn\'t approaved'),
		'#title' => t('Decline'),
	        '#required' => $delete,	
		);
	}
       
}else{
	drupal_set_message(t('You need to Log in for ordering material!<a href=@url> log in.</a>', array('@url'=> url('/user'))), 'info');	
	$header = array(t('Name'),t('Member price'),t('Non member price'), t('Guarantee'));
	$form['table'] = array(
	'#type'=> 'formtable',  
	'#title'=> t('Material'), 
	'#tree'=> true, 
	'#header'=> $header,
	);

	$query = db_select('materiaal_ding','md');
	$query->fields('md',array('ding_id','naam','prijs_leden','prijs_niet_leden','waarborg'))
	->condition('verhuurbaar_aan','P','<>') 
	->Orderby('naam','asc'); 
	foreach($query->execute()->fetchAll() as $ding){
		$form['table'][$ding->ding_id] = array(
		'naam'=> array('#markup'=>l($ding->naam,'materiaal/edit/'.$ding->ding_id )),
		'pl'=> array('#type'=> 'item', '#markup'=> '<p>' .materiaal_price_to_euro($ding->prijs_leden) . '</p>'),
		'pnl'=> array('#type'=> 'item', '#markup'=> '<p>' .materiaal_price_to_euro($ding->prijs_niet_leden) . '</p>'),
		'w'=> array('#type'=> 'item', '#markup'=> '<p>' . materiaal_price_to_euro($ding->waarborg). '</p>'),
		);
	}
	

}
	return $form;	
	
}

function materiaal_is_reserveerbaar($form,$form_state){	
	$q = db_select('materiaal_aanvraag_reservatie');
	$q->condition(db_or()->condition('tijdstip_teruggave',strtotime($form_state['values']['tijdstip_afhalen']),'>=')->condition('tijdstip_afhalen',strtotime($form_state['values']['tijdstip_teruggave']),'>='))
	->fields('materiaal_aanvraag_reservatie');
		
	

foreach($q->execute()->fetchAll() as $reservatie){
        foreach($form_state['values']['table'] as $key => $value){
		if($value['checkbox'] == 1){
               	 	if(materiaal_behoort_tot_reservatie($key,$reservatie->reservatie_id)){ 
				dsm('Some items are already reservated at this moment');
                        	return false;
               	 	}
        	}
	}
}

	if(($form_state['values']['tijdstip_afhalen'] >= variable_get('start_election') and $form_state['values']['tijdstip_afhalen'] <= variable_get('end_election')) or ($form_state['values']['tijdstip_teruggave'] >= variable_get('start_election') and $form_state['values']['tijdstip_teruggave'] <= variable_get('end_election'))){
		drupal_set_message(t('We don\'t allow reservations in the election week.'));
		return false;
	} 
	return true; 
}

function materiaal_is_presidium($persoon_id){
	$q = ldb_hierarchie_query();
	//117 is presidium
	$q->condition('parent.groep_id',117)
	->condition('hierarchie.toonopwebsite',1);
	$ids= $q->execute()->fetchCol();

	$sub = array();
	foreach($ids as $id){
		$qu = ldb_select('persoon_groep');
		$qu->condition('persoon_id',$persoon_id)
		->condition('groep_id',$id)
		->condition('jaar',ldb_get_jaar('boekjaar'))
		->fields('persoon_groep');
		//1876 is prowebteam
		if(count($qu->execute()->fetchAll()) > 0 and $id != 1876){
			return true;
		}	
	}	

}
function materiaal_form_validate($form,$form_state){

	$d = date('Y-m-d H:i',time());

	if($d > $form_state['values']['tijdstip_afhalen']){
		form_set_error($form_state['values']['tijdstip_afhalen'], t('The pickup date can\'t be in the past.'));
	}elseif($form_state['values']['tijdstip_afhalen'] > $form_state['values']['tijdstip_teruggave']){
		form_set_error($form_state['values']['tijdstip_teruggave'],t('The pickup date must be before the return date!'));
	}
	if($form_state['triggering_element']['#parents'][0] == 'delete'){
		dsm($form_state['values']['del_reason']);
	}
}
function materiaal_form_submit(&$form,&$form_state){

switch($form_state['triggering_element']['#parents'][0]){
	case 'submit':

	$t = time();	
	$reservatie = $form['#reservatie'];
	if(materiaal_is_reserveerbaar($form,$form_state)){
		if($form['#reservatie'] == false){
		$reservatie_id =  materiaal_get_max('materiaal_aanvraag_reservatie','reservatie_id')+1;	
		db_insert('materiaal_aanvraag_reservatie')
                        	->fields(array(
                       		'reservatie_id' => $reservatie_id,
                        	'reden' => $form_state['values']['reden'],
                       	 	'tijdstip_aanvraag' => $t, 
                       		'persoon_id' => $form['#persoon']->persoon_id,
                	        'tijdstip_afhalen' => strtotime($form_state['values']['tijdstip_afhalen']),
                	        'tijdstip_teruggave'=> strtotime($form_state['values']['tijdstip_teruggave']),
                	        'presidium_activiteit' => $form_state['values']['presidium'],
                	        'is_lid' => materiaal_is_lid($form['#persoon']->persoon_id),
                	        )
                	        )->execute();

			
		} else{
			$reservatie_id = $reservatie->reservatie_id;
			db_delete('materiaal_reservatie_bevat_ding')
        		->condition('reservatie_id',$reservatie->reservatie_id)
        		->execute();
			db_update('materiaal_aanvraag_reservatie')
			->condition('reservatie_id',$reservatie->reservatie_id)
			->fields(array(
			'tijdstip_afhalen' => strtotime($form_state['values']['tijdstip_afhalen']),
			'tijdstip_teruggave'=> strtotime($form_state['values']['tijdstip_teruggave']),
			'presidium_activiteit' => $form_state['values']['presidium'],
			)
			)->execute();
		}
	
		
		foreach($form_state['values']['table'] as $key => $value){
			
			if($value['checkbox'] == 1){
				db_insert('materiaal_reservatie_bevat_ding')
				->fields(array(
				'reservatie_id' => $reservatie_id,
				'ding_id' => $key,
				)	
				)->execute();
			}
		}

		$query = db_select('materiaal_reservatie_bevat_ding');
		$query->condition('reservatie_id',$reservatie_id)
		->fields('materiaal_reservatie_bevat_ding',array('ding_id'));
		$params['dingen'] = $query->execute()->fetchAll();	
		$params['reservatie_id'] = $reservatie_id;

		$params['persoon'] = $form['#persoon'];

		//TODO mails
     
		drupal_mail('materiaal','aanvraag_materiaal','materiaal@wina.be',null,$params,'materiaal@wina.be',true);   
		drupal_mail('materiaal','aanvraag_klant',$form['#persoon']->mail,null,$params,'materiaal@wina.be',true);
              //rupal_mail('materiaal','aanvraag_materiaal','yanice@wina.be',null,$params,'materiaal@wina.be',true);
	dsm(t('Your reservation is requested. You\'ll receive a mail with an aproval or deny'));
	}
		break;
	case 'reject':
		$params = array();
		$params['reservatie_id'] = $form['#reservatie']->reservatie_id;
		$params['reject_reason'] = $form_state['values']['reject_reason'];
		drupal_mail('materiaal','aanvraag_afgekeurd',$form['#persoon']->mail,null,$params,'materiaal@wina.be',true);
		
		db_delete('materiaal_aanvraag_reservatie')
        	->condition('reservatie_id',$form['#reservatie']->reservatie_id)
	        ->execute();

       		 db_delete('materiaal_reservatie_bevat_ding')
	        ->condition('reservatie_id',$form['#reservatie']->reservatie_id)
        	->execute();

		drupal_goto('/materiaal');                                                             
	break;
}

}
function materiaal_ajax_naam_callback($form,$form_sate){
	return $form['textfields'];
}
