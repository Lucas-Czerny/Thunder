#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Thunder/Core/GUID.h"

#include "Thunder/Renderer/Texture.h"

#include "Thunder/Scene/SceneCamera.h"
#include "Thunder/Scene/ScriptableComponent.h"

#define FOR_EVERY_COMPONENT(func, ...) \
	func<IDComponent>(__VA_ARGS__); func<TagComponent>(__VA_ARGS__); func<TransformComponent>(__VA_ARGS__); \
	func<SpriteRendererComponent>(__VA_ARGS__); func<CameraComponent>(__VA_ARGS__); func<ScriptComponent>(__VA_ARGS__);	\
	func<Rigidbody2DComponent>(__VA_ARGS__); func<BoxCollider2DComponent>(__VA_ARGS__);

namespace Thunder
{
	struct IDComponent
	{
		GUID ID;

		IDComponent() = default;
		IDComponent(const GUID& id)
			: ID(id) {}

		static const char* GetName() { return "ID Component"; }
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		operator std::string& () { return Tag; }

		static const char* GetName() { return "Tag Component"; }
	};

	struct TransformComponent
	{
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };

		TransformComponent() = default;
		TransformComponent(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation)
			: Translation(translation), Scale(scale), Rotation(rotation) {}

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::toMat4(glm::quat(Rotation))
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		static const char* GetName() { return "Transform Component"; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color;
		Ref<Texture2D> Texture;
		
		SpriteRendererComponent()
			: Color(1.0f, 1.0f, 1.0f, 1.0f), Texture(Texture2D::WhiteTexture) {}

		SpriteRendererComponent(const glm::vec4& color)
			: Color(color), Texture(Texture2D::WhiteTexture) {}

		SpriteRendererComponent(const Ref<Texture2D>& texture)
			: Color(1.0f, 1.0f, 1.0f, 1.0f), Texture(texture) {}

		static const char* GetName() { return "Sprite Renderer Component"; }
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool KeepAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const SceneCamera& camera)
			: Camera(camera) {}

		static const char* GetName() { return "Camera Component"; }
	};

	typedef ScriptableComponent* (*InstantiateFunction)();
	typedef void (*DeleteFunction)(ScriptableComponent*);

	struct ScriptComponent
	{
		ScriptableComponent* Instance = nullptr;

		InstantiateFunction Instantiate;
		DeleteFunction Delete;

		template <typename T>
		void Bind()
		{
			Instantiate = []() { return (ScriptableComponent*)(new T()); };
			Delete = [](ScriptableComponent* script) { delete script; };
		}

		ScriptComponent() = default;
		ScriptComponent(ScriptableComponent* instance)
			: Instance(instance) {}

		static const char* GetName() { return "Script Component"; }
	};

	// Physics

	enum class BodyType
	{
		Static = 0, Kinetic, Dynamic
	};

	struct Rigidbody2DComponent
	{
		friend class Scene;

	protected:
		void* Box2DBody = nullptr;

	public:
		BodyType Type = BodyType::Dynamic;

		bool FixedRotation = false;

		Rigidbody2DComponent() = default;

		static const char* GetName() { return "Rigidbody2D Component"; }
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Scale = { 1.0f, 1.0f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.1f;
		float RestitutionThreshold = 0.5f;

		BoxCollider2DComponent() = default;

		static const char* GetName() { return "BoxCollider2D Component"; }
	};
}