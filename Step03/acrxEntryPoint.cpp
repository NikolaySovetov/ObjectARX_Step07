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
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "utilities.h"
#include <stdexcept>

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

EdEmployeeReactor* pEdEmployeeReactor = nullptr;
ObEmployeeReactor* pObEmployeeReactor = nullptr;


//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep03App : public AcRxArxApp {

public:
	CStep03App() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here
		pEdEmployeeReactor = new EdEmployeeReactor(true);
		pObEmployeeReactor = new ObEmployeeReactor;

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void* pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here
		detachAllEmployeeReactors();
		delete pEdEmployeeReactor;
		delete pObEmployeeReactor;

		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

	static void Step03_createBlock() {
		TCHAR strBlockName[128];
		if (acedGetString(0, _T("Enter BlockRecord name: "), strBlockName) != RTNORM) {
			return;
		}

		try {
			SymbolTableWrapper BTable;
			AcDbBlockTable* pBTable = BTable.GetBlockTable(AcDb::kForWrite);

			if (pBTable->has(strBlockName)) {
				acutPrintf(_T("\nWarning: %s block exists"), strBlockName);
				return;
			}

			AddEmployeeBlockRecord(pBTable, strBlockName);
		}
		catch (const std::exception& e) {
			acutPrintf(_T("\nException: %s"), e.what());
		}
	}

	static void Step03_createLayer() {
		TCHAR strLayerName[128];
		if (acedGetString(0, _T("Enter BlockRecord name: "), strLayerName) != RTNORM) {
			return;
		}

		try {
			SymbolTableWrapper layerTable;
			AcDbLayerTable* pLayerTable = layerTable.GetLayerTable(AcDb::OpenMode::kForWrite);

			if (pLayerTable->has(strLayerName)) {
				acutPrintf(_T("\nWarning: %s layer exists"), strLayerName);
				return;
			}

			AcDbLayerTableRecordPointer pTRecord;
			pTRecord.create();
			pTRecord->setName(strLayerName);
			if (pLayerTable->add(pTRecord) != Acad::eOk) {
				acutPrintf(_T("\nError: Can't add %s layer"), strLayerName);
				return;
			}
		}
		catch (const std::exception& e) {
			acutPrintf(_T("\nException: %s"), e.what());
		}

		acutPrintf(_T("\nEvent: %s layer created"), strLayerName);
	}

	virtual AcRx::AppRetCode On_kLoadDwgMsg(void* pkt) {
		AcRx::AppRetCode retCode = AcRxArxApp::On_kLoadDwgMsg(pkt);

		DocVars.docData().m_pDbEmlpoyeeReactor = 
			new DbEmployeeReactor(acdbHostApplicationServices()->workingDatabase());
		return retCode;
	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep03App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, Step03, _createBlock, createBlock, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, Step03, _createLayer, createLayer, ACRX_CMD_TRANSPARENT, NULL)
