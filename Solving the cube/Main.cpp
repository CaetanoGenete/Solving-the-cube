
#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/glew.h>    
#include <GLFW/glfw3.h>

#include "Window\Window.h"
#include "Graphics\Renderer.h"

#include "CubeView.h"
#include "glm\gtc\matrix_transform.hpp"

#include "Events\Event.h"
#include "Events\WindowEvents.h"

#include "CoordinateLevel.h"
#include "Scrambler.h"

#include <time.h>

bool running = true;

Virtual3DCube* cubeView;

bool onWindowClosed(WindowClosedEvent& event) {
	running = false;

	return true;
}

void onEvent(Event& event) {
	cubeView->onEvent(event);

	EventDispatcher dispatcher(event);

	dispatcher.dispatch<WindowClosedEvent>(std::bind(&onWindowClosed, std::placeholders::_1));
}

int main() {
		
	WindowProperties mainWindowProps("main"); // creates default properties (width = 1280, height = 720)
	mainWindowProps.v_sync_ = true;

	Window mainWindow(mainWindowProps); //creates the main window
	mainWindow.setEventCallBack(std::bind(&onEvent, std::placeholders::_1));
	mainWindow.initialise();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Renderer renderer;
	loadSymmetries();
	loadTransitionTables();

	StaticVirtual3DCubeInitialise();
	cubeView = new Virtual3DCube(mainWindow);

	cubeView->camera().setProjection(glm::perspective(glm::radians(90.0f), (float)mainWindow.width() / mainWindow.height(), 0.1f, 1000.0f));
	//cubeView->camera().setProjection(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, 0.01f, 1000.0f));
	cubeView->camera().moveTo(0.0f, 0.0f, 12.0f);
	cubeView->camera().lookAt(0, 0, 0);

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(mainWindow.window(), true);
    ImGui::StyleColorsDark();

	utils::String scramble;

    // Main loop
	while (running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the screen

		cubeView->render(renderer); // draws cube onto screen.

		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("VirtualCubeInterface");
		ImGui::SetWindowSize(ImVec2(395, 375));

		if (ImGui::Button("SOLVE", ImVec2(380, 33)))
			cubeView->solve();

		if (ImGui::Button("HINT", ImVec2(380, 33)))
			cubeView->hint();

		if (ImGui::Button("SCRAMBLE", ImVec2(380, 33)))
			scramble = cubeView->scramble();
		
		ImGui::Text(scramble.getData());

		ImGui::Separator();

		ImGui::Columns(3, 0, false);
		{
			if (ImGui::Button("U", ImVec2(125, 100)))
				cubeView->turn(Ux1);


			if (ImGui::Button("R", ImVec2(125, 100)))
				cubeView->turn(Rx1);

			ImGui::NextColumn();

			if (ImGui::Button("F", ImVec2(125, 100)))
				cubeView->turn(Fx1);

			if (ImGui::Button("D", ImVec2(125, 100)))
				cubeView->turn(Dx1);

			ImGui::NextColumn();

			if (ImGui::Button("L", ImVec2(125, 100)))
				cubeView->turn(Lx1);

			if (ImGui::Button("B", ImVec2(125, 100)))
				cubeView->turn(Bx1);
		}

		ImGui::End();

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		mainWindow.onUpdate();
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

	delete cubeView;

	delete[] corner_ori_flip_ud_slice_prune_table;
	delete[] ud_edge_perm_corner_perm_prune_table;

    glfwTerminate();

    return 0;
}


