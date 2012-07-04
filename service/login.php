<?php

header('Content-type: application/json');

include 'lib\LoginController.inc';

	$username = isset($_POST['username']) ? $_POST['username'] : '';
	$password = isset($_POST['password']) ? $_POST['password'] : ''; 
	

	if(LoginController::login($username, $password))
	{
		echo '[{"loginSuccessful": 1}]';	
	}
	else
	{
		echo '[{"loginSuccessful": 0}]';			
	}
?>