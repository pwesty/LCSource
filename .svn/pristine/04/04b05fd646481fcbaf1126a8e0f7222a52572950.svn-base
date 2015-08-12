#include "random_machine.h"
#include <time.h>

template <typename Distribution, typename Result>
Result generate( boost::mt19937& random_actor, const Result min, const Result max )
{
	Distribution dist( min, max );
	boost::variate_generator<boost::mt19937&, Distribution> vg( random_actor, dist );
	return vg();
}

RandomNumberGenerator::RandomNumberGenerator() : random_actor_()
{
}

RandomNumberGenerator::RandomNumberGenerator( const tUINT seed ) : random_actor_( seed )
{
}

RandomNumberGenerator::~RandomNumberGenerator()
{
}

tUINT RandomNumberGenerator::randU()
{
	return generate<boost::uniform_int<tUINT>, tUINT>( random_actor_, 0, UINT_MAX );
}

tUINT RandomNumberGenerator::randU( const tUINT n )
{
	if( n == 0 )
		return 0;

	return generate<boost::uniform_int<tUINT>, tUINT>( random_actor_, 0, n );
}

tUINT RandomNumberGenerator::randU( const tUINT min, const tUINT max )
{
	if( min == max )
		return min;

	return generate<boost::uniform_int<tUINT>, tUINT>( random_actor_, min, max );
}

tINT RandomNumberGenerator::randI()
{
	return generate<boost::uniform_int<tINT>, tINT>( random_actor_, 0, INT_MAX );
}

tINT RandomNumberGenerator::randI( const tINT n )
{
	if( n == 0 )
		return 0;

	return generate<boost::uniform_int<tINT>, tINT>( random_actor_, 0, n );
}

tINT RandomNumberGenerator::randI( const tINT min, const tINT max )
{
	return generate<boost::uniform_int<tINT>, tINT>( random_actor_, min, max );
}

tFLOAT RandomNumberGenerator::randF()
{
	return generate<boost::uniform_real<tFLOAT>, tFLOAT>( random_actor_, 0.0f, 1.0f );
}

tFLOAT RandomNumberGenerator::randF( const tFLOAT n )
{
	return generate<boost::uniform_real<tFLOAT>, tFLOAT>( random_actor_, 0.0f, n );
}

tFLOAT RandomNumberGenerator::randF( const tFLOAT min, const tFLOAT max )
{
	if( min == max )
		return min;

	return generate<boost::uniform_real<tFLOAT>, tFLOAT>( random_actor_, min, max );
}

tDOUBLE RandomNumberGenerator::randD()
{
	return generate<boost::uniform_real<tDOUBLE>, tDOUBLE>( random_actor_, 0.0f, 1.0f );
}

tDOUBLE RandomNumberGenerator::randD( const tDOUBLE n )
{
	if( n == 0.0f )
		return 0.0f;

	return generate<boost::uniform_real<tDOUBLE>, tDOUBLE>( random_actor_, 0.0f, n );
}

tDOUBLE RandomNumberGenerator::randD( const tDOUBLE min, const tDOUBLE max )
{
	if( min == max )
		return min;

	return generate<boost::uniform_real<tDOUBLE>, tDOUBLE>( random_actor_, min, max );
}

tFLOAT RandomNumberGenerator::randNormF( const tFLOAT mean, const tFLOAT variance )
{
	boost::normal_distribution<tFLOAT> dist( mean, variance );
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<tFLOAT> > vg( random_actor_, dist );
	return vg();
}

tDOUBLE RandomNumberGenerator::randNormD( const tDOUBLE mean, const tDOUBLE variance )
{
	boost::normal_distribution<tDOUBLE> dist( mean, variance );
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<tDOUBLE> > vg( random_actor_, dist );
	return vg();
}