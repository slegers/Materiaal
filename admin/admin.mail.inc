<?php

function materiaal_settings_form($form,$form_state){
	$form['email'] = array(
	'#type' => 'fieldset',
	'#title' => t('E-mail settings'),
	'#collapsible'=> true,
	);
	$form['email']['message_klant_dutch'] = array(
	'#type'=> 'textarea',
	'#title' => t('Message custumors'),
	'#rows' => 10,
	'#required' => true,
	'#default_value' => variable_get('message_klant_dutch'),
	);
	$form['email']['message_klant_engl'] = array(
	'#type'=> 'textarea',
	'#title' => t('Message custumors'),
	'#rows' => 10,
	'#required' => true,
	'#default_value' => variable_get('message_klant_engl'),
	);
	$form['email']['message_materiaal'] = array(
	'#type' => 'textarea',
	'#title' => t('Message material'),
	'#rows' => 10,
	'#required' => true,
	'#default_value'=> variable_get('message_materiaal'),
	'#description' => t('No params available'),
	);
	$form['email']['message_winagang'] = array(
	'#type' => 'textarea',
	'#title' => t('Message winagang'),
	'#rows' => 10,
	'#required' => true,
	'#default_value' => variable_get('message_winagang'),
	);
	$form['email']['message_reject'] = array(
	'#type' => 'textarea',
	'#title' => t('Message reject'),
	'#rows' => 10,
	'#required' => true,
	'#default_value' => variable_get('message_reject'),
	);
	$form['email']['message_approved'] = array(
	'#type' => 'textarea',
	'#title' => t('Message approved'),
	'#rows' => 10,
	'#required' => true,
	'#default_value' => variable_get('message_approved'),
	);
	return system_settings_form($form);
}
