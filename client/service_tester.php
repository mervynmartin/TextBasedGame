<html>
	<head>
		
		<link type="text/css" href="css/ui-lightness/jquery-ui-1.8.21.custom.css" rel="stylesheet" />

		<style type="text/css">
			
			.userlogin-label { width: 5em; display: inline-block; }
			
		</style>


		<script src="js/jquery-1.7.2.min.js"></script>
		<script src="js/jquery-ui-1.8.21.custom.min.js"></script>
		<script src="js/jquery.json-2.3.min.js"></script>
		
		<script type="text/javascript">
			
			var serviceUrl = "http://localhost:80/TextBasedGame/service/";
			
			
			
			$(function(){
						
				$("#loginDialog").dialog({
					width: 800,
					height: 570,
					buttons: {
						"OK": function() {
							$.ajax({
								url: serviceUrl + "runservice.php",
								type: $("#method").val(),
								dataType: "json",
								data: {service: $("#service").val(), data: $("#data").val() },
								success: function(obj){
									$("#result").val(jQuery.toJSON(obj));								
								},
								error: function(xhr,status, err){
									alert("something done fucked up");								
								}
							});

						}
					}
				});	
			
			});
			
		</script>
		
	</head>
	<body>
		<div id="loginDialog">
			<div style="width: 370px; display: inline-block; vertical-align: top;">
				<form>
					<label class="service-label" for="service">Service:</label><br />
					<input type="text" name="service" id="service"/ style="width: 100%"><br />
					<label class="method-label" for ="method">Method:</label><br />
					<select id="method" name="method" style="width:100%">
						<option value="get">GET</option>
						<option value="post">POST</option>
						<option value="put">PUT</option>
						<option value="delete">DELETE</option>
					</select><br />
					<label class="data-label" for"data">Data:</label><br />
					<textarea id="data" style="width: 100%; height:300px;"></textarea>
				</form>
			</div>
			<div style="width: 370px; display: inline-block; vertical-align: top;">
				<label class="return-label" for="return">Results</label><br />
				<textarea id="result" style="width:100%; height:395px;">Returned content ideally</textarea>
			</div>

		</div>
		
	</body>
</html>