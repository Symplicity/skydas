<?php
chdir(__DIR__);
require_once('./settings.inc');
require_once('./vendor/autoload.php');

// Put all Call Center functions here
require_once('./lib/EventSocketLayer.php');
require_once('./lib/freeswitch_lib.inc');

if(ENABLE_MEMCACHE){
    require_once('./lib/memcache_lib.inc');
}

if (empty($access_key)){
    echo "SETUP IS NOT VALID YOU MUST SET $access_key IN settings.inc".PHP_EOL;
    exit;
}

if (!extension_loaded('ESL')) {
	if (!dl('ESL.so')){
		echo "You Must have the FreeSwitch ESL PHP Module Loaded".PHP_EOL;
		exit;
	}
}

function skydas_autoload($loader_name) {
    if (file_exits('./lib/loaders/'.$loader_name.'_loader'.php)){
        require_once('./lib/loaders/'.$loader_name.'_loader.php');
    } else {
        throw new Exception("Unable to load $loader_name");
    }
}

$app = new \Slim\Slim();
$app->view(new \JsonApiView());
$app->add(new \JsonApiMiddleware());

foreach($loaders as $loader){
    skydas_autoload($loader);
}

$app->run();
?>
