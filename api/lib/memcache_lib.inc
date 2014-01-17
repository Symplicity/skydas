<?php

/***
  memcache functions
*/

global $memc;

$memc = memcache_object(MEMCHACHE_HOST, MEMCACHE_PORT);

function memcache_set($key,$value,$timeout) {
	$memc->set($key,$value,MEMCACHE_COMPRESSED, 15);
}

function memcache_fetch($key) {
	return $memc->get($key);
}