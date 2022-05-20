#pragma once

#include "Cober/Core/UUID.h"
#include "Cober/Sound/AudioManager.h"
#include "Cober/Renderer/Texture.h"
#include "Cober/Renderer/Primitives/Mesh.h"
#include "Cober/Renderer/Primitives/Primitives.h"
#include "Cober/Renderer/Lighting.h"
#include "Cober/Renderer/Shader.h"
#include "SceneCamera.h"

#include <glm/glm.hpp>
#include <glm//gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// Forward declaration
class btRigidBody;
class btCollisionShape;

namespace Cober {


	// [-----------------------------]
	// [----------- CORE ------------]
	struct IDComponent {
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};
	
	struct TransformComponent {

		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			//glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
			//	* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
			//	* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });
			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
		glm::vec3 GetTranslation() const { return Translation; }
		void SetTranslation(glm::vec3 translation)	{ Translation = translation; }

		glm::vec3 GetRotation() const { return Rotation; }
		void SetRotation(glm::vec3 rotation)  { Rotation = rotation; }

		glm::vec3 GetScale() const { return Scale; }
		void SetScale(glm::vec3 scale)	{ Scale = scale; }
	};

	struct TagComponent {

		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	// [-----------------------------]
	// [---------- CAMERA -----------]
	struct CameraComponent {

		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// [-----------------------------]
	// [---------- TEXTURES ---------]
	struct SpriteRendererComponent {

		glm::vec4 Color = glm::vec4(1.0f);
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};
	
	struct CircleRendererComponent {

		glm::vec4 Color = glm::vec4(1.0f);
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	// [-----------------------------]
	// [---------- MESHES -----------]
	struct CubeMeshComponent {

		Ref<Cube> cube;

		CubeMeshComponent() = default;
		CubeMeshComponent(const CubeMeshComponent&) = default;
	};

	enum class LightType { Directional = 0, Point, Spot };
	struct LightComponent {

		LightType lightType;

		LightComponent() = default;
		LightComponent(const LightComponent&) = default;
	};

	struct MeshComponent {

		Ref<Mesh> mesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
	};

	struct SphereMeshComponent {

		SphereMeshComponent() = default;
		SphereMeshComponent(const SphereMeshComponent&) = default;
	};

	// [-----------------------------]
	// [--------- MATEIRAL ----------]
	struct MaterialComponent {

		Ref<Shader> shader;
		std::string shaderRoute;

		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent&) = default;
	};

	// [-----------------------------]
	// [---------- SCRIPTS ----------]
	class ScriptableEntity;
	struct NativeScriptComponent {

		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind() { 
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	// [-----------------------------]
	// [----------- AUDIO -----------]
	struct AudioComponent {

		bool ReceiveEvent(Event& event) { return false; }
		
		void PlayMusic(std::string path, int channel) {
			AudioManager::GetInstance()->PlayMusic(path.c_str(), channel, true);
		}
		void PlayMusic(std::string path) {
			AudioManager::GetInstance()->PlayMusic(path.c_str(), numObj, true);
		}

		void RemoveEmisor() {
			AudioManager::GetInstance()->RemoveEmisor(numObj);
			numObj = -1;
		}

		int numObj;
		glm::vec3 pos;
		glm::vec3 vel;
		std::string audioRoute;

		AudioComponent() = default;
		AudioComponent(const AudioComponent&) = default;
	};

	struct AudioListenerComponent {

		bool ReceiveEvent(Event& event) { return false; }

		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 pos;
		glm::vec3 vel;

		AudioListenerComponent() = default;
		AudioListenerComponent(const AudioListenerComponent&) = default;
	};
	
	// [-----------------------------]
	// [---------- PHYSICS ----------]
	enum class BodyType { Static = 0, Kinematic, Dynamic };
	
	struct Rigidbody3DComponent {
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		btRigidBody* RuntimeBody;

		Rigidbody3DComponent() = default;
		Rigidbody3DComponent(const Rigidbody3DComponent&) = default;
	};

	struct Rigidbody2DComponent {
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider3DComponent {
		glm::vec3 Offset = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Size = { 1.0f, 1.0f, 1.0f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;
		btCollisionShape* Shape = nullptr;

		BoxCollider3DComponent() = default;
		BoxCollider3DComponent(const BoxCollider3DComponent&) = default;
	};

	struct BoxCollider2DComponent {
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 1.0f, 1.0f };

		// Make a physics material maybe
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};


	// [-----------------------------]
	// [------------ AI -------------]





	// [-----------------------------]
	// [---------- TERRAIN ----------]
}
