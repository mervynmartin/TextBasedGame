<?php
class character
{
	
	public $id;
	public $name;
	public $characterClass;
	public $stat_sheet;
	
	public function __construct($_characterId)
	{
		$this->id = $_characterId;
		$this->name = 'TITUS THE BARBARIAN';
		$this->class = new characterClass(1);
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
		$this->id = $_classId;		
	}

}

class classAbilityList
{
	

}

class characterAbilityList
{
	
	
}

?>