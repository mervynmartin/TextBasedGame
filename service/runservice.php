<?php

	header('Content-type: application/json');

	include 'lib/services.inc';
	echo Routes::routeRequest();

?>
