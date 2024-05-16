#include "pch.h"
#include "SessionDB.h"

SessionDB::SessionDB() : _dbOrm(5)
{
}

SessionDB::~SessionDB()
{
}

bool SessionDB::LoginDB(const wchar_t* id, int& accountCode)
{
	const wchar_t* query = L"SELECT accountCode, id, pwd FROM Account WHERE id = ?";
	DBConnRef conn = GDBPool->Pop();
	_dbOrm.SetDBConn(conn);
	bool result = conn->Prepare(query);
	_dbOrm.BindParamWchar(sizeof(id), (SQLPOINTER)id);
	result = conn->Execute();

	_dbOrm.BindColInt(sizeof(_accountCode), &_accountCode);
	_dbOrm.BindColWchar(sizeof(_id), &_id);
	_dbOrm.BindColWchar(sizeof(_pwd), &_pwd);

	result = conn->Fetch();
	conn->CloseCursor();
	conn->FreeStmt();
	_dbOrm.ReSetIdx();
	accountCode = _accountCode;

	return result;
}

bool SessionDB::LoginCheck(const wchar_t* pwd)
{
	if (wcscmp(_pwd, pwd) == 0)
		return true;
	return false;
}

bool SessionDB::CreateAccount(const wchar_t* id, const wchar_t* pwd)
{
	const wchar_t* query = L"INSERT INTO Account (id, pwd) VALUES (?, ?)";
	DBConnRef conn = GDBPool->Pop();

	_dbOrm.SetDBConn(conn);
	bool result = conn->Prepare(query);
	_dbOrm.BindParamWchar(sizeof(id), (SQLPOINTER)id);
	_dbOrm.BindParamWchar(sizeof(pwd), (SQLPOINTER)pwd);
	result = conn->Execute();
	conn->EndTran();
	conn->CloseCursor();
	conn->FreeStmt();
	_dbOrm.ReSetIdx();

	return result;
}

bool SessionDB::CreateCharacter(const wchar_t* name, int32 jobCode, int32 accountCode)
{
	const wchar_t* query = L"INSERT INTO Player (name, jobCode, mapCode, accountCode) VALUES (?, ?, 1, ?)";
	DBConnRef conn = GDBPool->Pop();

	_dbOrm.SetDBConn(conn);
	bool result = conn->Prepare(query);
	_dbOrm.BindParamWchar(sizeof(name), (SQLPOINTER)name);
	_dbOrm.BindParamInt(&jobCode);
	_dbOrm.BindParamInt(&accountCode);
	result = conn->Execute();
	conn->EndTran();
	conn->CloseCursor();
	conn->FreeStmt();
	_dbOrm.ReSetIdx();

	return result;
}

bool SessionDB::PlayerDB(int32 accountCode)
{
	const wchar_t* query = L"SELECT playerCode, name, jobCode, mapCode, gold FROM Player WHERE accountCode = ?";
	DBConnRef conn = GDBPool->Pop();

	_dbOrm.SetDBConn(conn);
	bool result = conn->Prepare(query);
	if (_accountCode > 0)
	{
		_dbOrm.BindParamInt(&_accountCode);
	}
	else
	{
		_dbOrm.BindParamInt(&accountCode);
	}
	result = conn->Execute();

	_dbOrm.BindColInt(sizeof(_playerCode), &_playerCode);
	_dbOrm.BindColWchar(sizeof(_name), &_name);
	_dbOrm.BindColInt(sizeof(_jobCode), &_jobCode);
	_dbOrm.BindColInt(sizeof(_mapCode), &_mapCode);
	_dbOrm.BindColInt(sizeof(_gold), &_gold);

	result = conn->Fetch();
	conn->CloseCursor();
	conn->FreeStmt();
	_dbOrm.ReSetIdx();

	return result;
}

void SessionDB::GetPlayerDBInfo(int32& playerCode, wchar_t* name, int32& jobCode, int32& mapCode, int32& gold)
{
	playerCode = _playerCode;
	jobCode = _jobCode;
	mapCode = _mapCode;
	gold = _gold;
	wcscpy_s(name, 10, _name);
}

void SessionDB::SavePlayerDB(int32 playerCode, int32 gold)
{
	const wchar_t* query = L"UPDATE Player SET gold = ? WHERE playerCode = ?";
	DBConnRef conn = GDBPool->Pop();

	_dbOrm.SetDBConn(conn);
	bool result = conn->Prepare(query);
	_dbOrm.BindParamInt(&gold);
	_dbOrm.BindParamInt(&playerCode);
	result = conn->Execute();
	result = conn->Fetch();
	conn->CloseCursor();
	conn->FreeStmt();
}
