#include "StdAfx.h"
#include "utilities.h"
#include <exception>
#include <stdexcept>

LayerTableWrapper::LayerTableWrapper(AcDb::OpenMode mode) {
	if (acdbHostApplicationServices()->workingDatabase()->getLayerTable(m_pLayerTable, mode)
		!= Acad::eOk) {
		throw std::runtime_error("Can't open LayerTable");
	}
}

LayerTableWrapper::~LayerTableWrapper() {
	if (m_pLayerTable) {
		m_pLayerTable->close();
	}
}

AcDbLayerTable* LayerTableWrapper::Get() {
	return m_pLayerTable;
}

AcDbLayerTableRecord* 
LayerTableWrapper::Add(std::unique_ptr<AcDbLayerTableRecord>& pLayerTableRecord) {
	if (m_pLayerTable->add(pLayerTableRecord.get()) != Acad::eOk) {
		throw std::runtime_error("Can't add BlockTableRecord to BlockTable");
	}
	AcDbLayerTableRecord* ptr{ pLayerTableRecord.get() };
	pLayerTableRecord.release();
	return ptr;
}


//----------------------------------------------------------
BlockTableWrapper::BlockTableWrapper(AcDb::OpenMode mode) {
	if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(m_pBlockTable, mode)
		!= Acad::eOk) {
		throw std::runtime_error("Can't open BlockTable");
	}
}

BlockTableWrapper::~BlockTableWrapper() {
	if (m_pBlockTable) {
		m_pBlockTable->close();
	}
}

AcDbBlockTable* BlockTableWrapper::Get() {
	return m_pBlockTable;
}

AcDbBlockTableRecord* 
BlockTableWrapper::Add(std::unique_ptr<AcDbBlockTableRecord>& pBlockTableRecord) {
	if (m_pBlockTable->add(pBlockTableRecord.get()) != Acad::eOk) {
		throw std::runtime_error("Can't add BlockTableRecord to BlockTable");
	}
	AcDbBlockTableRecord* ptr{ pBlockTableRecord.get() };
	pBlockTableRecord.release();
	return ptr;
}

//----------------------------------------------------------
BlockTableRecordWrapper::BlockTableRecordWrapper(AcDbBlockTableRecord* pBlockTableRecord) {
	m_pBlockTableRecord = pBlockTableRecord;
}

BlockTableRecordWrapper::~BlockTableRecordWrapper() {
	if (m_pBlockTableRecord) {
		m_pBlockTableRecord->close();
	}
}

AcDbBlockTableRecord* BlockTableRecordWrapper::Get() {
	return m_pBlockTableRecord;
}

void BlockTableRecordWrapper::Add(std::unique_ptr<AcDbEntity>& pEntity) {
	if (m_pBlockTableRecord->appendAcDbEntity(pEntity.get()) != Acad::eOk) {
		throw std::runtime_error("Can't add an Entity to BlockTableRecord");
	}
	pEntity->close();
	pEntity.release();
}








