#include "td_pch.h"
#include "Scene.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

#include "Thunder/Renderer/Renderer2D.h"

#include "Thunder/Core/Config.h"

#include "Components.h"
#include "Entity.h"

namespace Thunder
{
	Scene::Scene()
		: m_EditorCamera(EditorCamera(m_ViewportWidth, m_ViewportHeight))
	{
	}

	Scene::~Scene()
	{
	}

	template <typename T>
	void Scene::CopyComponentHelper(entt::registry& srcRegistry, entt::registry& destRegistry, std::unordered_map<GUID, entt::entity> idMap)
	{
		auto view = srcRegistry.view<T>();
		for (auto ent : view)
		{
			GUID guid = srcRegistry.get<IDComponent>(ent).ID;
			entt::entity destEntityID = idMap[guid];

			auto& component = srcRegistry.get<T>(ent);
			destRegistry.emplace_or_replace<T>(destEntityID, component);
		}
	}

	Ref<Scene> Scene::Copy()
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		std::unordered_map<GUID, entt::entity> m_GUIDToEntt;

		auto view = m_Registry.view<IDComponent>();
		for (auto ent : view)
		{
			GUID guid = m_Registry.get<IDComponent>(ent).ID;
			const std::string& name = m_Registry.get<TagComponent>(ent).Tag;

			Entity newEntity = newScene->CreateEntity(name, guid);
			m_GUIDToEntt[guid] = (entt::entity)newEntity;
		}

		FOR_EVERY_COMPONENT(CopyComponentHelper, m_Registry, newScene->m_Registry, m_GUIDToEntt);

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name /* = "Emptity" */, const GUID& guid /* = GUID::Generate() */)
	{
		Entity entity = Entity(m_Registry.create(), this);

		entity.AddComponent<IDComponent>(guid);
		entity.AddComponent<TagComponent>(name);
		entity.AddComponent<TransformComponent>();

		entity.m_Name = name;

		return entity;
	}

	template <typename T>
	void Scene::DuplicateEntityHelper(Entity& from, Entity& to)
	{
		if (!from.HasComponent<T>())
			return;

		T& component = from.GetComponent<T>();
		to.AddOrReplaceComponent<T>(component);
	}

	void Scene::DuplicateEntity(Entity& entity)
	{
		Entity newEntity = CreateEntity();

		FOR_EVERY_COMPONENT(DuplicateEntityHelper, entity, newEntity);
	}

	void Scene::OnEditorUpdate(Timestep deltaTime)
	{
		m_EditorCamera.OnUpdate(deltaTime);

		// Draw
		Renderer2D::BeginScene(m_EditorCamera);

		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int32_t)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnRuntimeUpdate(Timestep deltaTime)
	{
		// Update Scripts
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto entity : view)
			{
				auto& script = view.get<ScriptComponent>(entity);

				if (!script.Instance)
				{
					script.Instance = script.Instantiate();
					script.Instance->m_Entity = Entity(entity, this);

					script.Instance->OnCreate();
				}

				script.Instance->OnUpdate(deltaTime);
			}
		}

		// Physics

		{
			m_PhysicsWorld->Step(deltaTime, Config::VelocityIterations, Config::PositionIterations);

			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto ent : view)
			{
				Entity entity = { ent, this };

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rigidbody = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rigidbody.Box2DBody;

				const b2Vec2& position = body->GetPosition();

				transform.Translation.x = position.x;
				transform.Translation.y = position.y;

				transform.Rotation.z = body->GetAngle();
			}
		}

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform = glm::mat4(1.0f);

		// Get Primary Camera
		{
			auto group = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
				
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();

					break;
				}
			}
		}

		// Draw
		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (uint32_t)entity);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.81f });

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto ent : view)
		{
			Entity entity = { ent, this };

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rigidbody = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = (b2BodyType)rigidbody.Type;

			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(false);
			rigidbody.Box2DBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& boxCollider = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape shape;
				shape.SetAsBox(boxCollider.Scale.x * 0.5f * transform.Scale.x,
					boxCollider.Scale.y * 0.5f * transform.Scale.y);

				b2FixtureDef fixture;
				fixture.shape = &shape;
				fixture.density = boxCollider.Density;
				fixture.friction = boxCollider.Friction;
				fixture.restitution = boxCollider.Restitution;
				fixture.restitutionThreshold = boxCollider.RestitutionThreshold;

				body->CreateFixture(&fixture);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::OnEvent(Event& event)
	{
		m_EditorCamera.OnEvent(event);

		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto entity : view)
			{
				auto& script = view.get<ScriptComponent>(entity);
				script.Instance->OnEvent(event);
			}
		}
	}

	void Scene::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_EditorCamera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.KeepAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	template <typename T>
	void Scene::OnComponentAdded(const Entity& entity, T& component)
	{
		TD_WARN("Component {0} does not have an OnComponentAdded implementation!", component.GetName());
	}

	template <>
	void Scene::OnComponentAdded<TagComponent>(const Entity& entity, TagComponent& tagComponent)
	{
	}

	template <>
	void Scene::OnComponentAdded<TransformComponent>(const Entity& entity, TransformComponent& transformComponent)
	{
	}

	template <>
	void Scene::OnComponentAdded<SpriteRendererComponent>(const Entity& entity, SpriteRendererComponent& spriteRendererComponent)
	{
	}

	template <>
	void Scene::OnComponentAdded<CameraComponent>(const Entity& entity, CameraComponent& cameraComponent)
	{
		cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template <>
	void Scene::OnComponentAdded<ScriptComponent>(const Entity& entity, ScriptComponent& scriptComponent)
	{
	}
	
	template <>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(const Entity& entity, Rigidbody2DComponent& scriptComponent)
	{
	}
	
	template <>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(const Entity& entity, BoxCollider2DComponent& scriptComponent)
	{
	}
}
