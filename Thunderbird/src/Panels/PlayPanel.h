#pragma once

#include "Thunder/Scene/SceneState.h"

#include "Thunder/Renderer/Texture.h"

namespace Thunder
{
	class PlayPanel
	{
	private:
		SceneState m_State;

		std::array<Ref<Texture2D>, 3> m_Textures;

	public:
		PlayPanel();

		void OnImGuiRender();

		SceneState GetState() const { return m_State; }
	};
}


