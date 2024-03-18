// (C) Copyright 2002-2007 by Autodesk, Inc. 
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
//----- ObEmployeeReactor.cpp : Implementation of ObEmployeeReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ObEmployeeReactor.h"

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(ObEmployeeReactor, AcDbObjectReactor, 1)

//-----------------------------------------------------------------------------
ObEmployeeReactor::ObEmployeeReactor() : AcDbObjectReactor() {
}

//-----------------------------------------------------------------------------
ObEmployeeReactor::~ObEmployeeReactor () {
}

void ObEmployeeReactor::openedForModify(const AcDbObject* pObject) {
	if (DocVars.docData().m_doRepositioning) {
		return;
	}

	if (!DocVars.docData().m_editCommand) {
		return;
	}

	AcDbBlockReference* pBlockReference{ AcDbBlockReference::cast(pObject) };
	if (!pBlockReference) {
		return;
	}

	AcDbObjectId objectId{ pBlockReference->blockTableRecord() };
	AcDbBlockTableRecord* pBTRecord;
	if (acdbOpenObject(pBTRecord, objectId, AcDb::kForRead) != Acad::eOk) {
		acutPrintf(L"\nCan't open block table record!"); 
		return;
	}

	const TCHAR* strBlockName;
	pBTRecord->getName(strBlockName);
	pBTRecord->close();

	if (_tcscmp(L"EMPLOYEE", strBlockName) != 0) {
		acutPrintf(L"\nObject is not \"Employee\"");
		return;
	}

	DocVars.docData().m_changedObjects.append(pBlockReference->objectId());
	DocVars.docData().m_employeePositions.append(pBlockReference->position());
}
