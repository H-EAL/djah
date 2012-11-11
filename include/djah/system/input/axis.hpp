#ifndef DJAH_SYSTEM_INPUT_AXIS_HPP
#define DJAH_SYSTEM_INPUT_AXIS_HPP

#include <string>

namespace djah { namespace system { namespace input {

	class axis
	{
	public:
		axis(const std::string &name, unsigned int id, unsigned int min_value, unsigned int max_value)
			: name_(name)
			, id_(id)
			, value_(0.0f)
			, min_value_(static_cast<float>(min_value))
			, max_value_(static_cast<float>(max_value))
			, scale_coeff_(2.0f / (max_value - min_value))
		{
		}
		~axis() {}

		unsigned int		id()			const	{ return id_;					}
		const std::string&	name()			const	{ return name_;					}
		float				rawValue()		const	{ return value_;				}
		float				correctedValue(float deadZone = 0.0f) const
		{
			// Set value between -1 and 1
			const float v = (value_ - min_value_) * scale_coeff_ - 1.0f;
			return std::abs(v) < deadZone ? 0.0f : v;
		}

		void				setValue(float value)	{ value_ = value;				}

	private:
		unsigned int	id_;
		std::string		name_;
		float			value_;
		float			min_value_;
		float			max_value_;
		float			scale_coeff_;
	};

} /*input*/ } /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_INPUT_AXIS_HPP */