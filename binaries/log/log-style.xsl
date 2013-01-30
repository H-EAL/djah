<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">
	<html>
		<head>
			<link rel="stylesheet" type="text/css" href="log-style.css" title="Default Style"/>
			<script type="text/javascript" src="log-script.js"></script>
		</head>
		<body onLoad="formatLog();">
			<h2>Log (<xsl:value-of select="logs/channels" />)</h2>
			<table id="log_table">
				<tr>
					<th>Time</th>
					<th>Severity</th>
					<th>Channel</th>
					<th>File (line)</th>
					<th>Call Stack</th>
					<th>Message</th>
				</tr>
				
				<xsl:for-each select="logs/record">
				<tr>
					<td name="timestamp"><xsl:value-of select="timestamp"/></td>
					<td name="severity"><xsl:value-of select="severity"/></td>
					<td name="channel"><xsl:value-of select="channel"/></td>
					<td name="file"><xsl:value-of select="file"/> (<xsl:value-of select="line" />)</td>
					<td name="stacktrace"><xsl:value-of select="stacktrace"/></td>
					<td name="message"><xsl:value-of select="message"/></td>
				</tr>
				</xsl:for-each>
			</table>
		</body>
	</html>
</xsl:template>
</xsl:stylesheet>

