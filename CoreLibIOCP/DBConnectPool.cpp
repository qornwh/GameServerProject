#include "pch.h"
#include <sqlext.h>
#include "DBConnectPool.h"

DBPool::DBPool()
{
}

DBPool::~DBPool()
{
    WriteLockGuard writeLock(lock, "write");
    while (!_dbQueue.empty())
        _dbQueue.pop();
    
    if (_henv != SQL_NULL_HANDLE)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, _henv);
        _henv = SQL_NULL_HANDLE;
    }
}

void DBPool::Init(const wchar_t* connStr)
{
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, nullptr, &_henv);
    CrashFunc(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
    ret = SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);
    CrashFunc(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);

    wcscpy_s(_connStr, connStr);
    WriteLockGuard writeLock(lock, "write");
    for (int i = 0; i < 10; i++)
    {
        DBConnRef conn = DBConnRef(new DBConn(), GlobalPush);
        conn->Init(_henv, _connStr);
        _dbQueue.emplace(conn);
    }
}

DBConnRef DBPool::Pop()
{
    WriteLockGuard writeLock(lock, "write");
    if (_dbQueue.empty())
    {
        DBConnRef conn = DBConnRef(new DBConn(), GlobalPush);
        conn->Init(_henv, _connStr);
        return conn;
    }
    else
    {
        DBConnRef dbConn = _dbQueue.front();
        _dbQueue.pop();
        return dbConn;
    }
}

void DBPool::Push(DBConnRef conn)
{
    WriteLockGuard writeLock(lock, "write");
    _dbQueue.emplace(conn);
}

void DBPool::GlobalPush(DBConn* conn)
{
    GDBPool->Push(DBConnRef(conn, GlobalPush));
}
