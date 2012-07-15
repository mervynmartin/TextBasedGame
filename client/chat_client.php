<html>
	<head>
		
		<link type="text/css" href="css/ui-lightness/jquery-ui-1.8.21.custom.css" rel="stylesheet" />

		<style type="text/css">
			
			.chat-content-line { width: 100%; border: 1px solid black; }
			
		</style>


		<script src="js/jquery-1.7.2.min.js"></script>
		<script src="js/jquery-ui-1.8.21.custom.min.js"></script>
		<script src="js/jquery.json-2.3.min.js"></script>
		
		<script type="text/javascript">
			
			var serviceUrl = "../service/";
			
			
			
			$(function(){
				
				lastChatLine = -1;
				
				function checkChatLines() {
					$.ajax({
						url: serviceUrl + "runservice.php",
						type: "get",
						dataType: "json",
						data: {service: "chatlines", data: '{"lastchatline":"' + lastChatLine + '"}' },
						success: function(obj) { processChatLines(obj.body); },
						statuscode: {
							401: function(){ clearInterval(chatTimer); }
						},
						error: function(xhr,status, err){ alert("something done fucked up");}
					});
				}
				
				function processChatLines(_obj)
				{
					$.each(_obj, function(i, item){
						if(parseInt(item.chatLineContents.id) > parseInt(lastChatLine))
						{
							lastChatLine = parseInt(item.chatLineContents.id);
							lineformat = item.chatLineContents.user  + ": " + item.chatLineContents.message;
							$("#chat-content").append(
								$(document.createElement("div")).addClass("chat-content-line").text(lineformat)															
							);
						}	
					});
				}
				
				function sendChatLine()
				{
					message = $("#chat-input").val();					
					$.ajax({
						url: serviceUrl + "runservice.php",
						type: "put",
						dataType: "json",
						data: {service: "chatlines", data: '{"message":"' + message + '"}'},
						success: function(obj) { $("#chat-input").val(""); },
						error: function (xhr, status, err) { alert("could not send message"); }
					})
				}
			
				$("#chat-input-form").submit(function(e){
					e.preventDefault();
					sendChatLine();
				});
				
				
				$("#loginDialog").dialog({
					width: 400,
					buttons: {
						"OK": function() {
							$.ajax({
								url: serviceUrl + "runservice.php",
								type: "POST",
								dataType: "json",
								data: {service: 'login', data: '{"username": "' + $("#username").val() + '", "password": "' + $("#password").val() + '"}' },
								success: function(){
									checkChatLines();
									chatTimer = setInterval(function(){checkChatLines()},1000);
									$("#loginDialog").dialog("close");
								},
								statuscode: {
									401: function(obj){
										$("#login-status").text("Login Failed");
									}
								},
								error: function(xhr,status, err){
									alert("something done fucked up");								
								}
							});
						},
					}
				});	
				
			
			
			});
			
		</script>
		
	</head>
	<body>

		<div id="loginDialog">
			<form id="login-input-form">
				<label class="userlogin-label" for="username">Username:</label>
				<input type="text" name="username" id="username"/><br />
				<label class="userlogin-label" for ="password">Password:</label>
				<input type="password" name="password" id="password" />
				<div id="login-status"></div>
			</form>
		</div>


		<div style="position: fixed; top: 1%; width: 99%; height:93%; border: 1px solid black;">
			<div id="chat-content" style="position: absolute; bottom: 0; width: 100%;">
			</div>
		</div>

		<div id="chat-bar" style="position:fixed; bottom: 0; width: 100%; height: 6%;">
			<form id="chat-input-form">
				<table style="width: 99%">
					<tr>
						<td style="width: 90%">
							<input type="text" id="chat-input" style="width: 100%; height: 100%;" />
						</td>
						<td style="width: 10%">
							<input type="button" id="chat-input-send" name="chat-input-send" value="Send" style="height:100%; width: 100%; "/>
						</td>
					</tr>
				</table>
			</form>
		</div>

		
	</body>
</html>