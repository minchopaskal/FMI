#include "include/ui_engine.h"

#include "include/curve_engine.h"

UIEngine *ui = nullptr;
UIEngine* UIInit(CurveEngine *curves, GLFWwindow *window) {
	if (!ui) {
		ui = new UIEngine{};
		ui->init(curves, window);
	}

	return ui;
}

void UIShutDown() {
	if (!ui) {
		return;
	}

	ui->shutdown();
	delete ui;
}

void UIEngine::init(CurveEngine *curves, GLFWwindow *window) {
	this->curves = curves;
	
	// Prepare ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void UIEngine::shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

UIEngine* getDefaultUIEngine() {
	return nullptr;
}

void UIEngine::drawUI() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	drawMainWindow();

	if (showKeyCtrls) {
		drawControls();
	}
}

void UIEngine::render() {
	ImGui::Render();
}

void UIEngine::renderDrawData() {
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIEngine::drawColorEdit(const String &label, const glm::vec3 &arr) {
	ImGui::ColorEdit3(label.c_str(), (float*)&arr, 1);
	ImGui::Separator();
}

bool UIEngine::drawRadioGroup(const Vec<String> &labels, bool *vals) {
	bool result = false;
	for (unsigned int i = 0; i < labels.size(); ++i) {
		if (ImGui::RadioButton(labels[i].c_str(), vals[i])) {
			for (unsigned int j = 0; j < labels.size(); ++j) {
				vals[j] = (j == i);
			}
			result = true;
		}
	}
	ImGui::Separator();

	return result;
}

void UIEngine::drawMainWindow() {
	ImGui::Begin("Options");

	if (drawRadioGroup({ "C0 curves", "C1 curves" }, curves->smoothness)) {
		curves->clear();
	}

	if (curves->points.size() > 2) {
		if (curves->smoothness[0]) { // C^0
			if (ImGui::Button("Start New Curve")) {
				curves->addCurveC0();
			}
			for (unsigned int i = 0; i < curves->knotsIdxs.size(); ++i) {
				ImGui::Text("Knot at idx: %d\n", curves->knotsIdxs[i]);
			}
		} else if (curves->smoothness[1]) { // C^1
			if (ImGui::Button("Space Knots Equally")) {
				curves->spaceKnotsEq();
			}
			ImGui::Text("u0: 0.f");
			for (unsigned int i = 1; i < curves->knots.size() - 1; ++i) {
				char label[512];
				sprintf_s(label, "u%d", i);

				if (ImGui::SliderFloat(label, &curves->knots[i], curves->knots[i - 1] + 0.001f, curves->knots[i + 1] - 0.001f)) {
					curves->curveChange = true;
				}
			}
			ImGui::Text("u%d: 1.f", curves->knots.size() - 1);
		} else if (curves->smoothness[2]) { // C^2

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

	drawColorEdit("Background color", bgColor);
	drawColorEdit("Line Color", lineColor);
	drawColorEdit("Control Points Color", cntPntColor);
	drawColorEdit("Control Lines Color", cntLineColor);
	drawColorEdit("Knots Color", knotPntColor);
	ImGui::Separator();

	ImGui::SliderFloat("Control Points Size", &cntPointSize, 1.f, 10.f);
	ImGui::SliderFloat("Control Lines Width", &cntLineWidth, 1.f, 10.f);
	ImGui::SliderFloat("Knot Points Size", &knotPointSize, 1.f, 10.f);
	ImGui::SliderFloat("Curve Width", &curveWidth, 1.f, 10.f);

	ImGui::Text("Enter coordinates of a point to add it to the last curve");
	static float xy[2] = { 0.f, 0.f };
	ImGui::DragFloat2("Coordinates", xy);
	if (ImGui::Button("Add Point")) {
		curves->addPointAt(xy[0], xy[1]);
	}
	ImGui::Separator();

	if (ImGui::Button("Clear")) {
		curves->clear();
	}
	ImGui::SameLine();
	ImGui::Text("Clear all %d points", curves->points.size());
	ImGui::Separator();

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}

void UIEngine::drawControls() {
	ImGui::Begin("Keyboard controls");
	
	ImGui::Text("[Esc] - Exit the application");
	ImGui::Separator();

	ImGui::Text("[WASD] - Move camera Up/Left/Down/Right");
	ImGui::Text("[C] - Clear");
	ImGui::Text("[F](C0 mode) - Add New curve");
	ImGui::Text("[N](C1 mode) - Equalize space between knots");
	ImGui::Text("[0-1] - Change smoothness level");
	ImGui::Text("LCtrl + Mouse - Pan camera");
	ImGui::Text("LMouse over empty space - create new point");
	ImGui::Text("LMouse over existing point - move point around");
	ImGui::Text("LMouse over control lines - create new point in-b/n control points");
	ImGui::Text("RMouse over empty - destroy last point");
	ImGui::Text("RMouse over point - destroy hovered point");

	ImGui::End();
}
