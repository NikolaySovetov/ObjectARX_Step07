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
void AddEmployeeBlockRecord(AcDbBlockTable* pBTable, const TCHAR* strBlockName) {
	const double pi{ 3.1415926 };

	AcDbSymbolTableRecordPointer<AcDbBlockTableRecord> pBTRecord;
	pBTRecord.create();
	pBTRecord->setOrigin(AcGePoint3d{ 0, 0, 0 });
	pBTRecord->setName(strBlockName);
	if (pBTable->add(pBTRecord) != Acad::eOk) {
		throw std::runtime_error("Can't add BlockRecord to BlockTable");
	}

	AcDbObjectPointerBase<AcDbCircle> face;
	face.create();
	face->setCenter(AcGePoint3d::kOrigin);
	face->setNormal(AcGeVector3d::kZAxis);
	face->setRadius(1.0);
	face->setColorIndex(2);
	if (pBTRecord->appendAcDbEntity(face) != Acad::eOk) {
		throw std::runtime_error("Can't add Object to BlockRecord");
	}

	AcDbObjectPointer<AcDbCircle> rightEye;
	rightEye.create();
	rightEye->setCenter(AcGePoint3d(0.15, 0.20, 0));
	rightEye->setNormal(AcGeVector3d::kZAxis);
	rightEye->setRadius(0.05);
	rightEye->setColorIndex(1);
	if (pBTRecord->appendAcDbEntity(rightEye) != Acad::eOk) {
		throw std::runtime_error("Can't add Object to BlockRecord");
	};

	AcDbObjectPointer<AcDbCircle> leftEye;
	leftEye.create();
	leftEye->setCenter(AcGePoint3d(-0.15, 0.30, 0));
	leftEye->setNormal(AcGeVector3d::kZAxis);
	leftEye->setRadius(0.05);
	leftEye->setColorIndex(1);
	if (pBTRecord->appendAcDbEntity(leftEye) != Acad::eOk) {
		throw std::runtime_error("Can't add Object to BlockRecord");
	}

	AcDbObjectPointer<AcDbArc> mouth;
	mouth.create();
	mouth->setCenter(AcGePoint3d(0, 0.25, 0));
	mouth->setNormal(AcGeVector3d::kZAxis);
	mouth->setRadius(0.8);
	mouth->setStartAngle(pi + pi * 0.3);
	mouth->setEndAngle(pi + pi * 0.7);
	mouth->setColorIndex(3);
	if (pBTRecord->appendAcDbEntity(mouth) != Acad::eOk) {
		throw std::runtime_error("Can't add Object to BlockRecord");
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








