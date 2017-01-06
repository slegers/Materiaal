    <?php

function _materiaal_theme($existing, $type, $theme, $path){

  return array(
	'materiaal_table' => array(
	  'variables' => array(
	    'maand' => NULL,
	  ),
	 'file'=> 'materiaal.theme.inc',	
       ),
       'materiaal_ding' => array(
	  'variables' => array(
	    'ding_id' => NULL,
	  ),
	 'file' => 'materiaal.theme.inc',
	),
      );
}

function theme_materiaal_table($variables){
   drupal_add_css(drupal_get_path('module','materiaal') . '/materiaal.css',array('type' => 'file'));
      $output = '<div class="he">' . $variables['maand'] . '</div>';
       
     return $output;
}
