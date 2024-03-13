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

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep03App : public AcRxArxApp {

public:
	CStep03App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	static void Step03_createBlockRecord() {
		try	{
			BlockTableWrapper BTable(AcDb::kForWrite);
			const TCHAR* strBlockName{ L"Employee" };

			if (BTable.Get()->has(strBlockName)) {
				acutPrintf(_T("\nWarning: %s block exists"), strBlockName);
				return;
			}

			auto pBTRecord{ std::make_unique<AcDbBlockTableRecord>() };
			BlockTableRecordWrapper BTRecord(pBTRecord.get());
			BTRecord.Get()->setOrigin(AcGePoint3d::kOrigin);
			BTRecord.Get()->setName(strBlockName);
			
			BTable.Add(pBTRecord);

			std::unique_ptr<AcDbEntity> face{ std::make_unique<AcDbCircle>
				(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, 1.0) };
			face->setColorIndex(2);
			BTRecord.Add(face);

			std::unique_ptr<AcDbEntity> rightEye{ std::make_unique<AcDbCircle>
				(AcGePoint3d(0.33, 0.25, 0), AcGeVector3d::kZAxis, 0.1) };
			rightEye->setColorIndex(4);
			BTRecord.Add(rightEye);

			std::unique_ptr<AcDbEntity> leftEye{ std::make_unique<AcDbCircle>
				(AcGePoint3d(-0.33, 0.25, 0), AcGeVector3d::kZAxis, 0.1) };
			leftEye->setColorIndex(4);
			BTRecord.Add(leftEye);

			const double pi{ 3.1415926 };
			std::unique_ptr<AcDbEntity> mouth{ std::make_unique<AcDbArc>
				(AcGePoint3d(0.0, 0.5, 0.0), AcGeVector3d::kZAxis, 1.0, pi + pi*0.3, pi + pi * 0.7) };
			mouth->setColorIndex(1);
			BTRecord.Add(mouth);
		}
		catch (const std::exception& e) {
			acutPrintf(_T("\nException: %s"), e.what());
		}
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep03App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, Step03, _createBlockRecord, createBlockRecord, ACRX_CMD_TRANSPARENT, NULL)

