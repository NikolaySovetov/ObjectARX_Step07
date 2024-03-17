// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- DocData.cpp : Implementation file
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "utilities.h"

//-----------------------------------------------------------------------------
//----- The one and only document manager object. You can use the DocVars object to retrieve
//----- document specific data throughout your application
AcApDataManager<CDocData> DocVars;

//-----------------------------------------------------------------------------
//----- Implementation of the document data class.
CDocData::CDocData() {
	//m_editCommand = false;
	//m_doRepositioning = false;

	attachEmployeeReactorToAllEmployee(true);
}

//-----------------------------------------------------------------------------
CDocData::CDocData(const CDocData& data) {
}

//-----------------------------------------------------------------------------
CDocData::~CDocData() {
}

Acad::ErrorStatus attachEmployeeReactorToAllEmployee(bool attach) {
	Acad::ErrorStatus es;

	AcDbBlockTable* pBTable;
	es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBTable, AcDb::kForRead);
	if (es != Acad::eOk) {
		acutPrintf(L"\nError: Can't open BlockTable");
		return es;
	}
//	AcDbSymbolTablePointer<AcDbBlockTable> stp;
//	stp.acquire(pBTable);

	AcDbBlockTableRecord* pBTRecordForModelSpace;
	es = pBTable->getAt(ACDB_MODEL_SPACE, pBTRecordForModelSpace, AcDb::kForRead);
	pBTable->close();

	if (es != Acad::eOk) {
		acutPrintf(L"\nError: Can't get BlockTableRecord for MODEL_SPACE");
		return es;
	}

	AcDbBlockTableRecordIterator* pIterator;
	es = pBTRecordForModelSpace->newIterator(pIterator);
	if (es != Acad::eOk) {
		acutPrintf(L"\nError: Can't create new iterator of BlockTableRecord");
		return es;
	}
	std::unique_ptr<AcDbBlockTableRecordIterator>pBTRIterator(pIterator);


	for (; !pBTRIterator->done(); pBTRIterator->step()) {
		AcDbEntity* pEntity;

		es = pBTRIterator->getEntity(pEntity, AcDb::kForRead);
		if (es != Acad::eOk) {
			acutPrintf(L"\nWarning: Can't get AcDbEntity from Iterator");
			continue;
		}

		AcDbBlockReference* pBReference = AcDbBlockReference::cast(pEntity);
		if (!pBReference) {
			pEntity->close();
			acutPrintf(L"\nEvent: Entity not the BlockReference");
			continue;
		}

		AcDbObjectId objectId = pBReference->blockTableRecord();

		AcDbBlockTableRecord* pBlockTableRecord;
		es = acdbOpenAcDbObject((AcDbObject*&)pBlockTableRecord, objectId, AcDb::kForRead);
		if (es != Acad::eOk) {
			acutPrintf(_T("\nCannot open block table record!"));
			pEntity->close();
			break;
		}

		const TCHAR* strBlockName;
		pBlockTableRecord->getName(strBlockName);
		pBlockTableRecord->close();

		if (_tcscmp(strBlockName, _T("EMPLOYEE")) != 0) {
			pEntity->close();
			continue;
		}

		if (attach) {
			pEntity->addReactor(pObEmployeeReactor);
		}
		else {
			pEntity->removeReactor(pObEmployeeReactor);
		}
		pEntity->close();
	}

	pBTRecordForModelSpace->close();

	return Acad::eOk;
}

void detachAllEmployeeReactors() {
	AcApDocumentIterator* pDocIt = acDocManager->newAcApDocumentIterator();
	if (pDocIt == NULL) {
		return;
	}
	std::unique_ptr<AcApDocumentIterator> pDocIterator(pDocIt);

	AcApDocument* pCurrentDocContext = acDocManager->curDocument();


	for (; !pDocIterator->done(); pDocIterator->step()) {
		AcApDocument* pDoc = pDocIterator->document();
		if (pDoc->lockMode() == AcAp::kNone) {
			if (acDocManager->setCurDocument(pDoc, AcAp::kAutoWrite, Adesk::kFalse) == Acad::eOk) {
				attachEmployeeReactorToAllEmployee(false);
				acDocManager->unlockDocument(pDoc);
			}
		}
		else {
			acDocManager->setCurDocument(pDoc);
			attachEmployeeReactorToAllEmployee(false);
		}
	}

	acDocManager->setCurDocument(pCurrentDocContext, AcAp::kNone, Adesk::kFalse);
}










