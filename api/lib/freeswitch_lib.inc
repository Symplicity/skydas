<?php

function eslCommand($command) {
  $esl = new ESLconnection(FS_ESL_HOST, FS_ESL_PORT, FS_ESL_PASSWORD);
  $e = $esl->api($command);
  if (is_object($e)) {
    $esl->disconnect();
    return $e->getBody();
  }
  $esl->disconnect();
}

function eslParser($input) {
	$i = 0;
	$lines = explode("\n",$input);
	foreach ($lines as $line) {
		$data[$i++] = explode('|',$line);
	}
	if (count($data) > 1){
		$i = 0;
		$keys = $data[0];
		foreach($data as $points){
			if (count($keys) == count($points)){
				if ($keys !== $points){
					$output[$i++] = array_combine($keys,$points);
				}
			}
		}
	}
	return $output;	
}