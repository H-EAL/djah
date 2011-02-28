#include "utils/randomizer.hpp"

#include <ctime>
#include <cstdlib>

namespace djah { namespace utils {

	//----------------------------------------------------------------------------------------------
	bool			randomizer::initialized_	= false;
	unsigned int	randomizer::seed_			= 0;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	int randomizer::random(int min, int max)
	{
		if( !initialized_ )
			init_seed();

		if ( min <= max )
			return rand()%(max-min+1) + min;
		else
			return rand()%(min-max+1) + max;
	}
	//----------------------------------------------------------------------------------------------
	int randomizer::random(int max)
	{
		return random(0, max);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	float randomizer::random(float min, float max)
	{
		if( !initialized_ )
			init_seed();

		if( min <= max )
			return static_cast<float>( rand() )/static_cast<float>(RAND_MAX) * (max-min) + min;
		else
			return static_cast<float>( rand() )/static_cast<float>(RAND_MAX) * (min-max) + max;
	}
	//----------------------------------------------------------------------------------------------
	float randomizer::random(float max)
	{
		return random(0.0f, max);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void randomizer::set_seed(unsigned int seed)
	{
		seed_ = seed;
		srand(seed_);
		initialized_ = true;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	unsigned int randomizer::get_seed()
	{
		return seed_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void randomizer::init_seed()
	{
		set_seed( static_cast<unsigned int>(time(0)) );
	}
	//----------------------------------------------------------------------------------------------

} /*utils*/ } /*djah*/