<?php

function materiaal_admin($form,$form_state){
	$form['settings'] = array(
	'#type' => 'fieldset',
	'#title' => t('General settings'),
	'#collapsible' => true,
	);
	$form['settings']['disable_reservations'] = array(
	'#type' => 'checkbox',
	'#title' => t('Disable reservations'),
	'#description' => t('Disable reservations for all members.'),
	'#default_value' => variable_get('disable_reservations',0),
	);
	$form['settings']['start_kiesweek'] = array(
	'#type' => 'date_select',
	'#title' => t('Start election week Wina'),
	'#date_format' => 'j/n/Y',
	'#default_value' => variable_get('start_election',date("now")),
	'#date_label_position' => '',
	'#date_year_range' => '0:+1',
	'#required' => true,
	);
	 $form['settings']['end_kiesweek'] = array(
        '#type' => 'date_select',
        '#title' => t('End election week Wina'),
	'#date_format' => 'j/n/Y',
	'#default_value' => variable_get('end_election',date("now")),
        '#date_label_position' => '',
        '#date_year_range' => '0:+1',
        '#required' => true,
        );
	 $form['settings']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        );
	return $form;
}
function materiaal_admin_submit($form,$form_state){
	variable_set('disable_reservations',$form_state['values']['disable_reservations']);
	variable_set('start_election',$form_state['values']['start_kiesweek']);
	variable_set('end_election',$form_state['values']['end_kiesweek']);
}
