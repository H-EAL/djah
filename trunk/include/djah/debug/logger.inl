namespace djah { namespace debug {

	//--------------------------------------------------------------------------
	template<typename T>
	logger_impl& logger_impl::operator <<(const T &msg)
	{
		std::ostringstream stream;
		stream << msg;
		write(stream.str());
		return *this;
	}
	//--------------------------------------------------------------------------

} /*debug*/ } /*djah*/