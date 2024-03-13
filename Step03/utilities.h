#pragma once
#include "StdAfx.h"
#include <memory>
#include <vector>

class ObjectOpenCloseWrapper {
private:
	AcDbObject* m_pObject;

public:
	ObjectOpenCloseWrapper();
	~ObjectOpenCloseWrapper();

	AcDbBlockTable* GetBlockTable(AcDb::OpenMode mode);
	AcDbLayerTable* GetLayerTable(AcDb::OpenMode mode);
	void isOccupied();
};

//----------------------------------------------------------
class BlockTableWrapper {
private:
	AcDbBlockTable* m_pBlockTable{};

public:
	BlockTableWrapper(AcDb::OpenMode mode);
	~BlockTableWrapper();
	AcDbBlockTable* Get();
	AcDbBlockTableRecord*
		Add(std::unique_ptr<AcDbBlockTableRecord>& pBlockTableRecord);
};

class BlockTableRecordWrapper {
private:
	AcDbBlockTableRecord* m_pBlockTableRecord{};

public:
	BlockTableRecordWrapper(AcDbBlockTableRecord*);
	~BlockTableRecordWrapper();
	AcDbBlockTableRecord* Get();
	void Add(std::unique_ptr<AcDbEntity>& pEntity);
};








