#pragma once
#include "pch.h"
#include <sql.h>
#include "DBConn.h"
#include "ReadWriteLock.h"

using DBConnRef = std::shared_ptr<DBConn>;
class DBPool
{
public:
    DBPool();
    ~DBPool();
    void Init(const wchar_t* connStr);
    DBConnRef Pop();
    void Push(DBConnRef conn);
    static void GlobalPush(DBConn* conn);

private:
    Lock lock;
    SQLHENV _henv = SQL_NULL_HANDLE;
    Queue<DBConnRef> _dbQueue;
    wchar_t _connStr[256];
    // wchar_t* _connStr = nullptr;
};
