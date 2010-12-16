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

		static T& get_instance()
		{
			if( instance_ == 0 )
				instance_ = new T;
			return *instance_;
		}

		static T* get_instance_ptr()
		{
			return &get_instance();
		}

		static void destroy()
		{
			delete instance_;
			instance_ = 0;
		}

	protected:

		// Only derived classes can access ctor/dtor
		singleton() {}
		virtual ~singleton() { if(instance_) delete instance_; }

	private:

		// No copy
		singleton(const singleton &);
		void operator =(const singleton &);

		// Unique instance
		static T *instance_;
	};

	//----------------------------------------------------------------------------------------------
	template<typename T> T *singleton<T>::instance_ = 0;
	//----------------------------------------------------------------------------------------------

} /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_SINGLETON_HPP */