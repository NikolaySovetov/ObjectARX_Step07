#pragma once
#include "StdAfx.h"
#include <memory>

class BlockTableWrapper {
private:
	AcDbBlockTable* m_pBlockTable{};

public:
	BlockTableWrapper(AcDb::OpenMode mode);
	~BlockTableWrapper();
	AcDbBlockTable* Get();
	void Add(std::unique_ptr<AcDbBlockTableRecord>& cBlockTableRecord);
};

class CircleWrapper {
public:
	AcDbCircle* m_pCircle;
	CircleWrapper() = default;
	~CircleWrapper

};










