<?php

class LoginController
{

    public static function login($_username, $_password)
    {
		$mysqli = new mysqli("localhost", "webapp", "Ppabew!", "ffe");

		$salt = "test";
		
		if (mysqli_connect_errno()) {
    		exit();
		}
			
		//connect to the database here
		//$username = mysql_real_escape_string($_username);
		$stmt = $mysqli->prepare("SELECT hash
		          FROM user
		          WHERE username = ?");
				  
		$stmt->bind_param('s', $_username);
		
		$stmt->execute();
		$stmt->bind_result($db_hash);
		$stmt->fetch();
		/*
		$result = $stmt->get_result();
		

		if($result->num_rows < 1) //no such user exists
		{
		    return false;
		}*/
		
		$hash = hash('sha256', $salt . $_password . $salt);
		
		if($hash == $db_hash)
		{
		    LoginController::validateUser($_username);	
			return true;
		}
		else
		{
		    return false;
		}
		return false;
		
		$stmt->close();
		$mysqli->close();

    }

    public static function validateUser($_username)
    {
        session_regenerate_id ();
        $_SESSION['valid'] = true;
        $_SESSION['user'] = $_username;
    }

    public static function logout()
    {
        $_SESSION = array(); //destroy all of the session variables
        session_destroy();
    }

    public static function checkLogin()
    {
        if(isset($_SESSION['valid']) && $_SESSION['valid'])
            return true;
        return false;
    }



}
