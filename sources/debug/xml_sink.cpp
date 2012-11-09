#include "debug/xml_sink.hpp"
#include "debug/assertion.hpp"

namespace djah { namespace debug {

	//----------------------------------------------------------------------------------------------
	xml_sink::xml_sink(const std::string &filePath, const std::string &channels)
		: basic_sink(channels)
		, filePath_(filePath)
	{
		fileStream_.open(filePath.c_str(), std::ios::out);
		DJAH_ASSERT( fileStream_.good() );
		fileStream_ << "<?xml version='1.0' encoding='utf-8'?>\n";
		fileStream_ << "<?xml-stylesheet type='text/xsl' href='log-style.xsl'?>\n";
		fileStream_ << "<logs>\n";
		fileStream_ << "    <channels>" << channels << "</channels>\n";
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	xml_sink::~xml_sink()
	{
		fileStream_ << "</logs>";
		fileStream_.close();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void xml_sink::consume(const basic_record &rec)
	{
		fileStream_
			<< "    <record>\n"
			<< "        <channel>"			<< rec.channels()	<< "</channel>\n"
			<< "        <severity>"			<< rec.severity()	<< "</severity>\n"
			<< "        <line>"				<< rec.line()		<< "</line>\n"
			<< "        <file>"				<< rec.file()		<< "</file>\n"
			<< "        <timestamp>"		<< rec.timestamp()	<< "</timestamp>\n"
			<< "        <stacktrace>"		<< rec.stackTrace()	<< "</stacktrace>\n"
			<< "        <message><![CDATA["	<< rec.message()	<< "]]></message>\n"
			<< "    </record>\n";
	}
	//----------------------------------------------------------------------------------------------

} /*debug*/ } /*djah*/