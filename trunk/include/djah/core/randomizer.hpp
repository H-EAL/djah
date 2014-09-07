#ifndef DJAH_CORE_RANDOMIZER_HPP
#define DJAH_CORE_RANDOMIZER_HPP

namespace djah {

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

} /*djah*/

#endif /* DJAH_CORE_RANDOMIZER_HPP */