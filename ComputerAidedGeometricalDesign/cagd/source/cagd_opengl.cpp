#include "include/cagd_opengl.h"

// C std
#include <cmath>

// User
#include "curve_engine.h"
#include "ui_engine.h"

CAGDOpenGL *opengl = nullptr;
CAGDOpenGL *OpenGLInit(CurveEngine *curves) {
	if (!opengl) {
		opengl = new CAGDOpenGL{};
		opengl->init(curves);
	}

	return opengl;
}

void OpenGLShutDown() {
	if (!opengl) {
		return;
	}

	opengl->shutdown();
	delete opengl;
}

void CAGDOpenGL::init(CurveEngine *curves) {
	this->curves = curves;

	glfwInit();
	window = glfwCreateWindow(WIDTH, HEIGHT, "Test", 0, 0);
	if (window == nullptr) {
		fprintf(stderr, "GLFW createWindow ERROR!\n");
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "GLAD LoadGLLoader ERROR!\n");
		glfwTerminate();
		exit(-1);
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, frame_buf_size_callback);
	glfwSetCursorPosCallback(window, mouse_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &vaoPoints);
	glGenVertexArrays(1, &vaoLines);

	// Prepare shader
	program.init("res\\shaders\\vert_std.glsl", nullptr, "res\\shaders\\frag_std.glsl");
	program.use();
}

void CAGDOpenGL::shutdown() {
	glDeleteVertexArrays(1, &vaoPoints);
	glDeleteVertexArrays(1, &vaoLines);

	glfwDestroyWindow(window);
	glfwTerminate();
}

bool CAGDOpenGL::running() const {
	return !glfwWindowShouldClose(window);
}

GLFWwindow* CAGDOpenGL::getGLFWwindow() const {
	return window;
}

void CAGDOpenGL::timeIt() {
	using std::chrono::duration;
	using std::chrono::duration_cast;

	auto currentTime = high_resolution_clock::now();
	deltaTime = duration_cast<duration<float>>(currentTime - lastFrame).count();
	lastFrame = currentTime;
}

void CAGDOpenGL::processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		curves->clear();
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		curves->addCurveC0();
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		curves->spaceKnotsEq();
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		curves->smoothness[0] = false;
		curves->smoothness[1] = true;
		curves->clear();
	}

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		curves->smoothness[0] = true;
		curves->smoothness[1] = false;
		curves->clear();
	}

	if (flags.firstMove) {
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		offsetY -= 100 * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		offsetY += 100 * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		offsetX += 100 * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		offsetX -= 100 * deltaTime;
	}
}

void CAGDOpenGL::render(UIEngine *ui) {
	auto projection = glm::ortho((0.0f + offsetX), ((float)WIDTH + offsetX), ((float)HEIGHT + offsetY), (0.0f + offsetY));
	program.setMat4("MVP", projection);

	auto bgColor = ui->bgColor;
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderControlPoints(ui);

	renderCurves(ui);
}

void CAGDOpenGL::cleanup() {
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void CAGDOpenGL::clearErrors() {
	while (glGetError() != GL_NO_ERROR);
}

unsigned int CAGDOpenGL::mouseOverPoint() {
	static constexpr float maxCntPointSize = 10.f;

	for (unsigned int i = 0; i < curves->points.size(); ++i) {
		if (abs(mousePos.x - curves->points[i].x) < maxCntPointSize &&
			abs(mousePos.y - curves->points[i].y) < maxCntPointSize) {
			return i;
		}
	}

	return -1;
}

unsigned int sizeOf(const Vec<Vec4> &v) {
	return v.size() * sizeof(Vec4);
}

void* dataOf(const Vec<Vec4> &v) {
	return (void*)v.data();
}

unsigned int countOf(const Vec<Vec4> &v) {
	return v.size();
}

void CAGDOpenGL::renderControlPoints(UIEngine *ui) {
	unsigned int vboPoints;
	if (ui->showCtrlPoints) {
		glBindVertexArray(vaoPoints);

		// Generating new vbo each frame
		// Not the best idea, but otherwise we should allocate a big vbo
		// and use glBufferSubData after adding a new point
		// Probably, current solution is okay for drawing Bezier or whatever
		glGenBuffers(1, &vboPoints);
		glBindBuffer(GL_ARRAY_BUFFER, vboPoints);
		glBufferData(GL_ARRAY_BUFFER, sizeOf(curves->points), 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOf(curves->points), dataOf(curves->points));
		glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(0);

		program.setVec3("color", ui->cntPntColor);
		glPointSize(ui->cntPointSize);
		glDrawArrays(GL_POINTS, 0, countOf(curves->points));

		if (ui->connectControlPoints) {
			program.setVec3("color", ui->cntLineColor);
			glLineWidth(ui->cntLineWidth);
			glDrawArrays(GL_LINE_STRIP, 0, countOf(curves->points));
		}

		// TODO: glBufferSubData since knotsPoints.size()
		// is always < points.size()
		glDeleteBuffers(1, &vboPoints);
		glGenBuffers(1, &vboPoints);
		glBindBuffer(GL_ARRAY_BUFFER, vboPoints);
		glBufferData(GL_ARRAY_BUFFER, sizeOf(curves->knotPoints), 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOf(curves->knotPoints), dataOf(curves->knotPoints));
		glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(0);

		program.setVec3("color", ui->knotPntColor);
		glPointSize(ui->knotPointSize);
		glDrawArrays(GL_POINTS, 0, countOf(curves->knotPoints));
	}
	glDeleteBuffers(1, &vboPoints);
}

void CAGDOpenGL::renderCurves(UIEngine * ui) {
	unsigned int vboLines;
	if (countOf(curves->curvePoints) > 1) {
		glPointSize(2.f);
		glLineWidth(ui->curveWidth);

		program.setVec3("color", ui->lineColor);
		glBindVertexArray(vaoLines);

		glGenBuffers(1, &vboLines);
		glBindBuffer(GL_ARRAY_BUFFER, vboLines);
		glBufferData(GL_ARRAY_BUFFER, sizeOf(curves->curvePoints), 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOf(curves->curvePoints), dataOf(curves->curvePoints));
		glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_LINE_STRIP, 0, countOf(curves->curvePoints));
	}
	glDeleteBuffers(1, &vboLines);
}

void CAGDOpenGL::frame_buf_size_callback(GLFWwindow *window, int w, int h) {
	glViewport(0, 0, w, h);
}

void CAGDOpenGL::mouse_pos_callback(GLFWwindow *window, double xPos, double yPos) {
	auto &curves = opengl->curves;
	auto &flags = opengl->flags;
	auto &mousePos = opengl->mousePos;
	auto &offsetX = opengl->offsetX;
	auto &offsetY = opengl->offsetY;

	if (flags.firstMove) {
		mousePos.x = (float)xPos + offsetX;
		mousePos.y = (float)yPos + offsetY;
		flags.firstMove = false;
	}

	float deltaX = (float)xPos - (mousePos.x - offsetX);
	float deltaY = (mousePos.y - offsetY) - (float)yPos;

	if (flags.LCtrlDown) {
		offsetX -= deltaX;
		offsetY += deltaY;
	}

	mousePos.x = ((float)xPos + offsetX);
	mousePos.y = ((float)yPos + offsetY);

	if (flags.mouseLButtonDown && curves->holding != -1) {
		curves->modifyPoint(curves->holding, mousePos.x, mousePos.y);
	}
}

void CAGDOpenGL::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto &flags = opengl->flags;
	
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
		flags.LCtrlDown = true;
	}

	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
		flags.LCtrlDown = false;
	}
}

void CAGDOpenGL::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	auto &curves = opengl->curves;
	auto &flags = opengl->flags;
	auto &mousePos = opengl->mousePos;
	
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		int idx = opengl->mouseOverPoint();
		if (idx != -1) {
			curves->erasePointAt((unsigned int)idx);
		} else {
			curves->erasePointAt(curves->points.size() - 1);
		}

		return;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (ImGui::IsAnyWindowHovered()) {
			return;
		}

		if (flags.mouseLButtonDown) {
			return;
		}

		flags.mouseLButtonDown = true;

		int idx = opengl->mouseOverPoint();
		if (idx != -1) {
			curves->holding = idx;
			curves->modifyPoint(idx, mousePos.x, mousePos.y);
			return;
		}

		// Check if the new point is between two control points in order not to add 
		// it as the last one
		auto &points = curves->points;
		for (unsigned int i = 0; points.size() > 1 && i < points.size() - 1; ++i) {
			Vec4 mouse = { mousePos.x, mousePos.y, 0.f, 0.f };
			if (glm::distance(points[i], mouse) + glm::distance(mouse, points[i + 1]) - glm::distance(points[i], points[i + 1]) <= 1e-1f) {
				curves->insertPointAt(i + 1, mousePos.x, mousePos.y);
				curves->holding = i;
				return;
			}
		}

		// we've clicked on an empty space, so create new vertex
		curves->holding = curves->addPointAt(mousePos.x, mousePos.y);
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		flags.mouseLButtonDown = false;
		curves->holding = -1;
	}

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
		flags.mouseMiddleDown = true;
	}

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
		flags.mouseMiddleDown = false;
	}
}

void CAGDOpenGL::window_size_callback(GLFWwindow* window, int width, int height) {
	opengl->WIDTH = width;
	opengl->HEIGHT = height;
}

// testing scrollback
void CAGDOpenGL::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	auto &offsetX = opengl->offsetX;
	auto &offsetY = opengl->offsetY;
	auto &zoom = opengl->zoom;

	zoom = min(max(0.1f, zoom + float(offsetY) * 5 * opengl->deltaTime), 10.f);
}
