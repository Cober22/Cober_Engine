#include "checkML.h"
#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Cober/Renderer/Renderer.h"

#include "Cober/Math.h"

#include <glm/glm.hpp> 

namespace Cober {	

	Scene::Scene()
	{
		// Insted of templates for use OnViewportResize
		//m_Registry.on_construct<CameraComponent>().connect<&Scene::OnViewportResize>();
	}

	Scene::~Scene() {
	}

	Entity Scene::CreateEntity(const std::string& name) {

		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {

		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart() {

		m_PhysicBroadphase = new btDbvtBroadphase();
		m_PhysicConfig = new btDefaultCollisionConfiguration();
		m_PhysicDispatcher = new btCollisionDispatcher(m_PhysicConfig);
		m_PhysicSolver = new btSequentialImpulseConstraintSolver();
		m_PhysicWorld = new btDiscreteDynamicsWorld(m_PhysicDispatcher, m_PhysicBroadphase, m_PhysicSolver, m_PhysicConfig);

		STEP_TIME = 1.0f / ITERATIONS_PER_SECOND;
		m_PhysicWorld->setGravity(btVector3(0, -9.8, 0));

		auto view = m_Registry.view<Rigidbody3DComponent>();
		for (auto e : view) {
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb3d = entity.GetComponent<Rigidbody3DComponent>();

			// SHAPE
			BoxCollider3DComponent bc3d;
			if (entity.HasComponent<BoxCollider3DComponent>()) {
				bc3d = entity.GetComponent<BoxCollider3DComponent>();
				bc3d.Size = transform.GetScale();
				bc3d.Shape =  new btBoxShape(btVector3(bc3d.Size.x, bc3d.Size.y, bc3d.Size.z));
			}

			// MOTION TYPE
			btQuaternion rotation;
			rotation.setEulerZYX(transform.Rotation.z, transform.Rotation.y, transform.Rotation.x);
			btVector3 position = btVector3(transform.Translation.x, transform.Translation.y, transform.Translation.z);

			btDefaultMotionState* motion = new btDefaultMotionState(btTransform(rotation, position));

			// 4
			btScalar bodyMass = bc3d.Density;
			btVector3 bodyIntertia;
			bc3d.Shape->calculateLocalInertia(bodyMass, bodyIntertia);
			
			// 5
			btRigidBody::btRigidBodyConstructionInfo bodyInfo(bodyMass, motion, bc3d.Shape, bodyIntertia);

			// 6
			bodyInfo.m_mass = bc3d.Density;
			bodyInfo.m_restitution = bc3d.Restitution;
			bodyInfo.m_friction = bc3d.Friction;
			btCollisionObject::CollisionFlags collisionFlag;
			switch (int(rb3d.Type)) {
				case 0: collisionFlag = btCollisionObject::CF_STATIC_OBJECT;	bodyInfo.m_mass = 0; break;
				case 1: collisionFlag = btCollisionObject::CF_KINEMATIC_OBJECT; bodyInfo.m_mass = 0; break;
				case 2: collisionFlag = btCollisionObject::CF_DYNAMIC_OBJECT; break;
			}

			// 7
			rb3d.RuntimeBody = new btRigidBody(bodyInfo);
			rb3d.RuntimeBody->setCollisionFlags(collisionFlag);

			// 8
			rb3d.RuntimeBody->setUserPointer(rb3d.RuntimeBody);
			//rb3d.RuntimeBody->setSleepingThresholds(DEFAULT_LINEAR_SLEEPING_THRESHOLD, DEFAULT_ANGULAR_SLEEPING_THRESHOLD);

			// 9
			rb3d.RuntimeBody->setLinearFactor(btVector3(1, 1, 0));

	
			m_PhysicWorld->addRigidBody(rb3d.RuntimeBody);
		}
	}

	void Scene::OnRuntimeStop() {
		delete m_PhysicWorld;
		delete m_PhysicConfig;
		delete m_PhysicSolver;
		delete m_PhysicDispatcher;
		delete m_PhysicBroadphase;
	}

	void Scene::OnUpdateRuntime(Timestep ts) {

		// Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{	// TODO: Move to ScenePlay
				if (!nsc.Instance) {
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}
				nsc.Instance->OnUpdate(ts);
			});
		}

		// Physics
		{
			auto view = m_Registry.view<Rigidbody3DComponent>();
			for (auto e : view) {
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb3d = entity.GetComponent<Rigidbody3DComponent>();

				if (rb3d.RuntimeBody)
				{
					//rb3d.RuntimeBody->setLinearVelocity(btVector3(1.0f, 1.0f, 1.0f));
					//rb3d.RuntimeBody->applyCentralForce(btVector3(1.0f, 1.0f, 1.0f));
					auto pos = rb3d.RuntimeBody->getCenterOfMassPosition();
					btScalar yaw, pitch, roll;
					rb3d.RuntimeBody->getCenterOfMassTransform().getRotation().getEulerZYX(yaw, pitch, roll);
					
					transform.SetTranslation(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
					transform.SetRotation(glm::vec3(roll, pitch, yaw));
				}
			};
			m_PhysicWorld->stepSimulation(ts);
		}

		// Render sprites
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary) {
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera) 
		{
			Renderer::BeginScene(*mainCamera, cameraTransform);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group) {
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}
			// Renderer::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera) {

		// Iterate through entities with TransformComponent
		Renderer::BeginScene(camera);
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}
		// Uncomment when lights are entities
		// Renderer::EndScene;
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);
			//if (!cameraComponent.FixedAspectRatio)
			cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetPrimaryCameraEntity() {
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{entity, this};
		}
		return {};
	}
	
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {
		
	}
	
	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
	
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {
	
	}
	
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<Rigidbody3DComponent>(Entity entity, Rigidbody3DComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider3DComponent>(Entity entity, BoxCollider3DComponent& component) {

	}
}