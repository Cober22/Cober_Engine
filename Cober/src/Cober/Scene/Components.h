#pragma once

#include <glm/glm.hpp>

#include "Cober/Renderer/Camera/SceneCamera.h"
#include "Cober/Scene/Entity.h"

namespace Cober {
	
	static enum CameraType {
		Perspective,
		Orthographic,
		FirstPerson,
		TopDown,
		RPG,
		ActionRPG
	};

	struct TransformComponent {

		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct TagComponent {

		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct SpriteRendererComponent {

		glm::vec4 Color = glm::vec4(1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};
	
	struct CameraComponent {

		SceneCamera Camera;
		CameraType cameraType = Orthographic;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraType GetCameraType() { return cameraType; }
		void SetCameraType(CameraType type) { cameraType = type; }
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

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
}
