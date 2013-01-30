function twoDigits(value)
{
   if(value < 10) {
    return '0' + value;
   }
   return value;
}

function formatLog()
{
	var timestamps = document.getElementsByName('timestamp');
	for( i=0; i < timestamps.length; ++i )
	{
		var date = new Date();
		date.setTime(timestamps[i].innerHTML * 1000);
		timestamps[i].innerHTML = twoDigits(date.getHours()) + ':' + twoDigits(date.getMinutes()) + ':' + twoDigits(date.getSeconds());
	}
	
	var stacktrace = document.getElementsByName('stacktrace');
	for( i=0; i < stacktrace.length; ++i )
	{
		stacktrace[i].innerHTML = stacktrace[i].innerHTML.replace(/\n/g, '<br/>');
	}
	
	var messages = document.getElementsByName('message');
	for( i=0; i < messages.length; ++i )
	{
		messages[i].innerHTML = messages[i].innerHTML.replace(/\n/g, '<br/>');
	}
	
}