<?php

	

	include 'lib/services.inc';
	
	$response = new RestResponse();
	
	$response = Routes::routeRequest();

	
	header('Content-type: application/json', true, $response->getStatus());
	echo $response->getJSONEncode();

?>
