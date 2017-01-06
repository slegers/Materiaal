<?php

function materiaal_add_ding_form($form,$form_state,$ding = NULL){

	if(is_null($ding)){
		$ding = new StdClass();
		$ding->naam ='';
		$ding->verhuurbaar_aan= 'E';
		$ding->prijs_leden = '';
		$ding->prijs_niet_leden = '';
		$ding->waarborg = '';
		$form['#ding'] = false;
	}else{
		$form['#ding'] = $ding;
	}
	$form['name'] = array(
	'#type'=> 'textfield',
	'#default_value'=> isset($ding->naam) ? $ding->naam : '',
	'#title'=> t('What?'),
	'#required'=> true,
	'#size'=> 30,
	);
	$form['leasable_to'] = array(
	'#title'=> t('Leasable to?'),
	'#type'=> 'radios',
	'#default_value'=> isset($ding->verhuurbaar_aan) ? $ding->verhuurbaar_aan : '',
	'#options'=> array(	
		'P'=> t('Presidium'),
		'E'=> t('Everyone'),
		),
	'#required'=> true,
	);
	$form['member_price'] = array(
	'#default_value'=> isset($ding->prijs_leden) ? $ding->prijs_leden : '',
	'#type'=> 'textfield',
	'#title'=> t('Member price?'),
	'#size'=> 10,
	'#description'=> t('In eurocents'),
	'#required'=> true,
//	'#disabled' => $form['#ding'],
	);
	$form['non_member_price'] = array(
	'#default_value'=> isset($ding->prijs_niet_leden) ? $ding->prijs_niet_leden : '',
	'#type'=> 'textfield',
	'#title'=> t('Non member price?'),
	'#size'=> 10,
	'#description'=> t('In eurocents'),
	'#required'=> true,
//	'#disabled' => $form['#ding'],
	);
	$form['guarantee'] = array(
	'#default_value'=> isset($ding->waarborg) ? $ding->waarborg : '',
	'#type'=> 'textfield',
	'#title'=> t('Guarantee?'),
	'#size'=> 10,
	'#description'=> t('In eurocents'),
	'#required'=> true,
//	'#disabled' => $form['#ding'],
	);
	$form['actions'] = array(
	'#type' => 'actions',
	);
	$form['actions']['submit'] = array(
	'#value'=> array('Submit'),
	'#type'=> 'submit',
	);
/*
	if($form['#ding']){
	
		$form['actions']['delete'] = array(
		'#value'=> t('Delete'),
		'#type'=> 'submit',
		);

	}*/
	return $form;
}

function materiaal_add_ding_form_validate($form,$form_state){	
	if(!is_numeric($form_state['values']['member_price'])&& $form_state['values']['member_price'] >= 0){
		form_set_error('member_price',t('Please enter an integer!'));
	}elseif(!is_numeric($form_state['values']['non_member_price']) && $form_state['values']['non_member_price'] >= 0){
		form_set_error('non_member_price',t('Please enter an integer!')); 
	}elseif(!is_numeric($form_state['values']['guarantee']) && $form_state['values']['guarantee'] >= 0){ 
		form_set_error('guarantee',t('Please enter an integer!'));
	} 	 	
}
function materiaal_add_ding_form_submit(&$form,&$form_state){
	
	switch($form_state['triggering_element']['#parents'][0]){
		case 'delete':
		
			$qu = db_delete('materiaal_ding');
		        $qu->condition('ding_id',$form['#ding']->ding_id);
		        $qu->execute();
	        	drupal_goto('materiaal/reservation');

			break;
		case 'submit':
			$ding = $form['#ding'];

		        if($ding == false){
			        db_insert('materiaal_ding')
			        ->fields(array(
		                'ding_id'=> materiaal_get_max('materiaal_ding','ding_id')+1,
                		'naam' => $form_state['values']['name'],
		                'verhuurbaar_aan'=> $form_state['values']['leasable_to'],
               			'prijs_leden'=>$form_state['values']['member_price'],
		                'prijs_niet_leden'=>$form_state['values']['non_member_price'],
               			'waarborg'=> $form_state['values']['guarantee'],
			        ))->execute();
		        	dsm('Saved!');
	        	}else{
		                db_update('materiaal_ding')
                		->condition('ding_id',$ding->ding_id)
		                ->fields(array('naam'=> $form_state['values']['name'],
                	        'verhuurbaar_aan'=>$form_state['values']['leasable_to'],
                      		'prijs_leden'=>$form_state['values']['member_price'],
	                        'prijs_niet_leden'=>$form_state['values']['non_member_price'],
        	                'waarborg'=>$form_state['values']['guarantee'],))
               			 ->execute();
		                dsm('Edited!');
        		}
		        drupal_goto('materiaal/reservation');

			break;
	}
}


