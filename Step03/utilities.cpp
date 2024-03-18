#include "StdAfx.h"
#include "utilities.h"
#include <exception>
#include <stdexcept>

SymbolTableWrapper::SymbolTableWrapper()
	: m_pSTable{ nullptr } {}

SymbolTableWrapper::~SymbolTableWrapper() {
	if (m_pSTable) {
		m_pSTable->close();
	}
}

AcDbBlockTable* SymbolTableWrapper::GetBlockTable(AcDb::OpenMode mode) {
	isOccupied();
	AcDbBlockTable* pBlockTable;
	if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, mode)
		!= Acad::eOk) {
		throw std::runtime_error("Can't open BlockTable");
	}
	m_pSTable = pBlockTable;
	return pBlockTable;
}

AcDbLayerTable* SymbolTableWrapper::GetLayerTable(AcDb::OpenMode mode) {
	isOccupied();
	AcDbLayerTable* pLayerTable;
	if (acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, mode)
		!= Acad::eOk) {
		throw std::runtime_error("Can't open BlockTable");
	}
	m_pSTable = pLayerTable;
	return pLayerTable;
}

void SymbolTableWrapper::isOccupied() {
	if (m_pSTable) {
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
	auto pFace = new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, 1.0);
	face.acquire(pFace);
	face->setColorIndex(2);
	if (pBTRecord->appendAcDbEntity(face) != Acad::eOk) {
		throw std::runtime_error("Can't add Object to BlockRecord");
	}

	AcDbObjectPointer<AcDbCircle> rightEye;
	auto pRightEye = new AcDbCircle(AcGePoint3d(0.15, 0.20, 0), AcGeVector3d::kZAxis, 0.05);
	rightEye.acquire(pRightEye);
	rightEye->setColorIndex(1);
	if (pBTRecord->appendAcDbEntity(rightEye) != Acad::eOk) {
		throw std::runtime_error("Can't add Object to BlockRecord");
	};

	AcDbObjectPointer<AcDbCircle> leftEye;
	auto pLeftEye = new AcDbCircle(AcGePoint3d(-0.15, 0.30, 0), AcGeVector3d::kZAxis, 0.05);
	leftEye.acquire(pLeftEye);
	leftEye->setColorIndex(1);
	if (pBTRecord->appendAcDbEntity(leftEye) != Acad::eOk) {
		throw std::runtime_error("Can't add Object to BlockRecord");
	}

	AcDbObjectPointer<AcDbArc> mouth;
	auto pMouth = new AcDbArc(AcGePoint3d(0.0, 0.5, 0.0), AcGeVector3d::kZAxis, 1.0, pi + pi * 0.3, pi + pi * 0.7);
	mouth.acquire(pMouth);
	mouth->setColorIndex(3);
	if (pBTRecord->appendAcDbEntity(mouth) != Acad::eOk) {
		throw std::runtime_error("Can't add Object to BlockRecord");
	}

}








