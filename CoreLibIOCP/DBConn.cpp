#include "pch.h"
#include "DBConn.h"
#include <sqlext.h>

DBConn::DBConn()
{
}

DBConn::~DBConn()
{
    if (_hdbc != SQL_NULL_HANDLE)
    {
        SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
        _hdbc = SQL_NULL_HANDLE;
    }

    if (_hstmt != SQL_NULL_HANDLE)
    {
        SQLFreeHandle(SQL_HANDLE_DBC, _hstmt);
        _hstmt = SQL_NULL_HANDLE;
    }
}

void DBConn::Init(SQLHENV& henv, wchar_t* connStr)
{
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &_hdbc);
    CrashFunc(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
    ret = SQLDriverConnect(_hdbc, nullptr, reinterpret_cast<SQLWCHAR*>(connStr), SQL_NTS, nullptr, 0, nullptr, SQL_DRIVER_COMPLETE);
    CrashFunc(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
    ret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &_hstmt);
    CrashFunc(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}

bool DBConn::Prepare(const wchar_t* query)
{
    SQLRETURN ret = SQLPrepare(_hstmt, (SQLWCHAR*)query, SQL_NTSL);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    ErrorDisplay(ret);
    return false;
}

bool DBConn::Execute()
{
    SQLRETURN ret = SQLExecute(_hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    ErrorDisplay(ret);
    return false;
}

bool DBConn::Exec(const wchar_t* query)
{
    SQLRETURN ret = SQLExecDirect(_hstmt, (SQLWCHAR*)query, SQL_NTSL);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    ErrorDisplay(ret);
    return false;
}

bool DBConn::Fetch()
{
    SQLRETURN ret = SQLFetch(_hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    if (ret == SQL_ERROR)
    {
        ErrorDisplay(ret);
    }
    return false;
}

void DBConn::ErrorDisplay(SQLRETURN ret)
{
    SQLGetDiagRec(SQL_HANDLE_STMT, _hstmt, ++_rec, _state, &_native, _message, sizeof(_message), &_length);
    wprintf(L"Sql Error %d : %s\n", ret, _message);
}

bool DBConn::BindCol(SQLUSMALLINT colIdx, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index)
{
    SQLRETURN ret = SQLBindCol(_hstmt, colIdx, cType, value, len, index);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    ErrorDisplay(ret);
    return false;
}

bool DBConn::BindParameter(SQLUSMALLINT colIdx, SQLSMALLINT paramType, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN cbColDef, SQLSMALLINT ibScale, SQLPOINTER rgbValue, SQLLEN cbValueMax, SQLLEN* index)
{
    SQLRETURN ret = SQLBindParameter(_hstmt, colIdx, paramType, cType, sqlType, cbColDef, ibScale, rgbValue, cbValueMax, index);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    ErrorDisplay(ret);
    return false;
}

bool DBConn::FreeStmt()
{
    SQLRETURN ret = SQLFreeStmt(_hstmt, SQL_UNBIND);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    ErrorDisplay(ret);
    return false;
}

bool DBConn::CloseCursor()
{
    SQLRETURN ret = SQLCloseCursor(_hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    ErrorDisplay(ret);
    return false;
}

bool DBConn::ConnectAttr()
{
    SQLRETURN ret = SQLSetConnectAttr(_hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    ErrorDisplay(ret);
    return false;
}

bool DBConn::EndTran()
{
    SQLRETURN ret = SQLEndTran(SQL_HANDLE_DBC, _hdbc, SQL_COMMIT);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    ErrorDisplay(ret);
    return false;
}
