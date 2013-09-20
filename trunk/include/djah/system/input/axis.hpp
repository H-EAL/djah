#ifndef DJAH_SYSTEM_INPUT_AXIS_HPP
#define DJAH_SYSTEM_INPUT_AXIS_HPP

#include <string>

namespace djah { namespace system { namespace input {

	class axis
	{
	public:
		axis(const std::string &_name, unsigned int _id, unsigned int _minValue,
			 unsigned int _maxValue, const unsigned int _deadZone)
			: name_(_name)
			, id_(_id)
			, value_(0.0f)
			, minValue_(float(_minValue))
			, maxValue_(float(_maxValue))
			, deadZone_(float(_deadZone))
			, scaleCoeff_(2.0f / (_maxValue - _minValue))
		{
		}
		~axis() {}

		unsigned int		id()			const	{ return id_;			}
		const std::string&	name()			const	{ return name_;			}
		float				rawValue()		const	{ return value_;		}
		float				value()			const
		{
			// Set value between -1 and 1
			const float v = (value_ - minValue_) * scaleCoeff_ - 1.0f;
			return std::abs(v) < deadZone_ ? 0.0f : v;
		}

		void				setRawValue(float _value)	{ value_ = _value;	}

	private:
		unsigned int	id_;
		std::string		name_;
		float			value_;
		float			minValue_;
		float			maxValue_;
		float			deadZone_;
		float			scaleCoeff_;
	};

} /*input*/ } /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_INPUT_AXIS_HPP */