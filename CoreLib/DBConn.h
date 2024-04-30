#pragma once
#include "pch.h"
#include <sql.h>

class DBConn : public std::enable_shared_from_this<DBConn>
{
public:
	DBConn();
	~DBConn();

	void Init(SQLHENV& henv, wchar_t* connStr);
	bool Prepare(const wchar_t* query);
	bool Execute();
	bool Exec(const wchar_t* query);
	bool Fetch();
	void ErrorDisplay(SQLRETURN ret);
	bool BindCol(SQLUSMALLINT colIdx, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
	bool BindParameter(SQLUSMALLINT colIdx, SQLSMALLINT paramType, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN cbColDef, SQLSMALLINT ibScale, SQLPOINTER rgbValue, SQLLEN cbValueMax, SQLLEN* index);
	bool FreeStmt();
	bool CloseCursor();

	bool ConnectAttr();
	bool EndTran();

private:
	SQLHDBC _hdbc = SQL_NULL_HANDLE;
	SQLHSTMT _hstmt = SQL_NULL_HANDLE;

	// error
	SQLSMALLINT _length;
	SQLINTEGER _rec = 0, _native;
	SQLWCHAR _state[7], _message[256];
};