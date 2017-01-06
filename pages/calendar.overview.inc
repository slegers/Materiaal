<?php

function materiaal_kalender($form,$form_state){
	$form['kalender'] = array(
	'#type' => 'item',
	'#markup' => theme('materiaal_table',array('maand'=> date("F Y"))),
	);
	return $form;
}
