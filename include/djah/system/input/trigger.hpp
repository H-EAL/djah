#ifndef DJAH_SYSTEM_INPUT_TRIGGER_HPP
#define DJAH_SYSTEM_INPUT_TRIGGER_HPP

#include <string>

namespace djah { namespace system { namespace input {

	class trigger
	{
	public:
		trigger(const std::string &_name, unsigned int _id, unsigned int _maxValue, const unsigned int _threshold)
			: name_(_name)
			, id_(_id)
			, value_(0.0f)
			, maxValue_(float(_maxValue))
			, threshold_(float(_threshold))
		{
		}
		~trigger() {}

		unsigned int		id()			const	{ return id_;			}
		const std::string&	name()			const	{ return name_;			}
		float				rawValue()		const	{ return value_;		}
		float				value()			const
		{
			const float v = value_ / maxValue_;
			return value_ >= threshold_ ? v : 0.0f;
		}

		void				setRawValue(float _value)	{ value_ = _value;	}

	private:
		unsigned int	id_;
		std::string		name_;
		float			value_;
		float			maxValue_;
		float			threshold_;
	};

} /*input*/ } /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_INPUT_TRIGGER_HPP */