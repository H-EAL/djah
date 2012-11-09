#ifndef DJAH_TYPES_HPP
#define DJAH_TYPES_HPP

namespace djah {

	//  8 bits types
	typedef	signed		char		s8;
	typedef unsigned	char		u8;
	// 16 bits types
	typedef	signed		short		s16;
	typedef unsigned	short		u16;
	// 32 bits types
	typedef	signed		int			s32;
	typedef unsigned	int			u32;
	typedef				float		f32;
	// 64 bits types
	typedef signed		long long	s64;
	typedef	unsigned	long long	u64;
	typedef				double		f64;

	// Words
	typedef	u8	 byte;
	typedef u16	 word;
	typedef u32	dword;
	typedef u64	qword;

	// Helpers
	namespace detail {

		template<typename T> struct shift { enum { value = sizeof(T) * 8 }; };

		template<typename ToType, typename FromType>
		inline ToType concat_bytes(FromType high, FromType low)
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
			return static_cast<ToType>(bytes >> shift<ToType>::value) & static_cast<ToType>(-1);
		}

	} /*detail*/


	// Words baking
	inline  word make_word	( byte high,  byte low) { return detail::concat_bytes< word>(high, low);	}
	inline dword make_dword	( word high,  word low) { return detail::concat_bytes<dword>(high, low);	}
	inline qword make_qword	(dword high, dword low) { return detail::concat_bytes<qword>(high, low);	}

	// Low bytes
	inline  byte low_byte	( word  w)				{ return detail::low_bytes< byte>	( w);			}
	inline  word low_word	(dword dw)				{ return detail::low_bytes< word>	(dw);			}
	inline dword low_dword	(qword qw)				{ return detail::low_bytes<dword>	(qw);			}

	// High bytes
	inline  byte high_byte	( word  w)				{ return detail::high_bytes< byte>	( w);			}
	inline  word high_word	(dword dw)				{ return detail::high_bytes< word>	(dw);			}
	inline dword high_dword	(qword qw)				{ return detail::high_bytes<dword>	(qw);			}

} /*djah*/

#endif /* DJAH_TYPES_HPP */