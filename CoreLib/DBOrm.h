#pragma once
#include "DBConnectPool.h"

class DBOrm
{
public:
	DBOrm(int size);
	~DBOrm();

	void SetDBConn(DBConnRef conn);
	void BindColInt(SQLULEN len, SQLPOINTER value);
	void BindColWchar(SQLULEN len, SQLPOINTER value);

private:
	DBConnRef _conn;
	int _colIdx = 1;
	vector<SQLLEN> _sqlLenList;
};
