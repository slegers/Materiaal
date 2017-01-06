<?php

function _materiaal_mail($key,&$message,$params){
	$reservatie = materiaal_reservatie_load($params['reservatie_id']);
	
	//params is gedef in reservatie.inc
	switch($key){
	  case 'aanvraag_klant':
		$params_html['!link'] =  url('materiaal/validate_reservatie',array('absolute' => TRUE));
	                $body = variable_get('message_klant_dutch');
	                $message['subject'] = 'Aanvraag materiaal';
	                $message['html'][] = strtr($body,$params_html);

	  break;
	  case'aanvraag_materiaal':

		$params_html['!link'] =  url('materiaal/validate_reservatie',array('absolute' => TRUE));
		$body = variable_get('message_materiaal');
		$message['subject'] = 'Aanvraag bevestigen';
		$message['html'][] = strtr($body,$params_html);

	  break;
	  case 'aanvraag_goedgekeurd':
		$lijst = '<table style="width:100%">';
		
		foreach(materiaal_get_gereserveerd_materiaal($reservatie->reservatie_id) as $ding){
		 	$lijst.= '<tr><td><strong>' .$ding->naam . '</strong></td></tr>';
		}
		$lijst .= '</table>';
		$params_html['!list'] = $lijst;
		$params_html['!pickup'] = '<strong>' . date('Y-m-d H:i',$reservatie->tijdstip_afhalen). '</strong>';
		$params_html['!return'] = '<strong>' .date('Y-m-d H:i',$reservatie->tijdstip_teruggave) . '</strong>';
		$body = variable_get('message_approved');
		$persoon = materiaal_get_persoon($reservatie->persoon_id);
		$params_html['!klant'] = $persoon-> voornaam . ' ' .$persoon->achternaam;
		$params_html['!link'] = url('sites/default/files/Contract.pdf',array('absolute' => TRUE));
		$params_html['!waarborg'] = materiaal_price_to_euro(materiaal_get_waarborg($reservatie->reservatie_id));
                $params_html['!prijs'] =  materiaal_price_to_euro(materiaal_get_prijs($reservatie->reservatie_id,$persoon->persoon_id));	
		$message['subject'] = 'Aanvraag goedgekeurd';
		$message['html'][] = strtr($body,$params_html);
	  break;
	  case 'aanvraag_afgekeurd':
		
		$body = variable_get('message_reject');
		$persoon = materiaal_get_persoon($reservatie->persoon_id);
		$params_html['!reject'] = '<i>' . $params['reject_reason'] . '</i>';
		$params_html['!klant'] = '<strong>' . $persoon-> voornaam . ' ' .$persoon->achternaam . '</strong>';
		$message['subject'] = 'Aanvraag geweigerd';
		$message['html'][] = strtr($body,$params_html);

	  break;
	  case 'aanvraag_gang':
		$lijst = '<table style="width:100%">';
		foreach(materiaal_get_gereserveerd_materiaal($reservatie->reservatie_id) as $ding){
			$lijst.= '<tr><td><strong>' .$ding->naam. '</strong></td></tr>';
		}
		$lijst .= '</table>';
		$params_html['!list'] = $lijst;
		$body = variable_get('message_winagang');
		$params_html['!pickup'] = '<strong>'.date('Y-m-d H:i',$reservatie->tijdstip_afhalen) .'</strong>';
		$params_html['!return'] = '<strong>' . date('Y-m-d H:i',$reservatie->tijdstip_teruggave) . '</strong>';
		$persoon = materiaal_get_persoon($reservatie->persoon_id);
		$params_html['!klant'] =  '<b>'. $persoon-> voornaam . ' ' .$persoon->achternaam . '</b>';
		$params_html['!waarborg'] = materiaal_price_to_euro(materiaal_get_waarborg($reservatie->reservatie_id));
		$params_html['!prijs'] =  materiaal_price_to_euro(materiaal_get_prijs($reservatie->reservatie_id,$persoon->persoon_id));
		$message['subject'] = 'Aanvraag goedgekeurd';
		$message['html'][] = strtr($body,$params_html);
	  break;	 
	  default:
		dsm('Whoops something went wrong please contact the webadmins (webteam@wina.be)');
	  break;
	} 

}
