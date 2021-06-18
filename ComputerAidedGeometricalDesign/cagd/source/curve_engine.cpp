#include "curve_engine.h"

#include "include/casteljau.h"

void CurveEngine::addPointRoutine(int idx, const Vec4 &point) {
	curveChange = true;
	if (knotsIdxs.empty()) {
		knotsIdxs.push_back(0);
	}

	if (idx == -1) {
		points.push_back(point);

		if (smoothness[1] && points.size() > 2) {
			if (knots.empty()) { // u_0 and u_last are always 0.f and 1.f
				knots.push_back(0.f);
				knots.push_back(1.f);
			} else {
				knots.back() = min(max(0.f, ((1.f - knots[knots.size() - 2]) / 2) + knots[knots.size() - 2]), 1.f);
				knots.push_back(1.f);
			}
		}
	} else {
		if (smoothness[1] && points.size() > 2) {
			if (idx <= 2) {
				knots.insert(knots.begin() + 1, knots[1] / 2.f);
			} else if (idx == points.size() - 1) {
				knots.back() = (knots[knots.size() - 2] + (1.f - knots[knots.size() - 2]) / 2.f);
				knots.push_back(1.f);
			} else {
				knots.insert(knots.begin() + (idx - 1), (knots[idx - 2] + knots[idx - 1]) / 2.f);
			}
		}

		points.insert(points.begin() + idx, point);
	}
}

int CurveEngine::addPointAt(float x, float y) {
	addPointRoutine(-1, { x, y, 0.f, 0.f });

	return points.size() - 1;
}

unsigned int CurveEngine::leastOrEqualKnotIdx(unsigned int idx) {
	unsigned int i = -1; // result

	unsigned int l = 0;
	unsigned int r = knotsIdxs.size();
	while (l <= r) {
		i = (l + r) / 2;
		if (knotsIdxs[i] == idx) {
			break;
		}

		if (knotsIdxs[i] > idx) {
			if (i == 0 || knotsIdxs[i - 1] < idx) break;
			r = i - 1;
		} else {
			l = i + 1;
		}
	}

	assert(i >= 0 && i < knotsIdxs.size());

	return i;
}

void CurveEngine::insertPointAt(unsigned int idx, float x, float y) {
	addPointRoutine(idx, { x, y, 0.f, 0.f });

	if (idx > knotsIdxs.back()) {
		return;
	}

	unsigned int i = leastOrEqualKnotIdx(idx);

	for (; i < knotsIdxs.size(); ++i) {
		++knotsIdxs[i];
	}
}

void CurveEngine::erasePointAt(unsigned int idx) {
	if (points.empty()) {
		return;
	}

	if (smoothness[1] && !knots.empty()) {
		knots.erase(knots.begin() + (knots.size() - 1));

		if (!knotPoints.empty()) {
			knotPoints.erase(knotPoints.begin() + (knotPoints.size() - 1));
		}

		if (!knots.empty()) {
			knots.back() = 1.f;
		}
	}

	curveChange = true;

	points.erase(points.begin() + idx);

	if (idx > knotsIdxs.back()) {
		return;
	}

	unsigned int i = leastOrEqualKnotIdx(idx);

	if (knotsIdxs[i] == idx) {
		if (i != 0 || (knotsIdxs.size() > 1 && knotsIdxs[1] == 1)) {
			knotsIdxs.erase(knotsIdxs.begin() + i);
		} else {
			knotsIdxs[0] = 1;
		}
	}

	for (; i < knotsIdxs.size(); ++i) {
		--knotsIdxs[i];
	}
}

void CurveEngine::modifyPoint(int idx, float x, float y) {
	curveChange = true;

	points[idx].x = x;
	points[idx].y = y;
}

void CurveEngine::spaceKnotsEq() {
	if (!smoothness[1]) {
		return;
	}

	curveChange = true;

	float dist = 1.f / (knots.size() - 1);
	knots[0] = 0.f;
	for (unsigned int i = 1; i < knots.size() - 1; ++i) {
		knots[i] = knots[i - 1] + dist;
	}
	knots.back() = 1.f;
}

void CurveEngine::calculateCurveC0(const Vec<Vec4> &points) {
	for (unsigned int i = 0; i < knotsIdxs.size(); ++i) {
		unsigned int startIdx = knotsIdxs[i];
		unsigned int endIdx = (i == knotsIdxs.size() - 1 ? points.size() : knotsIdxs[i + 1] + 1);
		float delta = 1.f / glm::distance(points[startIdx], points[endIdx - 1]);

		for (float t = 0.f; t <= 1.f; t += delta) {
			curvePoints.push_back(casteljau(points, startIdx, endIdx, t));
		}
	}
}

void CurveEngine::calculateCurveC1() {
	if (points.size() < 4) {
		calculateCurveC0(points);
		return;
	}

	Vec<Vec4> pointsC1(2 * points.size() - 3);
	knotPoints.clear();
	knotsIdxs.clear();
	knotsIdxs.push_back(0);

	// TODO: separate fors
	unsigned int i = 0;
	for (; i < pointsC1.size(); ++i) {
		if (i <= 1) {
			pointsC1[i] = points[i];
		} else if (i >= pointsC1.size() - 2) {
			pointsC1[i] = points[points.size() - (pointsC1.size() - i)];
		} else if (i % 2 == 1) {
			// here knots.size() represents the count of added points
			pointsC1[i] = points[i - knotsIdxs.size() + 1];
		} else {
			float delta_i = knots[i / 2 + 1] - knots[i / 2];
			float delta_i1 = knots[i / 2] - knots[i / 2 - 1];
			float sum = delta_i + delta_i1;
			Vec4 b_i = (delta_i / sum) * points[i / 2] + (delta_i1 / sum) * points[i / 2 + 1];
			pointsC1[i] = b_i;
			knotsIdxs.push_back(i);
			knotPoints.push_back(b_i);
		}
	}

	calculateCurveC0(pointsC1);
}

void CurveEngine::calculateCurve() {
	curvePoints.clear();

	if (points.size() < 2) {
		return;
	}

	if (smoothness[0]) {
		calculateCurveC0(points);
	}

	if (smoothness[1]) {
		calculateCurveC1();
	}

	if (smoothness[2]) {

	}

	curveChange = false;
}

void CurveEngine::addCurveC0() {
	if (!smoothness[0]) {
		return;
	}

	if ((points.size() < 2) || ((points.size() - 1) == knotsIdxs.back())) {
		return;
	}

	knotsIdxs.push_back(points.size() - 1);
}

void CurveEngine::clear() {
	curveChange = true;
	points.clear();
	knotsIdxs.clear();
	curvePoints.clear();
	knotPoints.clear();
	knots.clear();
}

unsigned int CurveEngine::pointsCount() const {
	return 0;
}

unsigned int CurveEngine::pointsSize() const {
	return 0;
}

void * CurveEngine::pointsData() const {
	return nullptr;
}

unsigned int CurveEngine::knotPointsCount() const {
	return 0;
}
