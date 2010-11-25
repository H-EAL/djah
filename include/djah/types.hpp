#ifndef DJAH_TYPES_HPP
#define DJAH_TYPES_HPP

#include <boost/cstdint.hpp>

namespace djah {

	//  8 bits types
	typedef	signed		char				s8;
	typedef unsigned	char				u8;
	// 16 bits types
	typedef	signed		short				s16;
	typedef unsigned	short				u16;
	// 32 bits types
	typedef	signed		int					s32;
	typedef unsigned	int					u32;
	typedef				float				f32;
	// 64 bits types
	typedef				boost::int64_t		s64;
	typedef				boost::uint64_t		u64;
	typedef				double				f64;

	// Words
	typedef	u8	 byte;
	typedef u16	 word;
	typedef u32	dword;
	typedef u64	qword;

	// Helpers
	namespace detail {

		template<typename T> struct shift;
		template<>			 struct shift< byte> { static const int value =  8; };
		template<>			 struct shift< word> { static const int value = 16; };
		template<>			 struct shift<dword> { static const int value = 32; };

		template<typename ToType, typename FromType>
		inline ToType concat_bytes(FromType low, FromType high)
		{
			return static_cast<ToType>(low | ((ToType)high) << shift<FromType>::value);
		}

		template<typename ToType, typename FromType>
		inline ToType low_bytes(FromType bytes)
		{
			return static_cast<ToType>(bytes);
		}

		template<typename ToType, typename FromType>
		inline ToType high_bytes(FromType bytes)
		{
			return static_cast<ToType>(bytes >> shift<ToType>::value) & (ToType)((u32)(-1));
		}
	}

	// Words baking
	inline  word make_word	( byte high,  byte low) { return detail::concat_bytes< word>(low, high);	}
	inline dword make_dword	( word high,  word low) { return detail::concat_bytes<dword>(low, high);	}
	inline qword make_qword	(dword high, dword low) { return detail::concat_bytes<qword>(low, high);	}

	// Low bytes
	inline  byte low_byte	( word  w)				{ return detail::low_bytes< byte>	( w);			}
	inline  word low_word	(dword dw)				{ return detail::low_bytes< word>	(dw);			}
	inline dword low_byte	(qword qw)				{ return detail::low_bytes<dword>	(qw);			}

	// High bytes
	inline  byte high_byte	( word  w)				{ return detail::high_bytes< byte>	( w);			}
	inline  word high_word	(dword dw)				{ return detail::high_bytes< word>	(dw);			}
	inline dword high_byte	(qword qw)				{ return detail::high_bytes<dword>	(qw);			}

} /*djah*/

#endif /* DJAH_TYPES_HPP */