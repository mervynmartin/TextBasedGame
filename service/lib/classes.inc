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