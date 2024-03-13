#include "StdAfx.h"
#include "utilities.h"
#include <exception>
#include <stdexcept>

ObjectOpenCloseWrapper::ObjectOpenCloseWrapper()
	: m_pObject{ nullptr } {}

ObjectOpenCloseWrapper::~ObjectOpenCloseWrapper() {
	if (m_pObject) {
		m_pObject->close();
	}
}

AcDbBlockTable* ObjectOpenCloseWrapper::GetBlockTable(AcDb::OpenMode mode) {
	isOccupied();
	AcDbBlockTable* pBlockTable;
	if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, mode)
		!= Acad::eOk) {
		throw std::runtime_error("Can't open BlockTable");
	}
	m_pObject = pBlockTable;
	return pBlockTable;
}

AcDbLayerTable* ObjectOpenCloseWrapper::GetLayerTable(AcDb::OpenMode mode) {
	isOccupied();
	AcDbLayerTable* pLayerTable;
	if (acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, mode)
		!= Acad::eOk) {
		throw std::runtime_error("Can't open BlockTable");
	}
	m_pObject = pLayerTable;
	return pLayerTable;
}

void ObjectOpenCloseWrapper::isOccupied() {
	if (m_pObject) {
		throw std::runtime_error("Attempt to redefine ObjectOpenCloseWrapper");
	}
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








