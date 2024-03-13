#pragma once
#include "StdAfx.h"
#include <memory>
#include <vector>
#include <stdexcept>

class LayerTableWrapper {
private:
	AcDbLayerTable* m_pLayerTable;

public:
	LayerTableWrapper(AcDb::OpenMode mode);
	~LayerTableWrapper();
	AcDbLayerTable* Get();
	AcDbLayerTableRecord*
		Add(std::unique_ptr<AcDbLayerTableRecord>& pLayerTableRecord);
};

class ObjectWrapper {
private:
	std::vector<AcDbObject*> m_pObjects;

public:
	ObjectWrapper() = default;
	~ObjectWrapper() {
		for (const auto& pObject : m_pObjects) {
			pObject->close();
		}
	}

	AcDbBlockTable* GetBlockTable(AcDb::OpenMode mode) {
		AcDbBlockTable* pBlockTable;
		if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, mode)
			!= Acad::eOk) {
			throw std::runtime_error("Can't open BlockTable");
		}
		m_pObjects.push_back(pBlockTable);
		return pBlockTable;
	}

	AcDbLayerTable* GetLayerTable(AcDb::OpenMode mode) {
		AcDbLayerTable* pLayerTable;
		if (acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, mode)
			!= Acad::eOk) {
			throw std::runtime_error("Can't open BlockTable");
		}
		m_pObjects.push_back(pLayerTable);
		return pLayerTable;
	}
};




class LayerTableRecordWrapper {
private:
	AcDbLayerTableRecord* m_pBlockTableRecord{};

public:
	LayerTableRecordWrapper(AcDbLayerTableRecord*);
	~LayerTableRecordWrapper();
	AcDbLayerTableRecord* Get();
	void Add(std::unique_ptr<AcDbEntity>& pEntity);
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








