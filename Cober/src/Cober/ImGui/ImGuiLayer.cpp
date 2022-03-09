#include "pch.h"
#include "ImGuiLayer.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"

namespace Cober {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnAttach() 
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
<<<<<<< Updated upstream
	}
=======
		io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport;
>>>>>>> Stashed changes

	void ImGuiLayer::OnDetach() 
	{

<<<<<<< Updated upstream
=======
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}


		Application& app = Application::Get();
		SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());
>>>>>>> Stashed changes
	}

	void ImGuiLayer::OnUpdate() 
	{

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(app.GetWindowWidth(), app.GetWindowHeight());

<<<<<<< Updated upstream
		float time = (float)SDL_GetTicks();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;
=======
	void ImGuiLayer::OnEvent(SDL_Event& event) {

		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	void ImGuiLayer::Begin() {
>>>>>>> Stashed changes

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(app.GetWindow());
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

<<<<<<< Updated upstream
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
=======
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
	
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}
>>>>>>> Stashed changes
	}

	void ImGuiLayer::OnEvent(SDL_Event* event)
	{
		// Process ImGui Events
		ImGui_ImplSDL2_ProcessEvent(event);
	}
}
