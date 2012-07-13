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
			
			var serviceUrl = "http://localhost:81/TextBasedGame/service/";
			
			
			
			$(function(){
				
				
				
				
				checkChatLines();
				
				
				
						
				function checkChatLines() {
					$.ajax({
						url: serviceUrl + "runservice.php",
						type: $("#method").val(),
						dataType: "json",
						data: {service: "chatlines", data: '{"lastchatline":"1"}' },
						success: function(obj){	processChatLines(obj.body); },
						error: function(xhr,status, err){ alert("something done fucked up");}
					});
				}
				
				function processChatLines(_obj)
				{
					$.each(_obj, function(i, item){
						lineformat = item.user  + ": " + item.message;
						$("#chat-content").append(
							$(document.createElement("div")).addClass("chat-content-line").text(lineformat)															
						);	
					});
					
					
				}
			
				
			
			
			});
			
		</script>
		
	</head>
	<body>

		<div style="position: fixed; top: 1%; width: 99%; height:93%; border: 1px solid black;">
			<div id="chat-content" style="position: absolute; bottom: 0; width: 100%;">
			</div>
		</div>

		<div id="chat-bar" style="position:fixed; bottom: 0; width: 100%; height: 6%;">
			<table style="width: 99%">
				<tr>
					<td style="width: 90%">
						<textarea id="chat-input" style="width: 100%; height: 100%;" ></textarea>
					</td>
					<td style="width: 10%">
						<input type="button" name="chat-input" value="Send" style="height:100%; width: 100%; "/>
					</td>
				</tr>
			</table>			
		</div>

		
	</body>
</html>