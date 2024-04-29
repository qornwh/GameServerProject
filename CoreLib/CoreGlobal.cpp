#include "pch.h"
#include "CoreGlobal.h"
#include "DBConnectPool.h"
#include "ThreadManager.h"

ThreadManager* GThreadManager = nullptr;
DBPool* GDBPool = nullptr;

class CoreGlobal
{
public:
    CoreGlobal()
    {
        GThreadManager = new ThreadManager();
        GDBPool = new DBPool();
    }

    ~CoreGlobal()
    {
        if (GThreadManager)
            delete GThreadManager;
        if (GDBPool)
            delete GDBPool;
    }
}GCoreGlobal;
