#ifndef DJAH_UTILS_RANDOMIZER_HPP
#define DJAH_UTILS_RANDOMIZER_HPP

namespace djah { namespace utils {

	class randomizer
	{
	public:

		static int			random(int min, int max);
		static int			random(int max);
		static float		random(float min, float max);
		static float		random(float max);

		static void			set_seed(unsigned int seed);
		static unsigned int get_seed();

	private:

		static bool			initialized_;
		static unsigned int seed_;

		static void			init_seed();
	};

} /*utils*/ } /*djah*/

#endif /* DJAH_UTILS_RANDOMIZER_HPP */