#include <cmath>

#include <algorithm>
#include <chrono>
#include <memory>
#include <vector>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "include/imgui.h"
#include "include/imgui_impl_glfw.h"
#include "include/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/Shader.h"
#include "casteljau.h"

#define max(x, y) (x) > (y) ? (x) : (y)
#define min(x, y) (x) < (y) ? (x) : (y)

using namespace std::chrono;

template <class T>
using Vec = std::vector<T>;
using Vec4 = glm::vec<4, float>;

// GLFW callbacks
void frame_buf_size_callback(GLFWwindow *window, int w, int h);
void processInput(GLFWwindow *window);
void mouse_pos_callback(GLFWwindow*, double xPos, double yPos);
void scroll_callback(GLFWwindow*, double, double);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow*, int button, int action, int mods);
void window_size_callback(GLFWwindow* window, int width, int height);

// Spline control helper functions
int addPointAtMouseLocation();
int addPointAt(float x, float y);
void insertPointAt(unsigned int);
void erasePointAt(unsigned int);
void modifyPoint(int);
void clearErrors();
void drawUI();
void calculateCurve();
void addCurve();
void clear();

// WARNING: GLOBAL VARIABLES (for state)
int WIDTH = 1200;
int HEIGHT = 768;
float offsetX, offsetY;

struct {
	float x = WIDTH / 2.f;
	float y = HEIGHT / 2.f;
} mousePos;

static float bgColor[3] = { 0.2f, 0.0f, 0.0f };
static glm::vec3 lineColor = { 0.f, 0.f, 0.f };
static glm::vec3 cntPntColor = { 1.f, 1.f, 0.f };
static glm::vec3 cntLineColor = { 1.f, 1.f, 0.f };
static glm::vec3 knotPntColor = { 1.f, 0.f, 0.f };
float deltaTime = 0.f;
float zoom = 1.f;

int holding = -1;
bool firstMove = true;
bool mouseLButtonDown = false;
bool LCtrlDown = false;
bool curveChange = false;
bool connectControlPoints = true;
bool showKeyCtrls = false;
bool showCtrlPoints = true;

high_resolution_clock::time_point lastFrame{};
Vec<Vec4> points;
Vec<Vec4> curvePoints;
Vec<Vec4> knotPoints;

// Smoothness - C^r
bool smoothness[3] = { true, false, false };
// if r == 0 knots are control points
Vec<unsigned int> knotsIdxs;
// if r > 0 we need parameters for the knots
Vec<float> knots;

int main(int argc, char **argv)
{
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Test", 0, 0);
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Prepare VAOs
	unsigned int vao;
	unsigned int vboPoints;
	glGenVertexArrays(1, &vao);
	
	unsigned int vaoLines;
	unsigned int vboLines;
	glGenVertexArrays(1, &vaoLines);

	// Prepare shader
	Shader program = Shader("res\\shaders\\vert_std.glsl", nullptr, "res\\shaders\\frag_std.glsl");
	program.use();
	glm::mat4 projection;

	// Prepare ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = duration_cast<duration<float>>(currentTime - lastFrame).count();
		lastFrame = currentTime;
		
		projection = glm::ortho((0.0f + offsetX), ((float)WIDTH + offsetX), ((float)HEIGHT + offsetY), (0.0f + offsetY));
		auto MVP = projection; // *view * model;
		// TODO: uniform buffer objects
		program.setMat4("MVP", MVP);
			
		processInput(window);

		drawUI();

		if (curveChange) {
			calculateCurve();
		}
		
		ImGui::Render();
		glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.f);
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (showCtrlPoints) {
			glBindVertexArray(vao);

			// Generating new vbo each frame
			// Not the best idea, but otherwise we should allocate a big vbo
			// and use glBufferSubData after adding a new point
			// Probably, current solution is okay for drawing Bezier or whatever
			glGenBuffers(1, &vboPoints);
			glBindBuffer(GL_ARRAY_BUFFER, vboPoints);
			glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vec4), 0, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(Vec4), (void*)points.data());
			glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
			glEnableVertexAttribArray(0);

			program.setVec3("lineColor", cntPntColor);
			glPointSize(6.f);
			glDrawArrays(GL_POINTS, 0, points.size());
			
			if (connectControlPoints) {
				program.setVec3("lineColor", cntLineColor);
				glLineWidth(1);
				glDrawArrays(GL_LINE_STRIP, 0, points.size());
			}

			glDeleteBuffers(1, &vboPoints);
			glGenBuffers(1, &vboPoints);
			glBindBuffer(GL_ARRAY_BUFFER, vboPoints);
			glBufferData(GL_ARRAY_BUFFER, knotPoints.size() * sizeof(Vec4), 0, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, knotPoints.size() * sizeof(Vec4), (void*)knotPoints.data());
			glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
			glEnableVertexAttribArray(0);

			program.setVec3("lineColor", knotPntColor);
			glPointSize(4.f);
			glDrawArrays(GL_POINTS, 0, knotPoints.size());
		}
		
		if (curvePoints.size() > 1) {
			glPointSize(2.f);
			glLineWidth(3);

			program.setVec3("lineColor", lineColor);
			glBindVertexArray(vaoLines);

			glGenBuffers(1, &vboLines);
			glBindBuffer(GL_ARRAY_BUFFER, vboLines);
			glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(Vec4), 0, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, curvePoints.size() * sizeof(Vec4), (void*)curvePoints.data());
			glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
			glEnableVertexAttribArray(0);
		
			glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size());
		}

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(window);

		glDeleteVertexArrays(1, &vboLines);
		glDeleteBuffers(1, &vboPoints);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	glDeleteVertexArrays(1, &vao);

	return 0;
}

void frame_buf_size_callback(GLFWwindow *window, int w, int h) {
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		clear();
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		addCurve();
	}

	if (firstMove) {
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

void mouse_pos_callback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMove)
	{
		mousePos.x = (float) xPos + offsetX;
		mousePos.y = (float) yPos + offsetY;
		firstMove = false;
	}

	float deltaX = (float)xPos - (mousePos.x - offsetX);
	float deltaY = (mousePos.y - offsetY) - (float) yPos;

	if (LCtrlDown) {
		offsetX -= deltaX;
		offsetY += deltaY;
	}

	mousePos.x = ((float) xPos + offsetX);
	mousePos.y = ((float) yPos + offsetY);

	if (holding != -1) {
		modifyPoint(holding);
	}

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
		LCtrlDown = true;
	}

	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
		LCtrlDown = false;
	}
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		for (unsigned int i = 0; i < points.size(); ++i) {
			if (abs(mousePos.x - points[i].x) < 6.f &&
				abs(mousePos.y - points[i].y) < 6.f) {
				erasePointAt(i);
				return;
			}
		}

		erasePointAt(points.size() - 1);
		return;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (ImGui::IsAnyWindowHovered()) {
			return;
		}

		if (mouseLButtonDown) {
			return;
		}

		mouseLButtonDown = true;

		for (unsigned int i = 0; i < points.size(); ++i) {
			if (abs(mousePos.x - points[i].x) < 6.f &&
				abs(mousePos.y - points[i].y) < 6.f) {
				// we've clicked on an existing point
				holding = i;
				modifyPoint(holding);
				return;
			}
		}

		// Check if the new point is between two control points in order not to add 
		// it as the last one
		for (unsigned int i = 0; points.size() > 1 && i < points.size() - 1; ++i) {
			Vec4 mouse = { mousePos.x, mousePos.y, 0.f, 0.f };
			if (glm::distance(points[i], mouse) + glm::distance(mouse, points[i + 1]) - glm::distance(points[i], points[i + 1]) <= 1e-1f) {
				insertPointAt(i + 1);
				return;
			}
		}

		// we've clicked on an empty space, so create new vertex
		holding = addPointAtMouseLocation();
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		mouseLButtonDown = false;
		holding = -1;
	}

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
		printf("Mouse middle pressed!\n");
		LCtrlDown = true;
	}

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
		printf("Mouse middle released!\n");
		LCtrlDown = false;
	}
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	WIDTH = width;
	HEIGHT = height;
}

// testing scrollback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoom = min(max(0.1f, zoom + float(yoffset) * 5 * deltaTime), 10.f);
}

void addPointRoutine(int idx, const Vec4 &point) {
	curveChange = true;
	if (knotsIdxs.empty()) {
		knotsIdxs.push_back(0);
	}

	if (idx == -1) {
		points.push_back(point);

		if (smoothness[1] && points.size() > 2) {
			if (knots.empty()) {
				knots.push_back(0.f);
				knots.push_back(1.f);
			} else {
				knots.back() = min(max(0.f, ((1.f - knots[knots.size() - 2]) / 2) + knots[knots.size() - 2]), 1.f);
				knots.push_back(1.f);
			}
		}

		// TODO: add knots for C2

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


		// TODO: insert knot in the right spot for C1
		// TODO: ==||== for C2
	}
}

int addPointAtMouseLocation() {
	addPointRoutine(-1, { mousePos.x, mousePos.y, 0.f, 0.f });

	return points.size() - 1;
}

int addPointAt(float x, float y) {
	addPointRoutine(-1, { x, y, 0.f, 0.f });

	return points.size() - 1;
}

unsigned int leastOrEqualKnotIdx(unsigned int idx) {
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

void insertPointAt(unsigned int idx) {
	addPointRoutine(idx, { mousePos.x, mousePos.y, 0.f, 0.f });

	if (idx > knotsIdxs.back()) {
		return;
	}

	unsigned int i = leastOrEqualKnotIdx(idx);

	for (; i < knotsIdxs.size(); ++i) {
		++knotsIdxs[i];
	}
}

void erasePointAt(unsigned int idx) {
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

void modifyPoint(int idx) {
	curveChange = true;

	points[idx].x = mousePos.x;
	points[idx].y = mousePos.y;
}

void clearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

// testing imgui
void drawUI() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Main window
	{
		ImGui::Begin("Options");
	
		if (ImGui::RadioButton("C0 continuity", smoothness[0])) {
			// TODO: find if there is a way to transform b/n smoothness degrees
			clear();

			smoothness[0] = true;
			smoothness[1] = false;
			smoothness[2] = false;
		}
		if (ImGui::RadioButton("C1 continuity", smoothness[1])) {
			// TODO: find if there is a way to transform b/n smoothness degrees
			clear();

			smoothness[0] = false;
			smoothness[1] = true;
			smoothness[2] = false;
		}
		ImGui::Separator();

		if (points.size() > 2) {
			if (smoothness[0]) { // C^0
				if (ImGui::Button("Start New Curve")) {
					addCurve();
				}
				for (unsigned int i = 0; i < knotsIdxs.size(); ++i) {
					ImGui::Text("Knot at idx: %d\n", knotsIdxs[i]);
				}
			} else if (smoothness[1]) { // C^1
				ImGui::Text("u0: 0.f");
				for (unsigned int i = 1; i < knots.size() - 1; ++i) {
					char label[512];
					sprintf_s(label, "u%d", i);

					if (ImGui::SliderFloat(label, &knots[i], knots[i - 1] + 0.001f, knots[i + 1] - 0.001f)) {
						curveChange = true;
					}
				}
				ImGui::Text("u%d: 1.f", knots.size() - 1);
			} else if (smoothness[2]) { // C^2

			}
		}
		ImGui::Separator();

		ImGui::Checkbox("Show Control points", &showCtrlPoints);

		if (showCtrlPoints) {
			ImGui::Checkbox("Connect Control Points", &connectControlPoints);
		}

		ImGui::Checkbox("Show Keyboard Controls", &showKeyCtrls);
		ImGui::Separator();

		ImGui::SliderFloat("Zoom", &zoom, 0.1f, 10.f);
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Zoom currently not working...");
		}
		ImGui::Separator();
		
		ImGui::ColorEdit3("Background", bgColor, 1);
		ImGui::Separator();

		ImGui::ColorEdit3("Line Color", (float*)&lineColor, 1);
		ImGui::Separator();

		ImGui::ColorEdit3("Control Points Color", (float*)&cntPntColor, 1);
		ImGui::Separator();

		ImGui::ColorEdit3("Control Line Color", (float*)&cntLineColor, 1);
		ImGui::Separator();

		ImGui::ColorEdit3("Knots Color", (float*)&knotPntColor, 1);
		ImGui::Separator();

		ImGui::Text("Enter coordinates of a point to add it to the last curve");
		static float xy[2] = { WIDTH / 2.f, HEIGHT / 2.f };
		ImGui::DragFloat2("Coordinates", xy);
		if (ImGui::Button("Add Point")) {
			addPointAt(xy[0], xy[1]);
		}
		ImGui::Separator();

		if (ImGui::Button("Clear")) {
			clear();
		}
		ImGui::SameLine();
		ImGui::Text("Clear all %d points", points.size());
		ImGui::Separator();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}

	if (showKeyCtrls) {
		ImGui::Begin("Keyboard controls");

		ImGui::Text("[WASD] - Move camera Up/Left/Down/Right");
		ImGui::Text("[C] - Clear");
		ImGui::Text("[F] - add new curve");
		ImGui::Text("LCtrl + Mouse - Pan camera");
		ImGui::Text("LMouse over empty space - create new point");
		ImGui::Text("LMouse over existing point - move point around");
		ImGui::Text("LMouse over control lines - create new point in-b/n control points");
		ImGui::Text("RMouse over empty - destroy last point");
		ImGui::Text("RMouse over point - destroy hovered point");

		ImGui::End();
	}
}

void calculateCurveC0(const Vec<Vec4> &points) {
	for (unsigned int i = 0; i < knotsIdxs.size(); ++i) {
		unsigned int startIdx = knotsIdxs[i];
		unsigned int endIdx = (i == knotsIdxs.size() - 1 ? points.size() : knotsIdxs[i + 1] + 1);
		float delta = 1.f / glm::distance(points[startIdx], points[endIdx - 1]);

		for (float t = 0.f; t <= 1.f; t += delta) {
			curvePoints.push_back(casteljau(points, startIdx, endIdx, t));
		}
	}
}

void calculateCurveC1() {
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

void calculateCurve() {
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

void addCurve() {
	if ((points.size() < 2) || ((points.size() - 1) == knotsIdxs.back())) {
		return;
	}

	knotsIdxs.push_back(points.size() - 1);
}

void clear() {
	curveChange = true;
	points.clear();
	knotsIdxs.clear();
	curvePoints.clear();
	knotPoints.clear();
}
