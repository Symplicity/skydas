<?php

/**
Mod_callcenter library

All Mod_Callcenter API information should be put in here. All Functions, etc
*/

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
    if (ENABLE_MEMCACHE){
       $queue = skycache_fetch('queues-all'); 
    }
    if (empty($queue)){
    	$agents_raw = eslCommand("callcenter_config queue list agents $queue@".FS_DOMAIN);
        $tiers_raw = eslCommand("callcenter_config queue list tiers $queue@".FS_DOMAIN);
        $callers_raw = eslCommand("callcenter_config queue list members $queue@".FS_DOMAIN);
        $agents_output = eslParser($agents_raw);
        $tiers_output = eslParser($tiers_raw);
        $callers_rawout = eslParser($callers_raw);
        if(is_array($callers_rawout)){
            foreach($callers_rawout as $caller) {
               $callers_output[] = queueCallerParse($caller);
            }
            $agents = setUserName($agents_output);
            $agents = agentListBuilder($agents,$tiers_output);
            $queues = array('agents'=>$agents,'callers'=>$callers_output);
        } else {
            $queues = null;
        }
        if (ENABLE_MEMCACHE) {
            skycache_set($queue,$queues,15)
        }
    }
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
        } elseif ($k == 'serving_agent'){
            $call['serving_agent'] = getUserByExtension($v,'@');
        }
        $call[$k] = $v;
    }
    return $call;
}
