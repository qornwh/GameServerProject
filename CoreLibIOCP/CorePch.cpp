#include "pch.h"
#include "CorePch.h"
#include <cassert>

void CrashFunc(bool result)
{
    if (!result)
    {
        assert(result);
        exit(1);
    }
}
