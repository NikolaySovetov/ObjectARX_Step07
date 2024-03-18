#pragma once
#include "StdAfx.h"
#include <memory>

class SymbolTableWrapper {
private:
	AcDbSymbolTable* m_pSTable;

public:
	SymbolTableWrapper();
	~SymbolTableWrapper();

	AcDbBlockTable* GetBlockTable(AcDb::OpenMode mode);
	AcDbLayerTable* GetLayerTable(AcDb::OpenMode mode);
	void isOccupied();
};

void AddEmployeeBlockRecord(AcDbBlockTable* pBTable, const TCHAR* strBlockName);






