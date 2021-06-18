#pragma once

#include "common_defines.h"

struct CurveEngine {
public:
	Vec<Vec4> points = {};
	Vec<Vec4> curvePoints = {};
	Vec<Vec4> knotPoints = {};
	Vec<unsigned int> knotsIdxs = {};
	Vec<float> knots = {};

	int holding = -1;

	bool smoothness[2] = { true, false };
	bool curveChange = true;

public:
	int addPointAt(float x, float y);
	void insertPointAt(unsigned int, float x, float y);
	void erasePointAt(unsigned int);
	void modifyPoint(int, float x, float y);
	void spaceKnotsEq();
	void calculateCurve();
	void addCurveC0();
	void clear();

	unsigned int pointsCount() const;
	unsigned int pointsSize() const;
	void* pointsData() const;

	unsigned int knotPointsCount() const;
	unsigned int knotPointsSize() const;
	void* knotPointsData() const;

	unsigned int curvePointsCount() const;
	unsigned int curvePointsSize() const;
	void* curvePointsData() const;

private:
	void addPointRoutine(int idx, const Vec4 &point);
	unsigned int leastOrEqualKnotIdx(unsigned int idx);
	void calculateCurveC0(const Vec<Vec4> &points);
	void calculateCurveC1();
};
