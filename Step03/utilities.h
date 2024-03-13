#pragma once
#include "StdAfx.h"
#include <memory>
#include <initializer_list>

class BlockTableWrapper {
private:
	AcDbBlockTable* m_pBlockTable{};

public:
	BlockTableWrapper(AcDb::OpenMode mode);
	~BlockTableWrapper();
	AcDbBlockTable* Get();
	void Add(std::unique_ptr<AcDbBlockTableRecord>& cBlockTableRecord);
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








