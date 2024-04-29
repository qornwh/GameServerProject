#pragma once


#ifdef _DEBUG
#pragma comment(lib, "CoreLib\\Debug\\CoreLib.lib")
#else
#pragma comment(lib, "CoreLib\\Release\\CoreLib.lib")
#endif

#include "CorePch.h"
#define AMODE 1

# if AMODE == 0
static boost::random::mt19937 rng;
static boost::random::uniform_int_distribution<> dstX{-2000, 2000};
static boost::random::uniform_int_distribution<> dstY{-2000, 2000};
static boost::random::uniform_real_distribution<> rot{0, 360};
# elif AMODE == 1
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
static boost::random::mt19937 rng;
static boost::random::uniform_int_distribution<> dstX{-15, 15};
static boost::random::uniform_int_distribution<> dstY{-22, 22};
static boost::random::uniform_real_distribution<> rot{0, 360};
#endif
