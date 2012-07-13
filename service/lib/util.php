<?php

class DatabaseController
{
	
	private $server;
	private $username;
	private $password;
	private $database;
	
	public function __construct()
	{
		$server = ApplicationConfig::getDefaultValue("db_server");
		$username = ApplicationConfig::getDefaultValue("db_username");
		$password = ApplicationConfig::getDefaultValue("db_password");
		$database = ApplicationConfig::getDefaultValue("db_name");		
	} 
	
	public function getDbCon()
	{
		$mysqli = new mysqli("localhost", "webapp", "Ppabew!", "ffe");
		
		if (mysqli_connect_errno()) {
    		exit();
		}
	}
	
	
	
}


class LoginController
{

    public static function login($_username, $_password)
    {
		
		$dbcon = new DatabaseController();	
		$mysqli = $dbcon->getDbCon();	
		
		$salt = "test";	
		
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


class ApplicationConfig
{
			
	public static function getDefaultValue($_key)
	{
		switch($_key)
		{
			case 'content_type':
				return 'json';
				break;
			case 'db_server':
				return 'localhost';
				break;
			case 'db_user':
				return 'webapp';
				break;
			case 'db_password':
				return 'Ppabew!';
				break;
			case 'db_name':
				return 'ffe';
				break;
		}
		 
	}
}

?>
