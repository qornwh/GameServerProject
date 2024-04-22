#pragma once
#include "DBOrm.h"

class SessionDB
{
public:
	SessionDB();
	~SessionDB();

	bool LoginDB(const WCHAR* id, int& accountCode);
	bool LoginCheck(const WCHAR* pwd);

	bool CreateAccount(const WCHAR* id, const WCHAR* pwd);

	bool PlayerDB(int32 accountCode);

	void GetPlayerDBInfo(int32& playerCode, WCHAR* name, int32& jobCode, int32& mapCode);

private:
	DBOrm _dbOrm;
	int32 _accountCode = -1;
	WCHAR _id[10] = { 0, };
	WCHAR _pwd[10] = { 0, };

	// player
	int32 _playerCode = -1;
	WCHAR _name[10] = { 0, };
	int32 _jobCode = -1;
	int32 _mapCode = -1;
};

