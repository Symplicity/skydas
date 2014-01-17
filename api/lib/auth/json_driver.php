<?php


/**
	Basic JSON API Auth Driver
*/

class json_auth_driver extends Auth {
	
	function __construct($driver_options = array()){
		if (!is_array($driver_options)){
			throw exception ('Must pass driver_options as a array');
		}
		foreach ($driver_options as $k => $v){
			$this->$k = $v;
		}
	}

	function load_flat_file(){
		$this->auth_file = file_get_contents($this->filename);
	}

	function query($api_key){
		$this->load_flat_file();
		$api_auth_data = json_decode($this->auth_file,true);

		if ($api_auth_data['key'] == $api_key){
			return true;
		} else {
			return false;
		}
	}

}