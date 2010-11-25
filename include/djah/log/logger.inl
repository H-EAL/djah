namespace djah { namespace log {

	//--------------------------------------------------------------------------
	template<typename T>
	logger& logger::operator <<(const T &msg)
	{
		std::ostringstream stream;
		stream << msg;
		write(stream.str());
		return *instance_;
	}
	//--------------------------------------------------------------------------

} /*log*/ } /*djah*/