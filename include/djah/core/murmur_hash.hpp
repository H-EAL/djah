#ifndef DJAH_UTILS_MURMUR_HASH_HPP
#define DJAH_UTILS_MURMUR_HASH_HPP

#include "../types.hpp"

namespace djah { namespace utils {

	//----------------------------------------------------------------------------------------------
	static const u32 _m = 0x5bd1e995;
	static const u32 _r = 24;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template <size_t LEN>
	inline u32 murmur_hash_len(u32 hash, const char* str, int idx)
	{
		return murmur_hash_len<LEN-4>( murmur_hash_len<4>(hash, str, idx), str, idx+4);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template <>
	inline u32 murmur_hash_len<4>(u32 hash, const char* str, int idx)
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
	inline u32 murmur_hash_len<3>(u32 hash, const char* str, int idx)
	{
		return (hash ^ (str[idx+2] << 16) ^ (str[idx+1] << 8) ^ (str[idx+0])) * _m;
	}
	//----------------------------------------------------------------------------------------------
	template <>
	inline u32 murmur_hash_len<2>(u32 hash, const char* str, int idx)
	{
		return (hash ^ (str[idx+1] << 8) ^ (str[idx+0])) * _m;
	}
	//----------------------------------------------------------------------------------------------
	template <>
	inline u32 murmur_hash_len<1>(u32 hash, const char* str, int idx)
	{
		return (hash ^ (str[idx+0])) * _m;
	}
	//----------------------------------------------------------------------------------------------
	template <>
	inline u32 murmur_hash_len<0>(u32 hash, const char* str, int idx)
	{
		return (hash);
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template <size_t LEN>
	inline u32 murmur_hash(u32 seed, const char* str)
	{
		u32 h = murmur_hash_len<LEN>(seed ^ LEN, str, 0);
		h ^= h >> 13;
		h *= _m;
		h ^= h >> 15;
		return h;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	inline u32 murmur_hash(const void *key, u32 len, u32 seed)
	{
		// 'm' and 'r' are mixing constants generated offline.
		// They're not really 'magic', they just happen to work well.

		// Initialize the hash to a 'random' value
		u32 h = seed ^ len;

		// Mix 4 bytes at a time into the hash
		const u32 * data = (u32 *)key;

		while(len >= 4)
		{
			u32 k = *(u32 *)data;

			k *= _m; 
			k ^= k >> _r;
			k *= _m; 

			h *= _m; 
			h ^= k;

			data += 4;
			len -= 4;
		}

		// Handle the last few bytes of the input array
		switch(len)
		{
		case 3: h ^= data[2] << 16;
		case 2: h ^= data[1] << 8;
		case 1: h ^= data[0];
			h *= _m;
		};

		// Do a few final mixes of the hash to ensure the last few
		// bytes are well-incorporated.
		h ^= h >> 13;
		h *= _m;
		h ^= h >> 15;

		return h;
	} 


	//----------------------------------------------------------------------------------------------
	// This generates an immediate hash with a literal string. Do NOT use with a dynamic string.
	#define djah_static_murmur_hash(STR) djah::utils::murmur_hash<sizeof(STR)>(0, STR)
	//----------------------------------------------------------------------------------------------
	inline u32 dyamic_murmur_hash(const char *str, u32 len, u32 seed = 0)
	{
		return murmur_hash(str, len, seed);
	}
	//----------------------------------------------------------------------------------------------

} /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_MURMUR_HASH_HPP */