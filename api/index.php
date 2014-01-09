<?php
chdir(__DIR__);
require_once('./settings.inc');
require_once('./vendor/autoload.php');

// Put all Call Center functions here
require_once('./lib/callcenter_lib.inc');

$app = new \Slim\Slim();
$app->view(new \JsonApiView());
$app->add(new \JsonApiMiddleware());

$app->get('/user/:exten',function($exten) use ($app) {
	$user = getUserByExtension($exten);
        $app->render(200,array('msg' => $user));
});

$app->get('/:queue/agents',function($queue) use ($app) {
	if (validateQueue($queue)){
		$output = eslCommand("callcenter_config queue list agents $queue@".FS_DOMAIN);	
		$output2 = eslCommand("callcenter_config queue list tiers $queue@".FS_DOMAIN);
		$agents = eslParser($output);
		$tiers = eslParser($output2);
		$agents = setUserName($agents);
		$agents = agentListBuilder($agents,$tiers);
		$app->render(200,array('msg' => $agents));
	} else {
		$app->render(200,array('msg' => 'Invalid Queue Name'));
	}
});

$app->get('/:queue/tiers',function($queue) use ($app) {
	if (validateQueue($queue)){
		$output = eslCommand("callcenter_config queue list tiers $queue@".FS_DOMAIN);
		$tiers = eslParser($output);
	    $app->render(200,array(
	                'msg' => $tiers,
	    ));
	} else {
		$app->render(200,array('msg' => 'Invalid Queue Name'));
	}
});

$app->get('/tiers',function() use ($app) {
	$output = eslCommand("callcenter_config tier list");
	$tiers = eslParser($output);
    $app->render(200,array('msg' => $tiers,));
});

$app->get('/calls',function() use ($app) {
	$output = eslCommand("show calls");
	$tiers = eslParser($output);
    $app->render(200,array('msg' => $tiers,));
});

$app->get('/channels',function() use ($app) {
	$output = eslCommand("show channels");
	$tiers = eslParser($output);
    $app->render(200,array('msg' => $tiers,));
});

$app->get('/:queue/callers',function($queue) use ($app) {
	if (validateQueue($queue)){
		$output = eslCommand("callcenter_config queue list members $queue@".FS_DOMAIN);
		$callers = eslParser($output);
        $callers_2 = array();
		foreach($callers as $call){
            $callers_output[] = queueCallerParse($call);
		}
        $app->render(200,array('msg' => $callers_output,));
	} else {
		$app->render(200,array('msg' => 'Invalid Queue Name'));
	}
});

$app->get('/status',function() use ($app) {
	$output = eslCommand("status");
	$callers = eslParser($output);
        $app->render(200,array(
                'msg' => $callers,
        ));
});

$app->get('/queues/all',function() use ($app) {
	$queue_list = getQueues();
	foreach($queue_list as $queue){
		$queues[$queue] = queueAllData($queue);
	}
      	$app->render(200,array('msg'=>$queues,));
});

$app->get('/validate/:queue',  function($queue) use ($app){
    if (validateQueue($queue)){
	    $app->render(200,array('msg' => array("validate" => "true")));
    } else {
	    $app->render(200,array('msg' => array("validate" => "false")));
    }
});

$app->post('/connect', function () use ($app){
    $req = $app->request();
    $connect = json_decode($req->getBody());
    error_log(print_r($connect,true));
});

$app->post('/voicemail/greeting',function() use ($app) {
   global $access_key;
   $req = $app->request();
   $connect = json_decode($req->getBody(),true);
   if ($connect['access_key'] == $access_key){
       error_log("Command: ".'vm_fsdb_pref_greeting_set default voip.symplicity.com '.$connect['exten'].' '.$connect['greeting_number']);
       $output = eslCommand('vm_fsdb_pref_greeting_set default voip.symplicity.com '.$connect['exten'].' '.$connect['greeting_number']);
       $app->render(200,array('msg'=>'greeting updated'));
   } else {
        $app->render(400,array('msg'=>'Access Denied','error'=>true));
   }
});

$app->post('/user/create', function() use ($app) {
    global $access_key;
    $req = $app->request();
    $user = json_decode($req->getBody(),true);
    if ($user['access_key'] == $access_key){
        if (createExtension($user)) {
            $app->render(200,array('msg'=>'User was created'));
        } else {
            $app->render(400,array('msg'=> "Failed to create user", 'error'=>true));
        }
    } else {
        $app->render(400,array('msg'=> 'access denied', 'error'=>true));
   }
});

$app->get('/user/fetch/:exten', function($exten) use ($app) {
    global $access_key;
    $req = $app->request();
    $vars = json_decode($req->getBody(),true);
    if ($vars['access_key'] == $access_key) {
        if (checkUserExists($exten)){
            $user = fetchUserData($exten);
            $app->render(200,array('msg' => $user));
        } else {
            $app->render(400,array('msg' => 'Sorry Extension Does not Exist', 'error' => true));
        }
    } else {
        $app->render(400,array('msg' => 'access denied', 'error' => true));
    }

});

$app->get('/tools/caller/:password/:cid', function($password,$cid) use ($app) {
    $parts = explode('/',$_SERVER['REQUEST_URI']);
    $cid = $parts[5];
    if ($parts[4] == 'passwordhere') {
        if(preg_match('/^(\+1|1)?[2-9]\d\d[2-9]\d{6}$/',$cid)){
                $call_deets = cidLookup($cid);
                $string = $call_deets->cnam .' '. $call_deets->number;
                $app->render(200,array('msg' => $string));
        } else {
                $string = "Invalid Number".PHP_EOL;
                $app->render(400,array('msg' => $string, 'error' => true));
        }
    } else {
        $app->render(400,array('msg' => 'access denied', 'error' => true));
    }

});


// Get Monitor Stats for showing Call Count and Channel Counts
// We use this in our Monitor System
$app->get('/monitor/:stat', function($stat) use ($app) {

    switch($stat) {
        case 'calls':
            $command = 'show calls count';
            $data = eslCommand($command);
            $data = explode(' ', $data);
            $results = trim($data[0]);
            break;;
        case 'channels':
            $command = 'show channels count';
            $data = eslCommand($command);
            $data = explode(' ', $data);
            $results = trim($data[0]);
            break;;
        default:
            $app->render(400,array('msg' => 'You Must pass approved monitor command', 'error'=>true));
            break;
    }

    if (isset($command)){
        $app->render(200,array('msg'=>$results));
    }

});

$app->run();
?>
