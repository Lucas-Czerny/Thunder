#include "td_pch.h"
#include "PropertiesPanel.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <imgui.h>

#include "Thunder/ImGui/ImGuiConfig.h"

#include "Thunder/Renderer/Texture.h"

#include "Thunder/Core/MouseButtonCodes.h"
#include "Thunder/ImGui/CustomImGuiWidgets.h"

#include "FileBrowserPanel.h"

#include "Thunder/Scene/Components.h"

namespace Thunder
{
	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::Begin("Properties");

		ImGui::Spacing();

		m_SelectedEntity = m_Context->GetSelectedEntity();

		if (!m_SelectedEntity)
		{
			ImGui::Text("No entity selected");
			ImGui::End();

			return;
		}

		{
			auto& tagComponent = m_SelectedEntity.GetComponent<TagComponent>();

			static char tagBuffer[256];

			memset(tagBuffer, '\0', sizeof(tagBuffer));
			strcpy_s(tagBuffer, sizeof(tagBuffer), tagComponent.Tag.c_str());

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

			ImGui::PushItemWidth(-Config::AddComponentButtonWidth);

			if (ImGui::InputText("##entityTag", tagBuffer, sizeof(tagBuffer)))
			{
				tagComponent.Tag = std::string(tagBuffer);
			}

			ImGui::PopItemWidth();
		}

		{
			ImGui::SameLine();

			bool popupOpen = ImGui::Button("Add Component");

			ImCustom::DrawPopup("Add Component", popupOpen, [&]()
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, Config::AddComponentPopupPaddingY });

				bool selected = true;

				ImGui::PushItemWidth(300.0f);

				#define DRAW_MENU_ITEM(class) \
					if (!m_SelectedEntity.HasComponent<class>() && ImGui::MenuItem(class::GetName())) \
					{ \
						m_SelectedEntity.AddComponent<class>(); \
					} \

				DRAW_MENU_ITEM(SpriteRendererComponent)
				DRAW_MENU_ITEM(CameraComponent)
				DRAW_MENU_ITEM(ScriptComponent)
				DRAW_MENU_ITEM(Rigidbody2DComponent)
				DRAW_MENU_ITEM(BoxCollider2DComponent)

				else
				{
					selected = false;
				}

				ImGui::PopStyleVar();
				return selected;
			});

		}

		DrawComponent<TransformComponent>([&](auto& transformComponent)
		{
			ImCustom::DrawVec3Control("Translation", transformComponent.Translation);
			ImGui::Spacing();

			ImCustom::DrawVec3Control("Scale", transformComponent.Scale, { 1.0f, 1.0f, 1.0f });
			ImGui::Spacing();

			glm::vec3 rotationInDegrees = glm::degrees(transformComponent.Rotation);

			ImCustom::DrawVec3Control("Rotation", rotationInDegrees);

			transformComponent.Rotation = glm::radians(rotationInDegrees);
		});

		DrawComponent<SpriteRendererComponent>([&](auto& spriteRendererComponent)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererComponent.Color));

			ImGui::Text("Drag Texture");

			ImGui::SameLine();

			ImGui::ImageButton((ImTextureID)spriteRendererComponent.Texture->GetRendererID(), ImVec2(21, 21), ImVec2(0, 0), ImVec2(1, 1), 0);

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE");

				if (payload != nullptr)
				{
					File* file = *(File**)payload->Data;
					spriteRendererComponent.Texture = Texture2D::Create(file->Path.string());
				}

				ImGui::EndDragDropTarget();
			}
		});

		DrawComponent<CameraComponent>([&](auto& cameraComponent)
		{
			ImGui::Checkbox("Primary", &cameraComponent.Primary);

			static const char* projectionTypeStrings[2] = { "Orthographic", "Perspective" };

			ProjectionType projectionType = cameraComponent.Camera.GetProjectionType();
			const char* currentProjectionTypeString = projectionTypeStrings[(int)projectionType];

			if (ImGui::BeginCombo("Projection Type", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						projectionType = ProjectionType(i);
						cameraComponent.Camera.SetProjectionType(projectionType);
					}
				}

				ImGui::EndCombo();
			}

			switch (projectionType)
			{
				case ProjectionType::Orthographic:
				{
					float size = cameraComponent.Camera.GetOrthographicSize();
					if (ImCustom::DragFloat("Size", &size, Config::OrthographicSizeSliderArgs))
						cameraComponent.Camera.SetOrthographicSize(size);

					float _near = cameraComponent.Camera.GetOrthographicNear();
					if (ImCustom::DragFloat("Near", &_near, Config::OrthographicNearSliderArgs))
						cameraComponent.Camera.SetOrthographicNear(_near);

					float _far = cameraComponent.Camera.GetOrthographicFar();
					if (ImCustom::DragFloat("Far", &_far, Config::OrthographicFarSliderArgs))
						cameraComponent.Camera.SetOrthographicFar(_far);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.KeepAspectRatio);

					break;
				}
				case ProjectionType::Perspective:
				{
					float fov = cameraComponent.Camera.GetPerspectiveVerticalFOV();
					fov = glm::degrees(fov);
					if (ImCustom::DragFloat("Vertical FOV", &fov, Config::PerspectiveFOVSliderArgs))
						cameraComponent.Camera.SetPerspectiveVerticalFOV(glm::radians(fov));

					float _near = cameraComponent.Camera.GetPerspectiveNear();
					if (ImCustom::DragFloat("Near", &_near, Config::PerspectiveNearSliderArgs))
						cameraComponent.Camera.SetPerspectiveNear(_near);

					float _far = cameraComponent.Camera.GetPerspectiveFar();
					if (ImCustom::DragFloat("Far", &_far, Config::PerspectiveFarSliderArgs))
						cameraComponent.Camera.SetPerspectiveFar(_far);

					break;
				}
			}
		});

		// TODO:
		DrawComponent<ScriptComponent>([&](auto& scriptComponent)
		{
		});

		DrawComponent<Rigidbody2DComponent>([&](auto& rigidbodyComponent)
		{
			static const char* bodyTypeStrings[3] = { "Static", "Kinematic", "Dynamic" };

			BodyType type = rigidbodyComponent.Type;
			const char* currentProjectionTypeString = bodyTypeStrings[(int)type];

			if (ImGui::BeginCombo("Body Type", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
					{
						rigidbodyComponent.Type = BodyType(i);
					}
				}

				ImGui::EndCombo();
			}
		});

		DrawComponent<BoxCollider2DComponent>([&](auto& colliderComponent)
		{
			float density = colliderComponent.Density;
			if (ImCustom::DragFloat("Density", &density, Config::PhysicsSliderArgs))
				colliderComponent.Density = density;

			float friction = colliderComponent.Friction;
			if (ImCustom::DragFloat("Friction", &friction, Config::PhysicsSliderArgs))
				colliderComponent.Friction = friction;

			float restitution = colliderComponent.Restitution;
			if (ImCustom::DragFloat("Restitution", &restitution, Config::PhysicsSliderArgs))
				colliderComponent.Restitution = restitution;

			float restitutionThreshold = colliderComponent.RestitutionThreshold;
			if (ImCustom::DragFloat("Restitution Threshold", &restitutionThreshold, Config::PhysicsSliderArgs))
				colliderComponent.RestitutionThreshold = restitutionThreshold;
		});

		ImGui::Spacing();

		ImGui::End();
	}

	template <typename T>
	void PropertiesPanel::DrawComponent(std::function<void(T&)> componentUICode, bool removeable /* = true */)
	{
		if (m_SelectedEntity.HasComponent<T>())
		{
			auto& component = m_SelectedEntity.GetComponent<T>();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

			bool open = ImGui::TreeNodeEx((void*)(uint64_t)(int32_t)m_SelectedEntity, flags, component.GetName());

			if (ImGui::BeginPopupContextItem(component.GetName()))
			{
				ImGui::Spacing();

				if (ImGui::MenuItem("Remove Component"))
				{
					m_SelectedEntity.RemoveComponent<T>();
				}

				ImGui::Spacing();

				ImGui::EndPopup();
			}

			if (open)	
			{
				componentUICode(component);
				ImGui::TreePop();
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}
	}
}
