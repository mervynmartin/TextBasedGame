<?php

class LoginController
{

    public static function login($username, $password)
    {

        //connect to the database here
        /*$username = mysql_real_escape_string($username);
        $query = "SELECT password, salt
                  FROM users
                  WHERE username = '$username';";

        $result = mysql_query($query);
        if(mysql_num_rows($result) < 1) //no such user exists
        {
            return false;
        }
        $userData = mysql_fetch_array($result, MYSQL_ASSOC);
        $hash = hash('sha256', $userData['salt'] . hash('sha256', $password) );
        if($hash != $userData['password'])
        {
            return false;
        }
        else
        {*/

            this::validateUser($username);
			
			return true;
        /*}*/


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
