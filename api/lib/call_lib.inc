<?php

/**
Call Library
*/

function cidLookup($cid) {
      $q = CIDNAME_HOST.$cid.'/'.CIDNAME_APIT_KEY.CIDNAME_OPTIONS;
      $result = file_get_contents($q);
      $result = json_decode($result);
      return $result;
}

function connectCall($user_uuid, $caller_uuid){
	
}