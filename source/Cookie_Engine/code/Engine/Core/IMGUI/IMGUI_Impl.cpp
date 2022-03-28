#include "IMGUI_Impl.h"
#include "Core/Application.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Cookie {
namespace IMGUI_Impl {

	void IMGUI_Impl::Init() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(Application::appData.m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void IMGUI_Impl::NewFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void IMGUI_Impl::Render() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void IMGUI_Impl::Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

} // namespace IMGUI_Impl
} // namespace Cookie