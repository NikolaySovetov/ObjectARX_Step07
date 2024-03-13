#include "StdAfx.h"
#include "utilities.h"
#include <stdexcept>

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

void BlockTableWrapper::Add(std::unique_ptr<AcDbBlockTableRecord>& cBlockTableRecord) {
	if (m_pBlockTable->add(cBlockTableRecord.get()) != Acad::eOk) {
		throw std::runtime_error("Can't add BlockTableRecord to BlockTable");
	}
	cBlockTableRecord.release();
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








