#ifndef DJAH_UTILS_SINGLETON_HPP
#define DJAH_UTILS_SINGLETON_HPP

//--------------------------------------------------------------------------------------------------
#define DJAH_MAKE_SINGLETON(CLASS) \
	friend class djah::utils::singleton<CLASS>
//--------------------------------------------------------------------------------------------------

namespace djah { namespace utils {

	template<typename T>
	class singleton
	{
	public:
		static void create()
		{
			if( pInstance_ == nullptr )
			{
				pInstance_ = new T;
				std::atexit( destroy );
			}
		}

		static T& get()
		{
			create();
			return *pInstance_;
		}

		static T* get_ptr()
		{
			return &get();
		}

		static void destroy()
		{
			delete pInstance_;
			pInstance_ = nullptr;
		}

	protected:
		// Only derived classes can access ctor/dtor
        singleton() = default;
        ~singleton() = default;

	private:
		// No copy
        singleton(const singleton &) = delete;
        void operator =(const singleton &) = delete;

		// Unique instance
		static T *pInstance_;
	};

	//----------------------------------------------------------------------------------------------
	template<typename T> T *singleton<T>::pInstance_ = nullptr;
	//----------------------------------------------------------------------------------------------

} /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_SINGLETON_HPP */