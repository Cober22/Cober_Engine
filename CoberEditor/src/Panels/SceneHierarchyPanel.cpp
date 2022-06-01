#include "pch.h"
#include "SceneHierarchyPanel.h"
#include "Cober/Scene/Components.h"
#include "Cober/Renderer/Renderer.h"
#include "Cober/Renderer/Lighting.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

namespace Cober {

	extern const std::filesystem::path g_AssetPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
		
		SetContext(context);
	}

	SceneHierarchyPanel::~SceneHierarchyPanel() {
	
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
		
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender() {

		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID , m_Context.get() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			if (ImGui::Selectable("Empty Entity"))
				Entity newEntity = m_Context->CreateEmptyEntity("Empty Entity");
			//if (!m_Context->World3D) {
				if (ImGui::Selectable("Quad")) {
					Entity newEntity = m_Context->CreateEmptyEntity("Quad");
					newEntity.AddComponent<SpriteRendererComponent>();
				}
				//if (ImGui::Selectable("Circle")) {
				//	Entity newEntity = m_Context->CreateEmptyEntity("Circle");
				//	newEntity.AddComponent<CircleRendererComponent>();
				//}
			//}
			//else if (m_Context->World3D) {
				if (ImGui::Selectable("Cube")) {
					Entity newEntity = m_Context->CreateEmptyEntity("Cube");
					newEntity.AddComponent<CubeMeshComponent>();
				}
				if (ImGui::Selectable("Mesh")) {
					Entity newEntity = m_Context->CreateEmptyEntity("Mesh");
					newEntity.AddComponent<MeshComponent>();
				}
				//if (ImGui::Selectable("Sphere")) {
				//	Entity newEntity = m_Context->CreateEmptyEntity("Sphere");
				//	newEntity.AddComponent<SphereMeshComponent>();
				//}
			//}
			if (ImGui::Selectable("DirectionalLight")) {
				Entity newEntity = m_Context->CreateEmptyEntity("Directional Light");
				newEntity.AddComponent<DirectionalLight>();
			}
			if (ImGui::Selectable("PointLight")) {
				Entity newEntity = m_Context->CreateEmptyEntity("Point Light");
				newEntity.AddComponent<PointLight>();
			}
			if (ImGui::Selectable("SpotLight")) {
				Entity newEntity = m_Context->CreateEmptyEntity("Spot Light");
				newEntity.AddComponent<SpotLight>();
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
			DrawComponents(m_SelectionContext);

		ImGui::End();
	}

	void SceneHierarchyPanel:: SetSelectedEntity(Entity entity) {
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity) {

		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		// Delete an Entity
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();

		if (entityDeleted) {
			if (entity.HasComponent<DirectionalLight>()) {
				DirectionalLight emptyLight{ glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.0f };
				for (auto material : Renderer::primitive.materials)
					Renderer::BindDirectionalLight(material->shader, emptyLight);
				Renderer::primitive.dirLights = 0;
			}
			if (entity.HasComponent<PointLight>()) {
				int index = entity.GetComponent<PointLight>().index;
				PointLight emptyLight{ glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.0f, 0.0f, 0.0f };
				for (auto material : Renderer::primitive.materials)
					Renderer::BindPointLight(material->shader, emptyLight, index);
				Renderer::primitive.pointLights.erase(Renderer::primitive.pointLights.begin() + entity.GetComponent<PointLight>().index);
			}
			if (entity.HasComponent<SpotLight>()) {
				int index = entity.GetComponent<SpotLight>().index;
				SpotLight emptyLight{ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
				for (auto material : Renderer::primitive.materials)
					Renderer::BindSpotLight(material->shader, emptyLight, index);
				Renderer::primitive.spotLights.erase(Renderer::primitive.spotLights.begin() + entity.GetComponent<SpotLight>().index);
			}
			if (entity.HasComponent<MaterialComponent>())
				if (entity.GetComponent<MaterialComponent>().shader)
					Renderer::primitive.materials.erase(Renderer::primitive.materials.begin() + entity.GetComponent<MaterialComponent>().index);
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	void SceneHierarchyPanel::DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction) {

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>()) {
			
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open) {
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent) {

				// For uncharge shaders from the removed lights
				if (std::is_same<T, DirectionalLight>()) {
					DirectionalLight emptyLight{ glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.0f };
					for (auto material : Renderer::primitive.materials)
						Renderer::BindDirectionalLight(material->shader, emptyLight);
					Renderer::primitive.dirLights = 0;
				}
				else if (std::is_same<T, PointLight>()) {
					int index = entity.GetComponent<PointLight>().index;
					PointLight emptyLight{ glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.0f, 0.0f, 0.0f };
					for (auto material : Renderer::primitive.materials)
						Renderer::BindPointLight(material->shader, emptyLight, index);
					Renderer::primitive.pointLights.erase(Renderer::primitive.pointLights.begin() + entity.GetComponent<PointLight>().index);
				}
				else if (std::is_same<T, SpotLight>()) {
					int index = entity.GetComponent<SpotLight>().index;
					SpotLight emptyLight{ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
					for (auto material : Renderer::primitive.materials)
						Renderer::BindSpotLight(material->shader, emptyLight, index);
					Renderer::primitive.spotLights.erase(Renderer::primitive.spotLights.begin() + entity.GetComponent<SpotLight>().index);
				}
				else if (std::is_same<T, MaterialComponent>())
					if (entity.GetComponent<MaterialComponent>().shader)
						Renderer::primitive.materials.erase(Renderer::primitive.materials.begin() + entity.GetComponent<MaterialComponent>().index);
				
				entity.RemoveComponent<T>();
			}
		}
	}

	template<typename T>
	void SceneHierarchyPanel::AddIfHasComponent(std::string name) {
		if (!m_SelectionContext.HasComponent<T>()) {
			if (ImGui::MenuItem(name.c_str())) {
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
	
	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>()) {

			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				tag = std::string(buffer);
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent")) {

			AddIfHasComponent<CameraComponent>("Camera Component");

			AddIfHasComponent<SpriteRendererComponent>("Sprite Renderer Component");
			AddIfHasComponent<CircleRendererComponent>("Circle Renderer Component");

			AddIfHasComponent<CubeMeshComponent>("Cube Mesh Component");
			AddIfHasComponent<MeshComponent>("Mesh Component");
			//AddIfHasComponent<SphereMeshComponent>("Sphere Mesh Component");

			AddIfHasComponent<DirectionalLight>("DirectionalLight Component");
			AddIfHasComponent<PointLight>("PointLight Component");
			AddIfHasComponent<SpotLight>("SpotLight Component");

			AddIfHasComponent<AudioComponent>("Audio Component");
			AddIfHasComponent<AudioListenerComponent>("Audio Listener Component");
			AddIfHasComponent<NativeScriptComponent>("Native Script Component");
			AddIfHasComponent<MaterialComponent>("Material Component");

			if (m_Context->GetWorldType()) {
				AddIfHasComponent<Rigidbody3DComponent>("Rigidbody 3D Component");
				AddIfHasComponent<BoxCollider3DComponent>("BoxCollider 3D Component");
			}
			else {
				AddIfHasComponent<Rigidbody2DComponent>("Rigidbody 2D Component");
				AddIfHasComponent<BoxCollider2DComponent>("BoxCollider 2D Component");
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});
		
		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = {
				"Perspective", "Orthographic",
				"FirstPerson",
				"TopDown",
				"RPG",
				"ActionRPG"
			};
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {

				for (int i = 0; i < 6; i++) {
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::CameraType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::CameraType::Perspective) {

				float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &verticalFov))
					camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

				float orthoNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetPerspectiveNearClip(orthoNear);

				float orthoFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetPerspectiveFarClip(orthoFar);
			}

			if (camera.GetProjectionType() == SceneCamera::CameraType::Orthographic) {

				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);

				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

			ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
					component.Texture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
		});
		
		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Thickess", &component.Thickness, 0.025f, 0.0f, 1.0f);
			ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
		});

		DrawComponent<CubeMeshComponent>("Cube Mesh Renderer", entity, [](auto& component)
		{
		});

		DrawComponent<DirectionalLight>("Directional Light", entity, [](auto& component)
		{
			ImGui::Checkbox("Draw Source", &component.Source);
			DrawVec3Control("Direction", component.Direction);
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));;
			ImGui::DragFloat("Ambient", &component.AmbientIntensity, 1.0f, 0.0f, 1.0f);
			ImGui::DragFloat("Diffuse", &component.DiffuseIntensity, 1.0f, 0.0f, 1.0f);
		});

		DrawComponent<PointLight>("Point Light", entity, [](auto& component)
		{
			ImGui::Checkbox("Draw Source", &component.Source);
			DrawVec3Control("Position", component.Position, 0.0f);
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Ambient", &component.AmbientIntensity, 1.0f, 0.0f, 1.0f);
			ImGui::DragFloat("Diffuse", &component.DiffuseIntensity, 1.0f, 0.0f, 1.0f);

			ImGui::DragFloat("Constant Att.", &component.Attenuation.Constant, 1.0f, 0.0f, 1.0f);
			ImGui::DragFloat("Linear Att.", &component.Attenuation.Linear, 0.0f, 0.0f, 100.0f);
			ImGui::DragFloat("Exponential Att.", &component.Attenuation.Exp, 0.0f, 0.0f, 100.0f);
		});

		DrawComponent<SpotLight>("Spot Light", entity, [](auto& component)
		{
			ImGui::Checkbox("Draw Source", &component.Source);
			DrawVec3Control("Direction", component.Direction, 0.0f);
			DrawVec3Control("Position", component.Position, 0.0f);
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Ambient", &component.AmbientIntensity, 1.0f, 0.0f, 1.0f);
			ImGui::DragFloat("Diffuse", &component.DiffuseIntensity, 1.0f, 0.0f, 1.0f);
			ImGui::DragFloat("CutOff", &component.CutOff, 1.0f, 0.0f, 100.0f);
			ImGui::DragFloat("OuterCutOff", &component.OuterCutOff, 1.0f, 0.0f, 100.0f);

			ImGui::DragFloat("Constant Att.", &component.Attenuation.Constant, 1.0f, 0.0f, 1.0f);
			ImGui::DragFloat("Linear Att.", &component.Attenuation.Linear, 0.0f, 0.0f, 100.0f);
			ImGui::DragFloat("Exponential Att.", &component.Attenuation.Exp, 0.0f, 0.0f, 100.0f);
		});

		DrawComponent<MeshComponent>("Mesh Renderer", entity, [](auto& component)
		{
			ImGui::Button("Mesh", ImVec2(100.0f, 0.0f));
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path meshPath = std::filesystem::path(g_AssetPath) / path;
					component.mesh = CreateRef<Mesh>();
					component.mesh->LoadMesh(meshPath.string());
					component.meshRoute = meshPath.string();
				}
				ImGui::EndDragDropTarget();
			}
		});

		//DrawComponent<SphereMeshComponent>("Sphere Mesh Renderer", entity, [](auto& component)
		//{
		//	// TODO
		//});

		DrawComponent<AudioComponent>("Audio", entity, [](auto& component)
		{
			ImGui::DragInt("Num Object", &component.numObj, -1);
			DrawVec3Control("Position", component.pos, 0.0f);
			DrawVec3Control("Velocity", component.vel, 0.0f);
			ImGui::Button("Audio", ImVec2(100.0f, 0.0f));
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path audioPath = std::filesystem::path(g_AssetPath) / path;
					component.audioRoute = audioPath.string();
				}
				ImGui::EndDragDropTarget();
			}
		});

		DrawComponent<AudioListenerComponent>("Audio Listener", entity, [](auto& component)
		{
			//Entity entity{ entity , m_Context };
			DrawVec3Control("Forward", component.forward, 0.0f);
			DrawVec3Control("Up", component.up, 0.0f);
			DrawVec3Control("Pos", component.pos, 0.0f);
			DrawVec3Control("Vel", component.vel, 0.0f);
			//DrawVec3Control("Pos", component.pos, entity.GetComponent<TransformComponent>().GetTranslation());
		});

		DrawComponent<NativeScriptComponent>("Native Script", entity, [](auto& component)
			{
				ImGui::Button("Script", ImVec2(100.0f, 0.0f));
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path scriptPath = std::filesystem::path(g_AssetPath) / path;
						//std::ofstream filePath;
						//filePath.open(path);
						//template<typename T>
						//component.Bind<T>();
					}
					ImGui::EndDragDropTarget();
				}
			});

		DrawComponent<Rigidbody3DComponent>("Rigidbody 3D", entity, [](auto& component)
		{
			const char* bodyTypeStrings[] = { "Static", "Kinematic", "Dynamic" };
			const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
			if (ImGui::BeginCombo("Body Type", currentBodyTypeString)) {

				for (int i = 0; i < 3; i++) {
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
						currentBodyTypeString = bodyTypeStrings[i];
						component.Type = (BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
		{
			const char* bodyTypeStrings[] = { "Static", "Kinematic", "Dynamic" };
			const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
			if (ImGui::BeginCombo("Body Type", currentBodyTypeString)) {

				for (int i = 0; i < 3; i++) {
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
						currentBodyTypeString = bodyTypeStrings[i];
						component.Type = (BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});

		DrawComponent<BoxCollider3DComponent>("Box Collider 3D", entity, [](auto& component)
		{
			ImGui::DragFloat3("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat3("Size", glm::value_ptr(component.Size));
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});

		DrawComponent<MaterialComponent>("Material", entity, [](auto& component)
		{
			Ref<MaterialComponent> material = CreateRef<MaterialComponent>();
			ImGui::Button("Shader", ImVec2(100.0f, 0.0f));
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path shaderPath = std::filesystem::path(g_AssetPath) / path;
					component.shader = CreateRef<Shader>(shaderPath.string());
					component.shaderRoute = shaderPath.string();

					material->shader = component.shader;
					Renderer::primitive.materials.push_back(material);
				}
				ImGui::EndDragDropTarget();
			}

			// Read uniform variables
		});
	}
}