#include <Thunder.h>
#include <sstream>
#include <chrono>
#include "imgui/imgui.h"

class ExampleLayer : public Thunder::Layer
{
private:
	float m_FPS = 0.0f;
	Thunder::OrthographicCameraController m_CameraController;
	Thunder::Ref<Thunder::Texture2D> m_CheckerboardTexture;

	Thunder::Ref<Thunder::Texture2D> m_SpriteSheet;
	Thunder::Ref<Thunder::SubTexture2D> m_Dancer;

	Thunder::TextureProperties m_CheckerboardTextureProperties = Thunder::TextureProperties::Create();

	float m_Counter;
	std::array<Thunder::Ref<Thunder::SubTexture2D>, 80> m_SubTextures;

public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
	}

	void OnAttach() override
	{
		m_CheckerboardTextureProperties.TilingFactor = 20.0f;

		m_CheckerboardTexture = Thunder::Texture2D::Create("assets/textures/checkerboard.png", m_CheckerboardTextureProperties);

		m_SpriteSheet = Thunder::Texture2D::Create("assets/textures/dance_spritesheet.png");

		int i = 0;
		for (int y = 0; y < 10; y++)
		{
			for (int x = 7; x >= 0; x--)
			{
				m_SubTextures[i] = Thunder::SubTexture2D::Create(m_SpriteSheet, { x, y }, { 110, 128 });
				i++;
			}
		}

		m_Counter = 0.5f;
	}

	~ExampleLayer()
	{
		Thunder::Instrumentor::EndSession();
	}

	void OnUpdate(Thunder::Timestep deltaTime) override
	{
		TD_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(deltaTime);

		m_FPS = 1 / deltaTime;

		Thunder::Renderer2D::ResetStats();

		{
			TD_PROFILE_SCOPE("Renderer Prep");

			Thunder::Renderer::SetClearColor({ 0.086f, 0.627f, 0.521f, 1.0f });
			Thunder::Renderer::ClearColor();

			Thunder::Renderer2D::BeginScene(m_CameraController.GetCamera());
		}

		{
			TD_PROFILE_SCOPE("Renderer Draw");

			int state = round(m_Counter);
			state %= 80;
			state = 80 - state;

			Thunder::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.16363f }, m_SubTextures[state - 1]);
			Thunder::Renderer2D::DrawQuad({ 1.2f, 0.0f, 0.0f }, { 1.0f, 1.16363f }, m_SubTextures[state - 1]);
			Thunder::Renderer2D::DrawQuad({ -1.2f, 0.0f, 0.0f }, { 1.0f, 1.16363f }, m_SubTextures[state - 1]);

			Thunder::Renderer2D::EndScene();
		}

		m_Counter += 8.0f * deltaTime;
	}

	void OnImGuiRender() override
	{
		auto stats = Thunder::Renderer2D::GetStats();

		ImGui::Begin("Stuff");

		ImGui::Text("Renderer 2D Stats:");

		ImGui::Text(" - FPS: %f", m_FPS);
		ImGui::Text(" - QuadsCount: %d", stats.QuadCount);
		ImGui::Text(" - DrawCalls: %d", stats.DrawCalls);

		ImGui::End();
	}

	void OnEvent(Thunder::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}
};

class Sandbox : public Thunder::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Thunder::Application* Thunder::CreateApplication()
{
	return new Sandbox();
}
