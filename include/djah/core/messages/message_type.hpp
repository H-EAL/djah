#ifndef DJAH_MESSAGES_MESSAGE_TYPE_HPP
#define DJAH_MESSAGES_MESSAGE_TYPE_HPP

namespace djah { namespace messages {

	class message_type
	{
	public:
		message_type(const std::string &type);

	private:
		std::string type_;
	};

} /*messages*/ }/*djah*/

#endif /* DJAH_MESSAGES_MESSAGE_TYPE_HPP */