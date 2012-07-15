<?php

class DatabaseController
{

	public static function getDbCon()
	{
		try {
			$server = ApplicationConfig::getDefaultValue("db_server");
			$username = ApplicationConfig::getDefaultValue("db_username");
			$password = ApplicationConfig::getDefaultValue("db_password");
			$database = ApplicationConfig::getDefaultValue("db_name");	
			
			self::$pdo = new PDO("mysql:host=". $server.";dbname=". $database, 
								$username, 
								$password);
			self::$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			
			return self::$pdo;
			
		}
		catch(PDOException $e)
		{
			$pdo = null;
			return false;	
		}
		
		
	}	
}

class DB { 
    
    private static $objInstance; 
    
    /* 
     * Class Constructor - Create a new database connection if one doesn't exist 
     * Set to private so no-one can create a new instance via ' = new DB();' 
     */ 
    private function __construct() {} 
    
    /* 
     * Like the constructor, we make __clone private so nobody can clone the instance 
     */ 
    private function __clone() {} 
    
    /* 
     * Returns DB instance or create initial connection 
     * @param 
     * @return $objInstance; 
     */ 
    public static function getInstance(  ) {
 
        if(!self::$objInstance){ 
            self::$objInstance = new PDO("mysql:host=". ApplicationConfig::getDefaultValue("db_server").";".
            							 "dbname=". ApplicationConfig::getDefaultValue("db_username"), 
										 ApplicationConfig::getDefaultValue("db_username"), 
										 ApplicationConfig::getDefaultValue("db_password"));
            self::$objInstance->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION); 
        } 
        
        return self::$objInstance; 
    
    } # end method 
    
    /* 
     * Passes on any static calls to this class onto the singleton PDO instance 
     * @param $chrMethod, $arrArguments 
     * @return $mix 
     */ 
    final public static function __callStatic( $chrMethod, $arrArguments ) { 
            
        $objInstance = self::getInstance(); 
        
        return call_user_func_array(array($objInstance, $chrMethod), $arrArguments); 
        
    } # end method 
    
}



class LoginController
{

    public static function login($_username, $_password)
    {
		
		$dbh = new PDO("mysql:host=". ApplicationConfig::getDefaultValue("db_server").";".
            							 "dbname=". ApplicationConfig::getDefaultValue("db_name"), 
										 ApplicationConfig::getDefaultValue("db_user"), 
										 ApplicationConfig::getDefaultValue("db_password"));
        $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

		
		$salt = ApplicationConfig::getDefaultValue("salt");	
		
		$stmt = $dbh->prepare("SELECT id, hash
		          FROM user
		          WHERE username = :username");
		
		$params = array(":username"=>$_username);
		
		$stmt->execute($params);
		$result = $stmt->fetchAll();
		
		
		foreach($result as $row)
		{
			$db_userid = $row["id"];
			$db_hash = $row["hash"];	
		}
		
		$hash = hash('sha256', $salt . $_password . $salt);
		
		if($hash == $db_hash)
		{
			session_start();
		    LoginController::validateUser($db_userid);	
			return true;
		}
		else
		{
		    return false;
		}
		return false;
		
		$stmt->close();
		$pdo->close();

    }

    public static function validateUser($_userid)
    {
        session_regenerate_id ();
        $_SESSION['valid'] = true;
        $_SESSION['userid'] = $_userid;
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
			case 'salt':
				return 'test';
				break;
		}
		 
	}
}

?>
