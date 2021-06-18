#pragma once

// C++ std
#include <chrono>

#include "include/common_headers.h"
#include "include/common_defines.h"

#include "include/shader.h"

struct CurveEngine;
struct UIEngine;
using std::chrono::high_resolution_clock;

struct CAGDOpenGL {
private:
	CurveEngine *curves = nullptr;
	GLFWwindow *window;

	unsigned int WIDTH = 1280, HEIGHT = 768;
	float zoom = 1.f; // not working
	float offsetX = 0.f, offsetY = 0.f;
	struct {
		float x, y;
	} mousePos = { 640.f, 384.f };
	
	high_resolution_clock::time_point lastFrame{};
	float deltaTime;

	Shader program;
	unsigned int vaoPoints;
	unsigned int vaoLines;
public:
	struct {
		bool firstMove : 1;
		bool LCtrlDown : 1;
		bool mouseLButtonDown : 1;
		bool mouseMiddleDown : 1;
	} flags = { 1, 0, 0, 0 };

public:
	bool running() const;

	GLFWwindow* getGLFWwindow() const;

	void timeIt();
	void processInput();

	void render(UIEngine *ui);
	void cleanup();

private:
	void init(CurveEngine *curves);
	void shutdown();

	void clearErrors();
	unsigned int mouseOverPoint();

	void renderControlPoints(UIEngine *ui);
	void renderCurves(UIEngine *ui);

	// GLFW callbacks
	static void frame_buf_size_callback(GLFWwindow *window, int w, int h);
	static void mouse_pos_callback(GLFWwindow*, double xPos, double yPos);
	static void scroll_callback(GLFWwindow*, double, double);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow*, int button, int action, int mods);
	static void window_size_callback(GLFWwindow* window, int width, int height);

	friend CAGDOpenGL* OpenGLInit(CurveEngine *curves);
	friend void OpenGLShutDown();
};

CAGDOpenGL* OpenGLInit(CurveEngine *curves);
void OpenGLShutDown();