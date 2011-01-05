#ifndef DJAH_UTILS_MURMUR_HASH_HPP
#define DJAH_UTILS_MURMUR_HASH_HPP

#include "../types.hpp"

namespace djah { namespace utils {

	//----------------------------------------------------------------------------------------------
	static const u32 _m = 0x5bd1e995;
	static const u32 _r = 24;
	//----------------------------------------------------------------------------------------------
/*
	//----------------------------------------------------------------------------------------------
	template <size_t LEN>
	u32 murmur_hash_len(u32 hash, const char* str, int idx)
	{
		return murmur_hash_len<LEN-4>( murmur_hash_len<4>(hash, str, idx), str, idx+4);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template <>
	u32 murmur_hash_len<4>(u32 hash, const char* str, int idx)
	{
		#define _k  ((str[idx+0]) | ((str[idx+1])<<8) | ((str[idx+2]) << 16) | ((str[idx+3]) << 24))
		u32 k;
		k = _k * _m;
		k ^= (k >> _r);
		k *= _m;
		return (hash * _m) ^ k;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template <>
	u32 murmur_hash_len<3>(u32 hash, const char* str, int idx)
	{
		return (hash ^ (str[idx+2] << 16) ^ (str[idx+1] << 8) ^ (str[idx+0])) * _m;
	}
	//----------------------------------------------------------------------------------------------
	template <>
	u32 murmur_hash_len<2>(u32 hash, const char* str, int idx)
	{
		return (hash ^ (str[idx+1] << 8) ^ (str[idx+0])) * _m;
	}
	//----------------------------------------------------------------------------------------------
	template <>
	u32 murmur_hash_len<1>(u32 hash, const char* str, int idx)
	{
		return (hash ^ (str[idx+0])) * _m;
	}
	//----------------------------------------------------------------------------------------------
	template <>
	u32 murmur_hash_len<0>(u32 hash, const char* str, int idx)
	{
		return (hash);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template <size_t LEN>
	u32 murmur_hash(u32 seed, const char* str)
	{
		u32 h = murmur_hash_len<LEN>(seed ^ LEN, str, 0);
		h ^= h >> 13;
		h *= _m;
		h ^= h >> 15;
		return h;
	}
	//----------------------------------------------------------------------------------------------
*/

	//----------------------------------------------------------------------------------------------
	template<u32 hash, char *str, int idx, u32 len>
	struct murmur_hash_len
	{
		enum { Result = murmur_hash_len<murmur_hash_len<hash, str, idx, 4>, str, idx+4, len-4> };
	};
	//----------------------------------------------------------------------------------------------
	template<u32 hash, char *str, int idx>
	struct murmur_hash_len<hash, str, idx, 4>
	{
		#define _k  ((str[idx+0]) | ((str[idx+1])<<8) | ((str[idx+2]) << 16) | ((str[idx+3]) << 24))
		enum
		{
			Tmp0	= _k * _m,
			Tmp1	= Tmp0 >> _r,
			Tmp2	= Tmp0 ^ Tmp1,
			k		= Tmp2 * _m,
			Result	= (hash * _m) ^ k
		};
	};
	//----------------------------------------------------------------------------------------------
	template<u32 hash, char *str, int idx>
	struct murmur_hash_len<hash, str, idx, 3>
	{
		enum { Result = (hash ^ (str[idx+2] << 16) ^ (str[idx+1] << 8) ^ (str[idx+0])) * _m };
	};
	//----------------------------------------------------------------------------------------------
	template<u32 hash, char *str, int idx>
	struct murmur_hash_len<hash, str, idx, 2>
	{
		enum { Result = (hash ^ (str[idx+1] << 8) ^ (str[idx+0])) * _m };
	};
	//----------------------------------------------------------------------------------------------
	template<u32 hash, char *str, int idx>
	struct murmur_hash_len<hash, str, idx, 1>
	{
		enum { Result = (hash ^ (str[idx+0])) * _m };
	};
	//----------------------------------------------------------------------------------------------
	template<u32 hash, char *str, int idx>
	struct murmur_hash_len<hash, str, idx, 0>
	{
		enum { Result = hash };
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<u32 seed, char *str>
	struct murmur_hash
	{
		enum
		{
			Tmp0	= sizeof(str),
			Tmp1	= murmur_hash_len<seed ^ Tmp0, str, 0, Tmp0>::Result,
			Tmp2	= Tmp1 >> 13,
			Tmp3	= Tmp1 ^ Tmp2,
			Tmp4	= Tmp3 * _m,
			Tmp5	= Tmp4 >> 15,
			Tmp6	= Tmp4 ^ Tmp5,
			Result	= Tmp6
		};
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// This generates an immediate hash with a literal string. Do NOT use with a dynamic string.
	#define DJAH_STATIC_MURMUR_HASH(str, seed) djah::utils::murmur_hash<seed, str>::Result
	//----------------------------------------------------------------------------------------------

} /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_MURMUR_HASH_HPP */