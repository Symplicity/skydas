<?php
chdir(__DIR__);
require_once('./settings.inc');
require_once('./vendor/autoload.php');

// Put all Call Center functions here
require_once('./lib/EventSocketLayer.php');
require_once('./lib/freeswitch_lib.inc');

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

$app = new \Slim\Slim();
$app->view(new \JsonApiView());
$app->add(new \JsonApiMiddleware());

if(ENABLE_MOD_CALLCENTER) {
    require_once('./lib/callcenter_lib.inc');
    require_once('./routes/mod_callcenter_routes.inc');
}

if (ENABLE_MONITOR_ROUTES) {
    require_once('./routes/monitor_status_routes.inc');
}

if (ENABLE_USER_ROUTES) {
    require_once('./lib/user_lib.inc');
    require_once('./routes/user_routes.inc');
}

if (ENABLE_CALL_ROUTES) {
    require_once('./lib/call_lib.inc');
    require_once('./routes/call_routes.inc');
}

$app->run();
?>
