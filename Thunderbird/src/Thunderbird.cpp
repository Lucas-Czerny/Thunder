#include "td_pch.h"

#include <Thunder.h>
#include <Thunder/Core/EntryPoint.h>

#include "EditorLayer.h"

class Thunderbird : public Thunder::Application
{
public:
	Thunderbird()
		: Application("Thunderbird")
	{
		PushLayer(new Thunder::EditorLayer());
	}

	~Thunderbird()
	{

	}
};

Thunder::Application* Thunder::CreateApplication()
{
	return new Thunderbird();
}