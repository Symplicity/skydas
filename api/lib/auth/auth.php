<?php

/*
  Auth Driver Base Class
*/

class Auth {
	
	function checkApiKey($apiKey){
		return $this->query($apiKey);
	}

}