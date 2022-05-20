#include "pch.h"
#include "SceneSerializer.h"
#include "Components.h"
#include "Entity.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Cober {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static std::string RigidbodyBodyTypeToString(BodyType bodyType)
	{
		switch (bodyType) {
			case BodyType::Static:		return "Static";
			case BodyType::Kinematic:	return "Kinematic";
			case BodyType::Dynamic:		return "Dynamic";
		}
	}

	static BodyType RigidbodyBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")		return BodyType::Static;
		if (bodyTypeString == "Kinematic")	return BodyType::Kinematic;
		if (bodyTypeString == "Dynamic")	return BodyType::Dynamic;

		CB_ASSERT(false, "Unknown body type");
		return BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity) 
	{
		CB_ASSERT(entity.HasComponent<IDComponent>());

		out << YAML::BeginMap;	// Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<TagComponent>()) {
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;	// TagComponent;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;	// TagComponent
		}

		if (entity.HasComponent<TransformComponent>()) {
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;	// TransformComponent;

			auto& transform = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation"	<< YAML::Value << transform.Translation;
			out << YAML::Key << "Rotation"	<< YAML::Value << transform.Rotation;
			out << YAML::Key << "Scale"		<< YAML::Value << transform.Scale;

			out << YAML::EndMap;	// TransformComponent
		}

		if (entity.HasComponent<CameraComponent>()) {
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;	// CameraComponent;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;
			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;	// Camera
			out << YAML::Key << "CameraType"	 << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV"	<< YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear"	<< YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar"	<< YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize"	<< YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear"	<< YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar"	<< YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap;	// Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap;	// CameraComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>()) {
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;	// SpriteRendererComponent;

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

			out << YAML::EndMap;	// SpriteRendererComponent
		}

		if (entity.HasComponent<AudioComponent>()) {
			out << YAML::Key << "AudioComponent";
			out << YAML::BeginMap;	// AudioComponent;

			auto& audioComponent = entity.GetComponent<AudioComponent>();
			out << YAML::Key << "NumObj" << YAML::Value << audioComponent.numObj;
			out << YAML::Key <<"Position"<< YAML::Value << audioComponent.pos;
			out << YAML::Key <<"Velocity"<< YAML::Value << audioComponent.vel;
			out << YAML::Key << "Route"  << YAML::Value << audioComponent.audioRoute;

			out << YAML::EndMap;	// AudioComponent
		}

		if (entity.HasComponent<AudioListenerComponent>()) {
			out << YAML::Key << "AudioListenerComponent";
			out << YAML::BeginMap;	// AudioListenerComponent;

			auto& audioListenerComponent = entity.GetComponent<AudioListenerComponent>();
			out << YAML::Key << "Forward"<< YAML::Value << audioListenerComponent.forward;
			out << YAML::Key << "Up"	 << YAML::Value << audioListenerComponent.up;
			out << YAML::Key <<"Position"<< YAML::Value << audioListenerComponent.pos;
			out << YAML::Key <<"Velocity"<< YAML::Value << audioListenerComponent.vel;

			out << YAML::EndMap;	// AudioListenerComponent
		}

		if (entity.HasComponent<Rigidbody3DComponent>()) {
			out << YAML::Key << "Rigidbody3DComponent";
			out << YAML::BeginMap;	// Rigidbody3DComponent;

			auto& rb3dComponent = entity.GetComponent<Rigidbody3DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << RigidbodyBodyTypeToString(rb3dComponent.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb3dComponent.FixedRotation;

			out << YAML::EndMap;	// Rigidbody3DComponent
		}

		if (entity.HasComponent<BoxCollider3DComponent>()) {
			out << YAML::Key << "BoxCollider3DComponent";
			out << YAML::BeginMap;	// BoxCollider3DComponent;

			auto& bc3dComponent = entity.GetComponent<BoxCollider3DComponent>();
			out << YAML::Key << "Offset"	<< YAML::Value	<< bc3dComponent.Offset;
			out << YAML::Key << "Size"		<< YAML::Value	<< bc3dComponent.Size;
			out << YAML::Key << "Density"	<< YAML::Value	<< bc3dComponent.Density;
			out << YAML::Key << "Friction"	<< YAML::Value	<< bc3dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value<< bc3dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc3dComponent.RestitutionThreshold;

			out << YAML::EndMap;	// BoxCollider3DComponent
		}

		if (entity.HasComponent<Rigidbody2DComponent>()) {
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap;	// Rigidbody2DComponent;

			auto& rb2dComponent = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << RigidbodyBodyTypeToString(rb2dComponent.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.FixedRotation;

			out << YAML::EndMap;	// Rigidbody2DComponent
		}

		if (entity.HasComponent<BoxCollider2DComponent>()) {
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;	// BoxCollider2DComponent;

			auto& bc2dComponent = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset"	<< YAML::Value	<< bc2dComponent.Offset;
			out << YAML::Key << "Size"		<< YAML::Value	<< bc2dComponent.Size;
			out << YAML::Key << "Density"	<< YAML::Value	<< bc2dComponent.Density;
			out << YAML::Key << "Friction"	<< YAML::Value	<< bc2dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComponent.RestitutionThreshold;

			out << YAML::EndMap;	// BoxCollider2DComponent
		}

		out << YAML::EndMap;	// Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;
				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented yet
		CB_ASSERT(false);
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		std::cout << "Deserializing scene " << sceneName << std::endl;

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				auto tagComponent = entity["TagComponent"];
				std::string name;
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				std::cout << "Deserializing entity with ID: " << uuid << std::endl;

				//Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);
				Entity deserializedEntity = m_Scene->CreateEmptyEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& transform = deserializedEntity.GetComponent<TransformComponent>();
					transform.Translation = transformComponent["Translation"].as<glm::vec3>();
					transform.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					transform.Scale = transformComponent["Scale"].as<glm::vec3>();
				}
				
				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::CameraType)cameraProps["CameraType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

				auto audioComponent = entity["AudioComponent"];
				if (audioComponent)
				{
					auto& audio = deserializedEntity.AddComponent<AudioComponent>();
					audio.numObj = audioComponent["NumObj"].as<int>();
					audio.pos	= audioComponent["Position"].as<glm::vec3>();
					audio.vel	= audioComponent["Velocity"].as<glm::vec3>();
					audio.audioRoute = audioComponent["Route"].as<std::string>();
				}

				auto audioListenerComponent = entity["AudioListenerComponent"];
				if (audioListenerComponent)
				{
					auto& audio = deserializedEntity.AddComponent<AudioListenerComponent>();
					audio.forward = audioListenerComponent["Forward"].as<glm::vec3>();
					audio.up = audioListenerComponent["Up"].as<glm::vec3>();
					audio.pos = audioListenerComponent["Position"].as<glm::vec3>();
					audio.vel = audioListenerComponent["Velocity"].as<glm::vec3>();
				}

				auto rigidbody3DComponent = entity["Rigidbody3DComponent"];
				if (rigidbody3DComponent)
				{
					auto& rb3d = deserializedEntity.AddComponent<Rigidbody3DComponent>();
					rb3d.Type = RigidbodyBodyTypeFromString(rigidbody3DComponent["BodyType"].as<std::string>());
					rb3d.FixedRotation = rigidbody3DComponent["FixedRotation"].as<bool>();
				}

				auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
				if (rigidbody2DComponent)
				{
					auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					rb2d.Type = RigidbodyBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
					rb2d.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
				}

				auto boxCollider3DComponent = entity["BoxCollider3DComponent"];
				if (boxCollider3DComponent)
				{
					auto& bc3d = deserializedEntity.AddComponent<BoxCollider3DComponent>();
					bc3d.Offset = boxCollider3DComponent["Offset"].as<glm::vec3>();
					bc3d.Size = boxCollider3DComponent["Size"].as<glm::vec3>();
					bc3d.Density = boxCollider3DComponent["Density"].as<float>();
					bc3d.Friction = boxCollider3DComponent["Friction"].as<float>();
					bc3d.Restitution = boxCollider3DComponent["Restitution"].as<float>();
					bc3d.RestitutionThreshold = boxCollider3DComponent["RestitutionThreshold"].as<float>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc2d.Density = boxCollider2DComponent["Density"].as<float>();
					bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not implemented yet
		CB_ASSERT(false);
		return false;
	}

}
