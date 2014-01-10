<?php

/**
Freeswitch User Library
**/

function getUserByExtension($exten, $delimiter = null){
	$user_list = eslCommand('list_users');
	$users = eslParser($user_list);
	if (!is_null($delimiter)){
		$exten = current(explode($delimiter, $exten));
	}
	foreach($users as $user){
		if ($user['userid'] == $exten){
			$name = explode('<',$user['effective_caller_id_name']);
			return $name[0];
		}
	}
	return 'User: ' . $exten . ' Does Not Exist';
}

function setUserName($data){
  if (count($data)) {
    $output = array();
	foreach($data as $exten){
	    $name = getUserByExtension($exten['name'], '@');
	    $exten['username'] = $name;
	    $output[] = $exten;
	}
	return $output;
  }
}

function createExtension($data){
    global $access_key;
    if ($data['access_key'] == $access_key){
        if (!file_exists(FS_CONF_DIR.'/directory/'.FS_DOMAIN.'/'.$data['user_id'].'.xml')){
            $xmlw = new XMLWriter();
            $xmlw->openURI(FS_CONF_DIR.'/directory/'.FS_DOMAIN.'/'.$data['user_id'].'.xml');
            $xmlw->setIndent(true);
            $xmlw->startElement('include');
            $xmlw->startElement('user');
            $xmlw->writeAttribute('id', $data['user_id']);
            $xmlw->startElement ( 'params' );
            foreach($data['params'] as $pname => $pvalue){
                $xmlw->startElement('param');
                $xmlw->writeAttribute('name', $pname);
                $xmlw->writeAttribute('value', $pvalue);
                $xmlw->endElement();
            }
            $xmlw->endElement();
            $xmlw->startElement ( 'variables' );
            foreach( $data['variables'] as $vname => $vvalue) {
                $xmlw->startElement('variable');
                $xmlw->writeAttribute('name', $vname);
                $xmlw->writeAttribute('value', $vvalue);
                $xmlw->endElement();
            }
            $xmlw->endElement();
            $xmlw->endElement();
            $xmlw->endElement();
            $xmlw->flush();
            if (isset($data['action'])) {
                eslCommand($data['action']);
            }
            return true;
        } else {
            return false;
        }
    } else {
        return "Bad Access Key";
    }
}

function checkUserExists($user) {
    if (!file_exists(FS_CONF_DIR.'/directory/'.FS_DOMAIN.'/'.$user.'.xml')){
        return false;
    } else {
        return true;
    }
}

function fetchUserData($user) {
    $reader = new XMLReader();
    $reader->open(FS_CONF_DIR.'/directory/'.FS_DOMAIN.'/'.$user.'.xml');
    while ($reader->read()) {
        $data = $reader->expand();
    }
    $data = json_encode($data);
    $output = json_decode($data, true);
}