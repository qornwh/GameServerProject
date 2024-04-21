#pragma once
#include <DBOrm.h>

class PlayerDB
{
public:
	PlayerDB();
	~PlayerDB();

	bool LoginDB(const wchar_t* id, const wchar_t* pwd);
	bool LoginCheck(const wchar_t* id, const wchar_t* pwd);
	void LoadDB();

private:
	int32 _accountCode = -1;
	wchar_t _id[10] = { 0, };
	int32 _playerCode = -1;
	wchar_t _name[10] = { 0, };
	int32 _jobCode = -1;
	int32 _mapCode = -1;
};

