#include "pch.h"
#include "CorePch.h"

void CrashFunc(bool result)
{
    if (!result)
    {
        assert(result);
        exit(1);
    }
}
