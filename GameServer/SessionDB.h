#pragma once
#include "DBOrm.h"

class SessionDB
{
public:
	SessionDB();
	~SessionDB();

	bool LoginDB(const wchar_t* id, int& accountCode);
	bool LoginCheck(const wchar_t* pwd);
	bool CreateAccount(const wchar_t* id, const wchar_t* pwd);
	bool CreateCharacter(const wchar_t* name, int32 jobCode, int32 accountCode);
	bool PlayerDB(int32 accountCode);
	void GetPlayerDBInfo(int32& playerCode, wchar_t* name, int32& jobCode, int32& mapCode, int32& gold);
	void SavePlayerDB(int32 playerCode, int32 gold);

private:
	DBOrm _dbOrm;
	int32 _accountCode = -1;
	wchar_t _id[10] = { 0, };
	wchar_t _pwd[10] = { 0, };

	// player
	int32 _playerCode = -1;
	wchar_t _name[10] = { 0, };
	int32 _jobCode = -1;
	int32 _mapCode = -1;
	int32 _gold = 0;
};

