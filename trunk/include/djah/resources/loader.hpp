#ifndef DJAH_RESOURCES_LOADER_HPP
#define DJAH_RESOURCES_LOADER_HPP

#define DJAH_DECLARE_LOADER(ASSET_TYPE)     \
namespace djah { namespace resources {      \
                                            \
	template<>                              \
    class loader<ASSET_TYPE>                \
	    : public ASSET_TYPE## _loader       \
    {                                       \
        typedef ASSET_TYPE AssetType;       \
    };                                      \
                                            \
} /*resources*/ } /*djah*/


namespace djah { namespace resources {

	template<typename T>
	class loader;

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_LOADER_HPP */