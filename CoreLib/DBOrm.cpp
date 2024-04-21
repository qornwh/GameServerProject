#include "pch.h"
#include "DBOrm.h"
#include <sqlext.h>

DBOrm::DBOrm(int size)
{
	_sqlLenList.resize(size);
}

DBOrm::~DBOrm()
{
}

void DBOrm::SetDBConn(DBConnRef conn)
{
	_conn = conn;
}

void DBOrm::BindColInt(SQLULEN len, SQLPOINTER value)
{
	_conn->BindCol(_colIdx, SQL_C_DEFAULT, len, value, &_sqlLenList[_colIdx - 1]);
	_colIdx++;
}

void DBOrm::BindColWchar(SQLULEN len, SQLPOINTER value)
{
	_conn->BindCol(_colIdx, SQL_C_WCHAR, len, value, &_sqlLenList[_colIdx - 1]);
	_colIdx++;
}
