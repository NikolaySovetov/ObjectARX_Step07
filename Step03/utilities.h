#pragma once
#include "StdAfx.h"
#include <memory>

class BlockTableWrapper {
private:
	AcDbBlockTable* m_pBlockTable{};

public:
	BlockTableWrapper(AcDb::OpenMode mode);
	~BlockTableWrapper();
	AcDbBlockTable* Get();
	void Add(std::unique_ptr<AcDbBlockTableRecord>& cBlockTableRecord);
};

class BlockTableRecordWrapper {
private:
	AcDbBlockTableRecord* m_pBlockTableRecord{};

public:
	BlockTableRecordWrapper(AcDbBlockTableRecord*);
	~BlockTableRecordWrapper();
	AcDbBlockTableRecord* Get();
	void Add(std::unique_ptr<AcDbEntity>& pEntity);

	// 	template<typename T>
//	void Add(std::unique_ptr<T>& pEntity) {
//		if (m_pBlockTableRecord->appendAcDbEntity(pEntity.get()) != Acad::eOk) {
//			throw std::runtime_error("Can't add an Entity to BlockTableRecord");
//		}
//		pEntity.release();
//	}
};

//-------------------------------------------------------------------------
class CircleWrapper {
public:
	std::unique_ptr<AcDbCircle> m_pCircle;

public:
	CircleWrapper(const AcGePoint3d& center, const AcGeVector3d& normal, double radius);
	std::unique_ptr<AcDbCircle> Get();
	~CircleWrapper();
};

class ArcWrapper {
public:
	std::unique_ptr<AcDbArc> m_pArc;

public:
	ArcWrapper(const AcGePoint3d& center, const AcGeVector3d& normal,
		double radius, double startRadius, double endRadius);
	~ArcWrapper();
	std::unique_ptr<AcDbArc> Get();
};








