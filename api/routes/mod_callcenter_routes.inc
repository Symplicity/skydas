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
	$queues = fetchQueues(); 
	foreach($queues as $k => $v){
    	$q[$v] = array('agents' => array(),'callers' => array());
	}
	$output = json_encode($q, JSON_FORCE_OBJECT);
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