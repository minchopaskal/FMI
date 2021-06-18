// C std
#include <cmath>

// C++ std
#include <algorithm>
#include <chrono>
#include <memory>
#include <vector>

// User
#include "include/curve_engine.h"
#include "include/cagd_opengl.h"
#include "include/ui_engine.h"

void mainLoop();
CurveEngine *curves = new CurveEngine{};
extern CAGDOpenGL *opengl;
extern UIEngine *ui;

int main(int argc, char **argv) {
	OpenGLInit(curves);
	UIInit(curves, opengl->getGLFWwindow());

	mainLoop();

	UIShutDown();
	OpenGLShutDown();
	
	return 0;
}

void mainLoop() {
	while (opengl->running()) {
		opengl->timeIt();
		opengl->processInput();
		ui->drawUI();

		if (curves->curveChange) {
			curves->calculateCurve();
		}

		ui->render();
		opengl->render(ui);
		ui->renderDrawData();

		opengl->cleanup();
	}
}