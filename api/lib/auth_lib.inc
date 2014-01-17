<?php


/*
* API AUTH 
*/

global $auth;

function loadAPIAuth($driverType='json', $driverOptions=array()){
	
	if ($driverType == 'json'){
		require_once('./lib/auth/auth.php');
		require_once('./lib/auth/json_driver.php');
		$auth = new json_auth_driver($driverOptions);

	} elseif ($driverType == 'SQL'){
		require_once('./lib/auth/auth.php');	
		require_once('./lib/auth/sql_driver.php');
		$auth = new sql_auth_driver($driverOptions);
		
	}
}