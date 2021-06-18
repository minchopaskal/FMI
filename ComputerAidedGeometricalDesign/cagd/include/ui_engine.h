#pragma once

#include "common_headers.h"
#include "common_defines.h"

struct CAGDOpenGL;
struct CurveEngine;

struct UIEngine {
private:
	CurveEngine *curves = nullptr;

public:
	glm::vec3 bgColor = { 0.2f, 0.0f, 0.0f };
	glm::vec3 lineColor = { 0.f, 0.f, 0.f };
	glm::vec3 cntPntColor = { 1.f, 1.f, 0.f };
	glm::vec3 cntLineColor = { 1.f, 1.f, 0.f };
	glm::vec3 knotPntColor = { 1.f, 0.f, 0.f };

	float zoom = 1.f; // not working
	float cntPointSize = 6.f;
	float cntLineWidth = 1.f;
	float knotPointSize = 4.f;
	float curveWidth = 3.f;

	bool connectControlPoints = true;
	bool showKeyCtrls = false;
	bool showCtrlPoints = true;

public:

	void drawUI(); // must be called before this->render()
	void render(); // must be called before opengl->render()
	void renderDrawData(); // must be called after opengl->render()

private:
	void drawColorEdit(const String &label, const glm::vec3 &arr);
	bool drawRadioGroup(const Vec<String> &labels, bool *vals);

	void drawMainWindow();
	void drawControls();

	void init(CurveEngine *curves, GLFWwindow *window);
	void shutdown();

	friend UIEngine* UIInit(CurveEngine *curves, GLFWwindow *window);
	friend void UIShutDown();
};

UIEngine* UIInit(CurveEngine *curves, GLFWwindow *window);
void UIShutDown();
