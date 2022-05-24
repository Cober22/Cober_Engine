#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{
}

void Sandbox2D::OnAttach()
{
	m_ActiveScene = CreateRef<Scene>();

	SceneSerializer serializer(m_ActiveScene);
	serializer.Deserialize("Assets/Scenes/ExampleScene.cober");
}

void Sandbox2D::OnDetach()
{
	//m_Framebuffer->Bind();
}

void Sandbox2D::OnUpdate( Timestep ts)
{	
	CB_PROFILE_FUNCTION();

	// Update
	{
		CB_PROFILE_SCOPE("CameraController::OnUpdate");
		
		m_ActiveScene->OnViewportResize((uint32_t)1280, (uint32_t)720);
	}

	// Render
	{
		CB_PROFILE_SCOPE("Render Prep");
		// BACKGRUOND COLOR!
		//RenderCommand::SetClearColor({ 0.02f, 0.008f, 0.05f, 1.0f });	// DARK BLUE
		//RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		 RenderCommand::SetClearColor({ 1.0f, 0.6f, 0.3f, 1.0f });	// ORANGE
		//RenderCommand::SetClearColor({ 0.8f, 0.35f, 0.35f, 1.0f });
		 RenderCommand::Clear();
	}
	
	{
		CB_PROFILE_SCOPE("Render Draw");

		m_ActiveScene->OnUpdateRuntime(ts);
	}
}

void Sandbox2D::OnEvent(Event& event)
{

}