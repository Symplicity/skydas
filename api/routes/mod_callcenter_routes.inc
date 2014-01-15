<?php
/*
Mod_callcenter API Routes
*/

$app->get('/:queue/agents',function($queue) use ($app) {
	if (validateQueue($queue)){
		$output = eslCommand("callcenter_config queue list agents $queue@".FS_DOMAIN);	
		$output2 = eslCommand("callcenter_config queue list tiers $queue@".FS_DOMAIN);
		$agents = eslParser($output);
		$tiers = eslParser($output2);
        if (!is_null($agents) || !is_null($tiers)){
		    $agents = setUserName($agents);
		    $agents = agentListBuilder($agents,$tiers);
		    $app->render(200,array('msg' => $agents));
        } else {
            $app->render(400,array('msg' => 'ESL Failed to produce usable data', 'error' => true));
        }
	} else {
		$app->render(200,array('msg' => 'Invalid Queue Name'));
	}
});

$app->get('/:queue/tiers',function($queue) use ($app) {
	if (validateQueue($queue)){
		$output = eslCommand("callcenter_config queue list tiers $queue@".FS_DOMAIN);
		$tiers = eslParser($output);
        if (!is_null($tiers)){
	        $app->render(200,array(
	                    'msg' => $tiers,
	        ));
        } else {
            $app->render(400,array('msg'=>'ESL faild to produce usable data', 'error' => true));
        }
	} else {
		$app->render(200,array('msg' => 'Invalid Queue Name'));
	}
});

$app->get('/queues/fetch',function() use ($app) {
	$queues = getQueues(); 
	foreach($queues as $v){
    	$output[$v] = array('agents' => new StdClass,'callers' => new StdClass);
	}
//	$output = json_encode($q, JSON_FORCE_OBJECT);
    if (!is_null($output)){
        $app->render(200,array('msg' => $output,));
    } else {
        $app->render(400, array('msg' =>'ESL Failed to produce usable data', 'error' => true));
    }
});

$app->get('/queues/tiers',function() use ($app) {
	$output = eslCommand("callcenter_config tier list");
	$tiers = eslParser($output);
    if (!is_null($tiers)){
        $app->render(200,array('msg' => $tiers,));
    } else {
        $app->render(400, array('msg' =>'ESL Failed to produce usable data', 'error' => true));
    }
});

$app->get('/:queue/callers',function($queue) use ($app) {
	if (validateQueue($queue)){
		$output = eslCommand("callcenter_config queue list members $queue@".FS_DOMAIN);
		$callers = eslParser($output);
        $callers_2 = array();
        if (!is_null($callers)){
		    foreach($callers as $call){
                $callers_output[] = queueCallerParse($call);
		    }
            $app->render(200,array('msg' => $callers_output,));
        } else {
            $app->render(400,array('msg','ESL Failed to Return Any Data','error'=>true));
        }
	} else {
		$app->render(200,array('msg' => 'Invalid Queue Name'));
	}
});

$app->get('/queues/all',function() use ($app) {
	$queue_list = getQueues();
	foreach($queue_list as $queue){
		$queues[$queue] = queueAllData($queue);
	}
    if (!empty($queues) || is_array($queues)){
    $app->render(200,array('msg'=>$queues,));
    } else {
        $app->render(400, array('msg' => 'Error Getting Queue Data', 'error' => true));
    }
});

$app->get('/validate/:queue',  function($queue) use ($app){
    if (validateQueue($queue)){
	    $app->render(200,array('msg' => array("validate" => "true")));
    } else {
	    $app->render(200,array('msg' => array("validate" => "false")));
    }
});

$app->get('/queues/status/:agent',  function($agent) use ($app){
    $status = eslCommand("callcenter_config agent get status ".$agent.'@'.FS_DOMAIN);
    $state = eslCommand("callcenter_config agent get state ".$agent.'@'.FS_DOMAIN);
    if (!is_null($status) || !is_null($state)){
    	$app->render(200,array('msg' => array('state' => $state, 'status' => $status)));
    } else {
    	$app->render(500,array('msg' => 'Recieved invalid results from agent status request','error'=>true));
    }
});

$app->post('/queues/dnd/:agent/:state', function($agent,$state) use ($app){
	if ($state == 'on'){
		$output = eslCommand("callcenter_config agent set status ".$agent.'@'.FS_DOMAIN." 'On Break'");
		$output = eslCommand("callcenter_config agent set state ".$agent.'@'.FS_DOMAIN." 'Idle'");
		if (!is_null($output)){
			$app->render(200,array('msg' => 'DND is Set for Agent'));
		} else {
			$app->render(500,array('msg' => 'Unable to set DND for Agent', 'error' => true));
		}
	} elseif ($state == 'off'){
		$output = eslCommand("callcenter_config agent set status ".$agent.'@'.FS_DOMAIN." 'Available'");
		$output = eslCommand("callcenter_config agent set state ".$agent.'@'.FS_DOMAIN." 'Waiting'");
		if (!is_null($output)){
			$app->render(200,array('msg' => 'DND is Turned off for Agent'));
		} else {
			$app->render(500,array('msg' => 'Unable to disable DND for Agent', 'error' => true));
		}
	}
});
