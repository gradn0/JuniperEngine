#include <Juniper/Core/Entrypoint.h>
#include <Juniper.h>

class GameLayer : public Juniper::Layer
{
public:
	GameLayer() :
		Layer("Game Layer")
	{
	}
};

class Sandbox : public Juniper::Application
{
public:
	Sandbox()
		: Juniper::Application(Juniper::WindowProps("Sandbox", 1280, 720, false))
	{
		PushLayer(new GameLayer());
		PushLayer(new Juniper::ImGuiLayer());
	}

	~Sandbox() {}
};

Juniper::Application* Juniper::CreateApplication()
{
	return new Sandbox();
}