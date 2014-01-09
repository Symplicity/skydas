<?php
include('settings.inc');
include('lib/callcenter_lib.inc');

$data = array(
    'access_key' => '',
    'user_id' => '1000',
    'params' => array(
        'password' =>'myawesomepassword',
        'vm-password' => '1234', 
        'vm-email-all-messages' => "true",
        'vm-notify-mailto' => "true", 
        'vm-mailto' => 'my@email.com',
        'vm-attach-file' => "true",
        'vm-message-ext' => 'wav',
        'directory-exten-visible' => 'true'
     ),
    'variables' => array(
        'toll_allow' => 'domestic,international,local', 
        'accountcode'=>'1000',
        'user_context' => 'default',
        'effective_caller_id_name' => 'Nick Claus',
        'effective_caller_id_number' => '1000',
        'outbound_caller_id_name' => '$${outbound_caller_name}',
        'outbound_caller_id_number' => '$${outbound_caller_id}',
        'directory_full_name' => 'Nick Claus'
    )
);

//echo json_encode($data);
echo createExtension($data);

?>
