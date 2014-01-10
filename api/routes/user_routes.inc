<?php

/**
	User Routes
*/

$app->get('/user/:exten',function($exten) use ($app) {
	$user = getUserByExtension($exten);
        $app->render(200,array('msg' => $user));
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