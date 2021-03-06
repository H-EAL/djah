#include "djah/core/randomizer.hpp"

#include <ctime>
#include <cstdlib>
#include <algorithm>

namespace djah {

	//----------------------------------------------------------------------------------------------
	bool			randomizer::initialized_	= false;
	unsigned int	randomizer::seed_			= 0;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	int randomizer::random(int min, int max)
	{
		if( !initialized_ )
			init_seed();

		if( min > max )
			std::swap(min, max);

		return rand()%(max-min+1) + min;
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

		if( min > max )
			std::swap(min, max);

		return static_cast<float>(rand())/static_cast<float>(RAND_MAX) * (max-min) + min;
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

} /*djah*/