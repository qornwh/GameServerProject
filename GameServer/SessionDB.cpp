#include "SessionDB.h"

SessionDB::SessionDB() : _dbOrm(4)
{
}

SessionDB::~SessionDB()
{
}

bool SessionDB::LoginDB(const WCHAR* id)
{
	const wchar_t* query = L"SELECT accountCode, id, pwd FROM Account WHERE id = ?";
	DBConnRef conn = GDBPool->Pop();
	_dbOrm.SetDBConn(conn);
	conn->Prepare(query);
	_dbOrm.BindParamWchar(sizeof(id), (SQLPOINTER)id);
	conn->Execute();

	_dbOrm.BindColInt(sizeof(_accountCode), &_accountCode);
	_dbOrm.BindColWchar(sizeof(_id), &_id);
	_dbOrm.BindColWchar(sizeof(_pwd), &_pwd);

	bool result = conn->Fetch();
	conn->FreeStmt();
	_dbOrm.ReSetIdx();

	return result;
}

bool SessionDB::LoginCheck(const WCHAR* pwd)
{
	if (wcscmp(_pwd, pwd) == 0)
		return true;
	return false;
}

bool SessionDB::CreateAccount(const WCHAR* id, const WCHAR* pwd)
{
	const wchar_t* query = L"INSERT INTO Account (id, pwd) VALUES (?, ?)";
	DBConnRef conn = GDBPool->Pop();
	
	_dbOrm.SetDBConn(conn);
	conn->Prepare(query);
	_dbOrm.BindParamWchar(sizeof(id), (SQLPOINTER)id);
	_dbOrm.BindParamWchar(sizeof(pwd), (SQLPOINTER)pwd);
	bool result = conn->Execute();
	conn->FreeStmt();
	_dbOrm.ReSetIdx();

	return result;
}

bool SessionDB::PlayerDB()
{
	const wchar_t* query = L"SELECT playerCode, name, jobCode, mapCode FROM Player WHERE accountCode = ?";
	DBConnRef conn = GDBPool->Pop();
	_dbOrm.SetDBConn(conn);
	conn->Prepare(query);
	_dbOrm.BindParamInt(&_accountCode);
	conn->Execute();

	_dbOrm.BindColInt(sizeof(_playerCode), &_playerCode);
	_dbOrm.BindColWchar(sizeof(_name), &_name);
	_dbOrm.BindColInt(sizeof(_jobCode), &_jobCode);
	_dbOrm.BindColInt(sizeof(_mapCode), &_mapCode);

	bool result = conn->Fetch();
	conn->FreeStmt();
	_dbOrm.ReSetIdx();

	return result;
}

void SessionDB::GetPlayerDBInfo(int32& playerCode, WCHAR* name, int32& jobCode, int32& mapCode, int32& accountCode)
{
	playerCode = _playerCode;
	jobCode = _jobCode;
	mapCode = _mapCode;
	accountCode = _accountCode;
	wcscpy_s(name, 10, _name);
}
