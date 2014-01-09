<?php
require_once('./EventSocketLayer.php');

function eslCommand($command) {
  $esl = new ESLconnection(FS_ESL_HOST, FS_ESL_PORT, FS_ESL_PASSWORD);
  $e = $esl->api($command);
  if (is_object($e)) {
    return $e->getBody();
  }
}

function eslParser($input) {
	$i = 0;
	$lines = explode("\n",$input);
	foreach ($lines as $line) {
		$data[$i++] = explode('|',$line);
	}
	if (count($data) > 1){
		$i = 0;
		$keys = $data[0];
		foreach($data as $points){
			if (count($keys) == count($points)){
				if ($keys !== $points){
					$output[$i++] = array_combine($keys,$points);
				}
			}
		}
	}
	return $output;	
}

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

function agentListBuilder($agents, $tiers){
	$_agents = array();
	foreach($agents as $agent){
		$_agents[$agent['name']]['name'] = $agent['username'];
		$_agents[$agent['name']]['status'] = $agent['status'];
		$_agents[$agent['name']]['state'] = $agent['state'];
		$_agents[$agent['name']]['no_answer_count'] = $agent['no_answer_count'];
		$_agents[$agent['name']]['calls_answered'] = $agent['calls_answered'];
		$_agents[$agent['name']]['name'] = $agent['username'];
		$_agents[$agent['name']]['extension'] = $agent['name'];

	}
	foreach($tiers as $teir){
		$_agents[$teir['agent']]['queue'] = $teir['queue'];
		$_agents[$teir['agent']]['queue_state'] = $teir['state'];
		$_agents[$teir['agent']]['level'] = $teir['level'];
		$_agents[$teir['agent']]['position'] = $teir['position'];
	}
	return $_agents;
}

function queueAllData($queue){
    	$agents_raw = eslCommand("callcenter_config queue list agents $queue@default");
        $tiers_raw = eslCommand("callcenter_config queue list tiers $queue@default");
        $callers_raw = eslCommand("callcenter_config queue list members $queue@default");
        $agents_output = eslParser($agents_raw);
        $tiers_output = eslParser($tiers_raw);
        $callers_rawout = eslParser($callers_raw);
        foreach($callers_rawout as $caller) {
            $callers_output[] = queueCallerParse($caller);
        }
        $agents = setUserName($agents_output);
        $agents = agentListBuilder($agents,$tiers_output);
        $queues = array('agents'=>$agents,'callers'=>$callers_output);
	return $queues;
}

function getQueues() {
	$queues = explode(',',CALLCENTER_QUEUES);
	return $queues;
}

function validateQueue($queue) {
	$queues = getQueues();
	if (in_array($queue,$queues)){
		return true;
	} else {
		return false;
	}
}

function queueCallerParse($caller) {
    $call = array();

    foreach($caller as $k=>$v) {
        if ($k == 'bridge_epoch' && $v == '0') {
	  $holdtime = strtotime('now') - $caller['joined_epoch'];
	  $call['hold_time'] = floor($holdtime/60).':'.($holdtime % 60);
	  //error_log(print_r($call,true));
        } elseif ($k == 'serving_agent'){
            $call['serving_agent'] = getUserByExtension($v,'@');
        }
        $call[$k] = $v;
    }
    return $call;

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
    //$data = simplexml_load_file(FS_CONF_DIR.'/directory/'.FS_DOMAIN.'/'.$user.'.xml');
    $reader = new XMLReader();
    $reader->open(FS_CONF_DIR.'/directory/'.FS_DOMAIN.'/'.$user.'.xml');
    while ($reader->read()) {
        $data = $reader->expand();
    }
    var_dump($data);
    $data = json_encode($data);
    var_dump($data);
    $output = json_decode($data, true);
    //var_dump($output);
}

function cidLookup($cid) {
      $q = CIDNAME_HOST.$cid.'/'.CIDNAME_APIT_KEY.CIDNAME_OPTIONS;
      $result = file_get_contents($q);
      $result = json_decode($result);
      return $result;
}
