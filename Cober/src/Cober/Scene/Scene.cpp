#include "checkML.h"
#include "pch.h"

#include "Scene.h"
#include "Components.h"
#include "Cober/Renderer/Renderer.h"
#include "Cober/Core/Math.h"

#include <glm/glm.hpp> 

#include "Entity.h"

namespace Cober {	

	Scene::Scene()
	{
		// Insted of templates for use OnViewportResize
		//m_Registry.on_construct<CameraComponent>().connect<&Scene::OnViewportResize>();
	}

	Scene::~Scene() {
	}

	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = src.view<Component>();
		for (auto srcEntity : view)
		{
			entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

			auto& srcComponent = src.get<Component>(srcEntity);
			dst.emplace_or_replace<Component>(dstEntity, srcComponent);
		}
	}

	template<typename Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Component>())
			dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
	}

	Ref<Scene> Scene::Copy(Ref<Scene> scene)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = scene->m_ViewportWidth;
		newScene->m_ViewportHeight = scene->m_ViewportHeight;
		newScene->World3D = scene->World3D;

		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto& srcSceneRegistry = scene->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			auto uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Rigidbody3DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<BoxCollider3DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Rigidbody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name) {

		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name) {

		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();	
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {

		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart() {
		// 3D WORLD
		if (World3D) {

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
					//bc3d.Size = transform.GetScale();
					bc3d.Shape =  new btBoxShape(btVector3(bc3d.Size.x * transform.Scale.x, bc3d.Size.y * transform.Scale.y, bc3d.Size.z * transform.Scale.z));
				}

				btQuaternion rotation;
				rotation.setEulerZYX(transform.Rotation.z, transform.Rotation.y, transform.Rotation.x);
				btVector3 position = btVector3(transform.Translation.x, transform.Translation.y, transform.Translation.z);
				btDefaultMotionState* motion = new btDefaultMotionState(btTransform(rotation, position));

				btScalar bodyMass = bc3d.Density;
				btVector3 bodyIntertia;
				bc3d.Shape->calculateLocalInertia(bodyMass, bodyIntertia);

				btRigidBody::btRigidBodyConstructionInfo bodyInfo(bodyMass, motion, bc3d.Shape, bodyIntertia);
				bodyInfo.m_mass = bc3d.Density;
				bodyInfo.m_restitution = bc3d.Restitution;
				bodyInfo.m_friction = bc3d.Friction;
				btCollisionObject::CollisionFlags collisionFlag;
				switch (int(rb3d.Type)) {
					case 0: collisionFlag = btCollisionObject::CF_STATIC_OBJECT;	bodyInfo.m_mass = 0; break;
					case 1: collisionFlag = btCollisionObject::CF_KINEMATIC_OBJECT; bodyInfo.m_mass = 0; break;
					case 2: collisionFlag = btCollisionObject::CF_DYNAMIC_OBJECT; break;
				}

				rb3d.RuntimeBody = new btRigidBody(bodyInfo);
				rb3d.RuntimeBody->setCollisionFlags(collisionFlag);
				rb3d.RuntimeBody->setUserPointer(rb3d.RuntimeBody);
				//rb3d.RuntimeBody->setSleepingThresholds(DEFAULT_LINEAR_SLEEPING_THRESHOLD, DEFAULT_ANGULAR_SLEEPING_THRESHOLD);
				rb3d.RuntimeBody->setLinearFactor(btVector3(1, 1, 0));
	
				m_PhysicWorld->addRigidBody(rb3d.RuntimeBody);
			}
		}
		else {	// 2D WORLD
			m_Physics2DWorld = new b2World({ 0.0f, -9.8f });
			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view) {
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2BodyDef bodyDef;
				bodyDef.type = (b2BodyType)rb2d.Type;
				bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
				bodyDef.angle = transform.Rotation.z;

				b2Body* body = m_Physics2DWorld->CreateBody(&bodyDef);
				body->SetFixedRotation(rb2d.FixedRotation);
				rb2d.RuntimeBody = body;

				if (entity.HasComponent<BoxCollider2DComponent>()) {
					auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

					b2PolygonShape boxShape;
					boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

					b2FixtureDef fixtureDef;
					fixtureDef.shape = &boxShape;
					fixtureDef.density = bc2d.Density;
					fixtureDef.friction = bc2d.Friction;
					fixtureDef.restitution = bc2d.Restitution;
					fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
					body->CreateFixture(&fixtureDef);
				}
			}
		}
	}

	void Scene::OnRuntimeStop() {

		if (World3D) {
			delete m_PhysicWorld;
			delete m_PhysicConfig;
			delete m_PhysicSolver;
			delete m_PhysicDispatcher;
			delete m_PhysicBroadphase;
		}
		else {
			delete m_Physics2DWorld;
			m_Physics2DWorld = nullptr;
		}
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
		
		// Physics 3D
		if (World3D) {
			m_PhysicWorld->stepSimulation(ts);
			auto view = m_Registry.view<Rigidbody3DComponent>();
			// Retrieve transform from Box2D
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
			}
		}
		else {	// 2D WORLD
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_Physics2DWorld->Step(ts, velocityIterations, positionIterations);

			// Retrieve transform from Box2D
			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view) {
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.x = 0.0f;
				transform.Rotation.y = 0.0f;
				if (rb2d.FixedRotation)
					transform.Rotation.z = 0.0f;
				else
					transform.Rotation.z = body->GetAngle();
			}
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

	void Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.GetName());
		CopyComponentIfExists<TransformComponent>(newEntity, entity);
		CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);
		CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
		CopyComponentIfExists<Rigidbody3DComponent>(newEntity, entity);
		CopyComponentIfExists<BoxCollider3DComponent>(newEntity, entity);
		CopyComponentIfExists<Rigidbody2DComponent>(newEntity, entity);
		CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
	}
	
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		//static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component){
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {
	}
	
	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {
	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {
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

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {
	}
}