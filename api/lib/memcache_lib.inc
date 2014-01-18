<?php

/***
  memcache functions
*/

global $memc;

$memc = new Memcache;
$memc->connect(MEMCACHE_HOST, MEMCACHE_PORT);

function skycache_set($key,$value,$timeout) {
    global $memc;
	$memc->set($key,$value,MEMCACHE_COMPRESSED, 15);
}

function skycache_fetch($key) {
    global $memc;
	return $memc->get($key);
}
