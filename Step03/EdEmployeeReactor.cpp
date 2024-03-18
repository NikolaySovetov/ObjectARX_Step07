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
//----- EdEmployeeReactor.cpp : Implementation of EdEmployeeReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "EdEmployeeReactor.h"

//-----------------------------------------------------------------------------
EdEmployeeReactor::EdEmployeeReactor(const TCHAR* strBlockName,
	const bool autoInitAndRelease) : AcEditorReactor(), mbAutoInitAndRelease(autoInitAndRelease) {

	m_strBlockName = strBlockName;
	
	if (autoInitAndRelease) {
		if (acedEditor)
			acedEditor->addReactor(this);
		else
			mbAutoInitAndRelease = false;
	}
}

//-----------------------------------------------------------------------------
EdEmployeeReactor::~EdEmployeeReactor() {
	Detach();
}

//-----------------------------------------------------------------------------
void EdEmployeeReactor::Attach() {
	Detach();
	if (!mbAutoInitAndRelease) {
		if (acedEditor) {
			acedEditor->addReactor(this);
			mbAutoInitAndRelease = true;
		}
	}
}

void EdEmployeeReactor::Detach() {
	if (mbAutoInitAndRelease) {
		if (acedEditor) {
			acedEditor->removeReactor(this);
			mbAutoInitAndRelease = false;
		}
	}
}

AcEditor* EdEmployeeReactor::Subject() const {
	return (acedEditor);
}

bool EdEmployeeReactor::IsAttached() const {
	return (mbAutoInitAndRelease);
}

//-----------------------------------------------------------------------------
void EdEmployeeReactor::commandWillStart(const ACHAR* strCommand) {
	if (_tcscmp(strCommand, L"MOVE") == 0 ||
		_tcscmp(strCommand, L"ROTATE") == 0 ||
		_tcscmp(strCommand, L"STRETCH") == 0 ||
		_tcscmp(strCommand, L"SCALE") == 0 ||
		_tcscmp(strCommand, L"GRIP_STRETCH") == 0)
	{
		DocVars.docData().m_editCommand = true;
		DocVars.docData().m_doRepositioning = false;

		DocVars.docData().m_changedObjects.setLogicalLength(0);
		DocVars.docData().m_employeePositions.setLogicalLength(0);
	}
}

void EdEmployeeReactor::commandEnded(const ACHAR* /*cmdStr*/) {
	if (!DocVars.docData().m_editCommand) {
		return;
	}

	DocVars.docData().m_editCommand = false;
	DocVars.docData().m_doRepositioning = true;

	int length{ DocVars.docData().m_changedObjects.length() };
	for (int i{ 0 }; i < length; ++i) {
		ResetToPreviousPosition(i);
	}
}

inline void ResetToPreviousPosition(int nIndex) {
	AcDbBlockReference* pBReference;
	if (acdbOpenObject(pBReference, DocVars.docData().m_changedObjects.at(nIndex), AcDb::kForWrite)
		!= Acad::eOk) {
		return;
	}
	AcGePoint3d newPosition = pBReference->position();
	AcGePoint3d oldPosition = DocVars.docData().m_employeePositions.at(nIndex);
	if (newPosition != oldPosition) {
		pBReference->setPosition(oldPosition);
	}
	pBReference->close();
}


