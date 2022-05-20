#pragma once

#include "Cober/Core/Timestep.h"
#include "Cober/Core/UUID.h"
#include "Cober/Renderer/Camera/EditorCamera.h"
#include "Components.h"
#include "entt.hpp"

// Bullet
#include "Bullet/btBulletDynamicsCommon.h"
// Box2D
#include "Box2D/include/box2d/b2_world.h"
#include "Box2D/include/box2d/b2_body.h"
#include "Box2D/include/box2d/b2_fixture.h"
#include "Box2D/include/box2d/b2_polygon_shape.h"

class b2World;

class btRigidBody;
class btDynamicsWorld;
class btCollisionConfiguration;
class btDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;

namespace Cober {

	class Entity;

	class Scene {

	public:
		Scene();
		~Scene();

		//template<typename Component>
		//Entity CreateEntity(const std::string& name = std::string());

		Entity CreateEmptyEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());

		void DestroyEntity(Entity entity);
		void RenderSceneEntities();

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);
		bool GetWorldType() { return World3D; }
		void SetWorldType(bool worldType) { World3D = worldType; }
		std::list<Entity> GetEntitiesOnScene() { return enttOnScene; };

		Entity GetPrimaryCameraEntity();

		void DuplicateEntity(Entity entity);
		static Ref<Scene> Copy(Ref<Scene> scene);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		std::list<Entity> enttOnScene;
		uint32_t m_ViewportWidth = 1280, m_ViewportHeight = 720;
		bool World3D = true;

		// Physics
		btDynamicsWorld* m_PhysicWorld = nullptr;
		btCollisionConfiguration* m_PhysicConfig = nullptr;
		btDispatcher* m_PhysicDispatcher = nullptr;
		btBroadphaseInterface* m_PhysicBroadphase = nullptr;
		btConstraintSolver* m_PhysicSolver = nullptr;

		b2World* m_Physics2DWorld = nullptr;

		int ITERATIONS_PER_SECOND = 60;
		float STEP_TIME;
		float DEFAULT_LINEAR_SLEEPING_THRESHOLD = 0.1f;
		float DEFAULT_ANGULAR_SLEEPING_THRESHOLD = 0.01f;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
