#include "pch.h"
#include "DBOrm.h"
#include <sqlext.h>

DBOrm::DBOrm(int32 size)
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

void DBOrm::BindParamInt(SQLPOINTER value)
{
	_conn->BindParameter(_paramIdx, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, value, 0, nullptr);
	_paramIdx++;
}

void DBOrm::BindParamWchar(SQLULEN len, SQLPOINTER value)
{
	_conn->BindParameter(_paramIdx, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, len, 0, value, 0, nullptr);
	_paramIdx++;
}

void DBOrm::ParamInit()
{
	_paramIdx = 1;
}
