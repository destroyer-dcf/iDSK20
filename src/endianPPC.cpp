#include <iostream>
#include <cstdlib>
using namespace std;
#include "endianPPC.h"

#ifndef _MSC_VER
#ifndef __MINGW32__
#include <sys/param.h>
#if defined(__FreeBSD__) || defined(__APPLE__) || defined(__MORPHOS__)
#include <machine/endian.h>
#else
#include <endian.h>
#endif
#endif
#endif

bool isBigEndian(void)
{
#ifdef BYTE_ORDER
    	return BYTE_ORDER == BIG_ENDIAN;
#elif defined _MSC_VER || defined __MINGW32__
	return false; // Modern VC++ and MinGW only run on x86, x64 and ARM, doesn't require big/little endian conversion
#else
    return __BYTE_ORDER == __BIG_ENDIAN;
#endif
}
