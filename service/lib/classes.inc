<?php
class character
{
	
	private $id;
	private $name;
	private $characterClass;
	private $stat_sheet;
	
	public function __construct($_characterId)
	{
		$id = $_characterId;
		$name = 'TITUS THE BARBARIAN';
		$class = new characterClass(1);
	}
	
	public function getJSONEncode()
	{
		return json_encode(get_object_vars($this));		
	}
	
	
}

class characterClass
{
	private $id;
	
	public function __construct($_classId)
	{
		$id = $_classId;		
	}

}

class classAbilityList
{
	

}

class characterAbilityList
{
	
	
}

?>