#pragma once
#include "DBOrm.h"

class InventoryDB
{
public:
	InventoryDB();
	~InventoryDB();

	void LoadDB(int32 playerCode);
	bool GetItem(int32& itemCode, int32& type, int32& count);

	void SaveInsertDB(int32 playerCode, int32 itemCode, int32 type, int32 count);
	void SaveUpdateDB(int32 playerCode, int32 itemCode, int32 type, int32 count);

	void SetCommit();

private:
	DBOrm _dbOrm;
	int32 _itemCode = -1;
	int32 _type = -1;
	int32 _count = -1;
	wchar_t insertQuery[100] = L"INSERT INTO Inventory (playerCode, itemCode, itemType, itemCount) VALUES (?,?,?,?)";
	wchar_t updateQuery[100] = L"UPDATE Inventory SET itemCount = ? WHERE playerCode = ? AND itemCode = ?";

	DBConnRef conn = nullptr;
};
