<?php

/**
	Call Routes
*/

$app->post('/connect', function () use ($app){
    $req = $app->request();
    $connect = json_decode($req->getBody());
});

$app->post('/voicemail/greeting',function() use ($app) {
   global $access_key;
   $req = $app->request();
   $connect = json_decode($req->getBody(),true);
   if ($connect['access_key'] == $access_key){
       $output = eslCommand('vm_fsdb_pref_greeting_set default voip.symplicity.com '.$connect['exten'].' '.$connect['greeting_number']);
       $app->render(200,array('msg'=>'greeting updated'));
   } else {
        $app->render(400,array('msg'=>'Access Denied','error'=>true));
   }
});

$app->get('/tools/caller/:password/:cid', function($password,$cid) use ($app) {
    $parts = explode('/',$_SERVER['REQUEST_URI']);
    $cid = $parts[5];
    if ($parts[4] == FS_CID_PASSWORD) {
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