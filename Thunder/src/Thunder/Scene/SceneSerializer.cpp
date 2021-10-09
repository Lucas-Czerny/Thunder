#include "td_pch.h"
#include "SceneSerializer.h"

#include <fstream>

#include "Entity.h"
#include "Components.h"

// Custom Encode & Decode Function for glm
namespace YAML
{
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

namespace Thunder
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
		return out;
	}

	void SceneSerializer::SerializeYAML(const fs::path& outputPath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap; // Scene

		out << YAML::Key << "Scene Name" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = Entity(entityID, m_Scene.get());
			SerializeEntityYAML(out, entity);
		});

		out << YAML::EndMap; // Scene

		std::ofstream outputFile(outputPath);
		outputFile << out.c_str();
	}

	void SceneSerializer::DeserializeYAML(const fs::path& inputPath)
	{
		std::ifstream inputFile(inputPath);

		if (inputFile.peek() == std::ifstream::traits_type::eof())
		{
			inputFile.close();
			return;
		}

		std::stringstream fileName;
		fileName << inputFile.rdbuf();

		inputFile.close();

		YAML::Node data = YAML::Load(fileName.str());

		std::string sceneName = data["Scene Name"].as<std::string>();

		YAML::Node entityNodes = data["Entities"];
		for (YAML::Node entityNode : entityNodes)
		{
			std::string name = entityNode["Tag Component"]["Tag"].as<std::string>();
			GUID id = (uint64_t)entityNode["EntityID"].as<uint64_t>();

			TD_CORE_INFO("Deserializing entity {0} with id {1}", name, (uint64_t)id);

			Entity entity = m_Scene->CreateEntity(name);

			DeserializeEntityYAML(entityNode, entity);
		}
	}

	void SceneSerializer::SerializeBinary()
	{
		TD_CORE_ASSERT(false, "Not implemented yet, use SerializeYAML instead");
	}

	void SceneSerializer::DeserializeBinary()
	{
		TD_CORE_ASSERT(false, "Not implemented yet, use DeserializeYAML instead");
	}

	void SceneSerializer::SerializeEntityYAML(YAML::Emitter& out, Entity& entity)
	{
		out << YAML::BeginMap; // Entity

		GUID id = entity.GetComponent<IDComponent>().ID;
		out << YAML::Key << "EntityID" << YAML::Value << (uint64_t)id;
		
		if (entity.HasComponent<TagComponent>())
		{
			auto& tagComponent = entity.GetComponent<TagComponent>();

			out << YAML::Key << "Tag Component" << YAML::Value << YAML::BeginMap; // Tag Component
			out << YAML::Key << "Tag" << YAML::Value << tagComponent.Tag;
			out << YAML::EndMap; // Tag Component
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();

			out << YAML::Key << "Transform Component" << YAML::Value << YAML::BeginMap; // Transform Component

			out << YAML::Key << "Translation" << YAML::Value << transformComponent.Translation;
			out << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;
			out << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;

			out << YAML::EndMap; // Transform Component
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "Sprite Renderer Component" << YAML::Value << YAML::BeginMap; // Sprite Renderer Component
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			out << YAML::EndMap; // Sprite Renderer Component
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			SceneCamera& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera Component" << YAML::Value << YAML::BeginMap; // Camera Component

			out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap; // Camera

			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFar();

			out << YAML::Key << "PerspectiveVerticalFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFar();

			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "KeepAspectRatio" << YAML::Value << cameraComponent.KeepAspectRatio;

			out << YAML::EndMap; // Camera Component
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			auto& rigidbody = entity.GetComponent<Rigidbody2DComponent>();

			out << YAML::Key << "Rigidbody2D Component" << YAML::Value << YAML::BeginMap; // Rigidbody2D Component

			out << YAML::Key << "Type" << YAML::Value << (int)rigidbody.Type;
			out << YAML::Key << "Fixed Rotation" << YAML::Value << rigidbody.FixedRotation;

			out << YAML::EndMap; // Rigidbody2D Component
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& collider = entity.GetComponent<BoxCollider2DComponent>();

			out << YAML::Key << "BoxCollider2D Component" << YAML::Value << YAML::BeginMap; // BoxCollider2D Component

			out << YAML::Key << "Offset" << YAML::Value << collider.Offset;
			out << YAML::Key << "Scale" << YAML::Value << collider.Scale;

			out << YAML::Key << "Density" << YAML::Value << collider.Density;
			out << YAML::Key << "Friction" << YAML::Value << collider.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << collider.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << collider.RestitutionThreshold;

			out << YAML::EndMap; // BoxCollider2D Component
		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::DeserializeEntityYAML(const YAML::Node& entityNode, Entity& entity)
	{
		YAML::Node transformComponentNode = entityNode["Transform Component"];
		if (transformComponentNode)
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();

			transformComponent.Translation = transformComponentNode["Translation"].as<glm::vec3>();
			transformComponent.Scale = transformComponentNode["Scale"].as<glm::vec3>();
			transformComponent.Rotation = transformComponentNode["Rotation"].as<glm::vec3>();
		}

		YAML::Node spriteRendererNode = entityNode["Sprite Renderer Component"];
		if (spriteRendererNode)
		{
			auto& spriteRendererComponent = entity.AddComponent<SpriteRendererComponent>();

			spriteRendererComponent.Color = spriteRendererNode["Color"].as<glm::vec4>();
		}

		YAML::Node cameraComponentNode = entityNode["Camera Component"];
		if (cameraComponentNode)
		{
			YAML::Node cameraNode = cameraComponentNode["Camera"];

			SceneCamera camera;

			camera.SetProjectionType((ProjectionType)cameraNode["ProjectionType"].as<int>());

			camera.SetOrthographic(
				cameraNode["OrthographicSize"].as<float>(),
				cameraNode["OrthographicNear"].as<float>(),
				cameraNode["OrthographicFar"].as<float>()
			);

			camera.SetPerspective(
				cameraNode["PerspectiveVerticalFOV"].as<float>(),
				cameraNode["PerspectiveNear"].as<float>(),
				cameraNode["PerspectiveFar"].as<float>()
			);

			camera.SetProjectionType((ProjectionType)cameraNode["ProjectionType"].as<int>());

			auto& cameraComponent = entity.AddComponent<CameraComponent>(camera);

			cameraComponent.Primary = cameraComponentNode["Primary"].as<bool>();
			cameraComponent.KeepAspectRatio = cameraComponentNode["KeepAspectRatio"].as<bool>();
		}

		YAML::Node rigidbody2DNode = entityNode["Rigidbody2D Component"];
		if (rigidbody2DNode)
		{
			auto& rigidbody2DComponent = entity.AddComponent<Rigidbody2DComponent>();

			rigidbody2DComponent.Type = (BodyType)rigidbody2DNode["Type"].as<int>();
			rigidbody2DComponent.FixedRotation = rigidbody2DNode["Fixed Rotation"].as<bool>();
		}
		
		YAML::Node boxCollider2DNode = entityNode["BoxCollider2D Component"];
		if (boxCollider2DNode)
		{
			auto& boxCollider2DComponent = entity.AddComponent<BoxCollider2DComponent>();

			boxCollider2DComponent.Offset = boxCollider2DNode["Offset"].as<glm::vec2>();
			boxCollider2DComponent.Scale = boxCollider2DNode["Scale"].as<glm::vec2>();

			boxCollider2DComponent.Density = boxCollider2DNode["Density"].as<float>();
			boxCollider2DComponent.Friction = boxCollider2DNode["Friction"].as<float>();
			boxCollider2DComponent.Restitution = boxCollider2DNode["Restitution"].as<float>();
			boxCollider2DComponent.RestitutionThreshold = boxCollider2DNode["RestitutionThreshold"].as<float>();
		}
	}
}