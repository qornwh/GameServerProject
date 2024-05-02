#pragma once
#include "pch.h"
#include "DBConnectPool.h"

class DBOrm
{
public:
	DBOrm(int size);
	~DBOrm();

	void SetDBConn(DBConnRef conn);
	void BindColInt(SQLULEN len, SQLPOINTER value);
	void BindColWchar(SQLULEN len, SQLPOINTER value);

	void BindParamInt(SQLPOINTER value);
	void BindParamWchar(SQLULEN len, SQLPOINTER value);

	void ReSetIdx()
	{
		_colIdx = 1;
		_paramIdx = 1;
	}

private:
	DBConnRef _conn;
	int _colIdx = 1;
	int _paramIdx = 1;
	Vector<SQLLEN> _sqlLenList;
};
