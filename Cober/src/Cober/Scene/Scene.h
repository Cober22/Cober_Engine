#pragma once

#include "Cober/Timestep.h"
#include "Cober/Renderer/Camera/EditorCamera.h"
#include "entt.hpp"

#include "Bullet/btBulletDynamicsCommon.h"
//#include "Bullet/BulletCollision/CollisionShapes/btBoxShape.h"

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

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 1280, m_ViewportHeight = 720;
	
		// Physics
		btDynamicsWorld* m_PhysicWorld = nullptr;
		btCollisionConfiguration* m_PhysicConfig = nullptr;
		btDispatcher* m_PhysicDispatcher = nullptr;
		btBroadphaseInterface* m_PhysicBroadphase = nullptr;
		btConstraintSolver* m_PhysicSolver = nullptr;

		int ITERATIONS_PER_SECOND = 60;
		float STEP_TIME;
		float DEFAULT_LINEAR_SLEEPING_THRESHOLD = 0.1f;
		float DEFAULT_ANGULAR_SLEEPING_THRESHOLD = 0.01f;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
