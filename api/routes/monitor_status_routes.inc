<?php

/**
 Monitoring Routes and status routes
*/

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

$app->get('/calls',function() use ($app) {
	$output = eslCommand("show calls");
	$tiers = eslParser($output);
    if (!is_null($tiers)){
        $app->render(200,array('msg' => $tiers,));
    } else {
        $app->render(400, array('msg' => 'ESL failed to produce usable data', 'error' => true));
    }
});

$app->get('/channels',function() use ($app) {
	$output = eslCommand("show channels");
	$tiers = eslParser($output);
    if (!is_null($tiers)){
        $app->render(200,array('msg' => $tiers,));
    } else {
        $app->redner(400,array('msg' => 'ESL failed to produce usable data', 'error' => true));
    }
});

$app->get('/status',function() use ($app) {
	$output = eslCommand("status");
	$callers = eslParser($output);
    if (!is_null($callers)){
        $app->render(200,array(
                'msg' => $callers,
        ));
    } else {
        $app->render(400, array('msg'=> 'ESL failed to produce usable data', 'error' => true));
    }
});
