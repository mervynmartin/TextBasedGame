<?php


include "lib/classes.inc";
include "lib/util.inc";


class RestUtils
{
	public static function getStatusCodeMessage($status)
	{
		// these could be stored in a .ini file and loaded
		// via parse_ini_file()... however, this will suffice
		// for an example
		$codes = Array(
		    100 => 'Continue',
		    101 => 'Switching Protocols',
		    200 => 'OK',
		    201 => 'Created',
		    202 => 'Accepted',
		    203 => 'Non-Authoritative Information',
		    204 => 'No Content',
		    205 => 'Reset Content',
		    206 => 'Partial Content',
		    300 => 'Multiple Choices',
		    301 => 'Moved Permanently',
		    302 => 'Found',
		    303 => 'See Other',
		    304 => 'Not Modified',
		    305 => 'Use Proxy',
		    306 => '(Unused)',
		    307 => 'Temporary Redirect',
		    400 => 'Bad Request',
		    401 => 'Unauthorized',
		    402 => 'Payment Required',
		    403 => 'Forbidden',
		    404 => 'Not Found',
		    405 => 'Method Not Allowed',
		    406 => 'Not Acceptable',
		    407 => 'Proxy Authentication Required',
		    408 => 'Request Timeout',
		    409 => 'Conflict',
		    410 => 'Gone',
		    411 => 'Length Required',
		    412 => 'Precondition Failed',
		    413 => 'Request Entity Too Large',
		    414 => 'Request-URI Too Long',
		    415 => 'Unsupported Media Type',
		    416 => 'Requested Range Not Satisfiable',
		    417 => 'Expectation Failed',
		    500 => 'Internal Server Error',
		    501 => 'Not Implemented',
		    502 => 'Bad Gateway',
		    503 => 'Service Unavailable',
		    504 => 'Gateway Timeout',
		    505 => 'HTTP Version Not Supported'
		);

		return (isset($codes[$status])) ? $codes[$status] : '';
	}
}




class RestResponse
{
	private $status;
	private $body;
	private $content_type;
	
	public function __construct()
	{
		$this->status		= 200;
		$this->body			= RestUtils::getStatusCodeMessage($this->status);
		$this->content_type	= ApplicationConfig::getDefaultValue('content_type');
	}

	public function setBody($_body)
	{
		$this->body = $_body;
	}
	
	public function setStatus($_status)
	{
		$this->status = $_status;
		$this->body = RestUtils::getStatusCodeMessage($this->status);		
	}
	
	public function getStatus()
	{
		return $this->status;		
	}
	
	public function getJSONEncode()
	{
		return json_encode(get_object_vars($this));		
	}
	
}


class RestRequest
{
	private $request_vars;
	private $data;
	private $http_accept;
	private $method;

	public function __construct()
	{
		$this->request_vars		= array();
		$this->data				= '';
		$this->http_accept		= (strpos($_SERVER['HTTP_ACCEPT'], 'json')) ? 'json' : 'xml';
		$this->method			= 'get';
	}

	
	public function getJSONEncode()
	{
		return json_encode(get_object_vars($this));		
	}

	public function setData($data)
	{
		$this->data = $data;
	}

	public function setMethod($method)
	{
		$this->method = $method;
	}

	public function setRequestVars($request_vars)
	{
		$this->request_vars = $request_vars;
	}

	public function getData()
	{
		return $this->data;
	}

	public function getMethod()
	{
		return $this->method;
	}

	public function getHttpAccept()
	{
		return $this->http_accept;
	}

	public function getRequestVars()
	{
		return $this->request_vars;
	}
}


class Routes 
{
	
	public static function routeRequest()
	{
		
		$restRequest	= new RestRequest();
		$restResponse 	= new RestResponse();
		$data			= array();
		
		$allOk 			= true;
		$evalService 	= "";
		$evalMethod 	= "";
		$requestMethod 	= "";
		$service		= "";
		
		$requestMethod = strtolower($_SERVER['REQUEST_METHOD']);
		
		switch($requestMethod)
		{
			case 'get':
				$data = $_GET;
				$evalMethod = 'get';
				break;
			case 'post':
				$data = $_POST;
				$evalMethod = 'post';
				break;
			case 'put':
				// basically, we read a string from PHP's special input location,
				// and then parse it out into an array via parse_str... per the PHP docs:
				// Parses str  as if it were the query string passed via a URL and sets
				// variables in the current scope.
				parse_str(file_get_contents('php://input'), $put_vars);
				$data = $put_vars;
				$evalMethod = 'put';
				break;
			case 'delete':
				$evalMethod = 'delete';
				break;
			case 'default':
				$allOk = false;
				$responseStatus = 400; //bad request
				exit();
				break;
		}
		
		$restRequest->setMethod($requestMethod);
		$restRequest->setRequestVars($data);
		if(isset($data['data']))
		{
			$restRequest->setData(json_decode($data['data']));
		}


		if(isset($data['service']))
		{
			$service = $data['service'];
		}
		
		
		

		//put this back in some day when login is working correctly.

		//if(!LoginController::checkLogin())
		//{
		//	$restResponse->setStatus(401); //not authorized
		//	$allOk = false;
		//}
	

		if ($allOk)
		{
			$serviceMethod = $evalMethod . $service;
			
			if (method_exists('services', $serviceMethod))
			{
				
				$restResponse = call_user_func("services::" . $serviceMethod, $restRequest);
			} 
			else
			{
				$responseStatus = 404; //not found
				$restResponse->setStatus($responseStatus);	
			}	
		}
		
		//send $restRepsonse to client
		return $restResponse;
				
	}
	

}



class Services
{
	
	public static function getCharacter($_restRequest)
	{

		$id =  $_restRequest->getData()->{'id'};
		
		$restResponse = new RestResponse();
		$character = new Character($id);
		
		
		$restResponse->setBody($character);
		
		return $restResponse;

	}
	
	public static function postLogin($_restRequest) //should be post in future
	{

		$restResponse = new RestResponse();
		
		$username = $_restRequest->getData()->{'username'};
		$password = $_restRequest->getData()->{'password'};
	
		if(LoginController::login($username, $password))
		{
			$kvp = Array(
				"loginSuccessful" => "1"
			);
			
			$restResponse->setBody($kvp);	
		}
		else
		{
			$restResponse->setStatus(401);
		}
		
		return $restResponse;
	}
	
	public static function putLogin($_restRequest)
	{
		
		$restResponse = new RestResponse();	
		
		session_start();
		$username = $_restRequest->getData()->{'username'};
		$password = $_restRequest->getData()->{'password'};
		$hash = hash('sha256', ApplicationConfig::getDefaultValue("salt") . $password . ApplicationConfig::getDefaultValue("salt")); 

		$dbh = new PDO("mysql:host=". ApplicationConfig::getDefaultValue("db_server").";".
						 "dbname=". ApplicationConfig::getDefaultValue("db_name"), 
						 ApplicationConfig::getDefaultValue("db_user"), 
						 ApplicationConfig::getDefaultValue("db_password"));
        $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		
		$stmt = $dbh->prepare("INSERT INTO user (username, hash) VALUES (:username, :hash)");
		$stmt->bindParam(":username", $username, PDO::PARAM_STR);
		$stmt->bindParam(":hash", $hash, PDO::PARAM_STR);
		
		$stmt->execute();
		
		return $restResponse;
		
	}
	
	public static function getChatLines($_restRequest)
	{
		
		$restResponse = new RestResponse();
		
		$lastChatLine = $_restRequest->getData()->{'lastchatline'};
		
		$chatlineCollection = Array();				
		
		try
		{

			$dbh = new PDO("mysql:host=". ApplicationConfig::getDefaultValue("db_server").";".
            							 "dbname=". ApplicationConfig::getDefaultValue("db_name"), 
										 ApplicationConfig::getDefaultValue("db_user"), 
										 ApplicationConfig::getDefaultValue("db_password"));
            $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

			$stmt = $dbh->prepare("SELECT chat.*, user.username FROM chat JOIN user ON chat.user = user.id WHERE chat.id > :id");
			$stmt->bindParam(":id", $lastChatLine, PDO::PARAM_INT);
			
			$stmt->execute();
			
			$result = $stmt->fetchAll();
			
			
			foreach($result as $row)
			{
				$chatlinecontents = new chatLineContents();
				$chatlinecontents->id = $row["id"];
				$chatlinecontents->user = $row["username"];
				$chatlinecontents->message = $row["message"];
				$chatlinecontents->createdate = $row["createdate"];
			
				$chatline = new chatline();
				$chatline->chatLineContents = $chatlinecontents;
				
				array_push($chatlineCollection, $chatline);
									
			}
			
			$dbh = null;
			
			$restResponse->setBody($chatlineCollection);
			
			return $restResponse;

		}
		catch(PDOException $e)
		{
			$restResponse->setStatus(500);
			$restResponse->setBody(array("error"=>$e));
			
			return $restResponse;
		}
		
		
		
	}
	
	public static function putChatLines($_restRequest)
	{
		
		$restResponse = new RestResponse();
		
		//if(LoginController::checkLogin())
		//{
				
			session_start();
			$message = $_restRequest->getData()->{'message'};
			$userid = $_SESSION["userid"]; //get user id at some point somehow
			
			
			$dbh = new PDO("mysql:host=". ApplicationConfig::getDefaultValue("db_server").";".
							 "dbname=". ApplicationConfig::getDefaultValue("db_name"), 
							 ApplicationConfig::getDefaultValue("db_user"), 
							 ApplicationConfig::getDefaultValue("db_password"));
            $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			
			$stmt = $dbh->prepare("INSERT INTO chat (user, message) VALUES (:user, :message)");
			$stmt->bindParam(":user", $userid, PDO::PARAM_INT);
			$stmt->bindParam(":message", $message, PDO::PARAM_INT);
			
			$stmt->execute();
			
		//}
		//else {
		//	$restResponse->setStatus(401);
		//}
		
		return $restResponse;
		
	}
	
}


class chatLine
{
	public $chatLineContents;
	
	function __construct()
	{
		$chatLineContents = new chatLineContents();
	}
}

class chatLineContents
{
	public $id;
	public $user;
	public $message;
	public $createdate;
}




?>