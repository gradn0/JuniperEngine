#include <Juniper/Core/Entrypoint.h>
#include "EditorLayer.h"

namespace Juniper {

	class JuniperEditor : public Application
	{
	public:
		JuniperEditor() : Application(WindowProps("Juniper Editor", 1280, 720, false))
		{
			PushLayer(new EditorLayer());
			PushLayer(new ImGuiLayer());
		}
		~JuniperEditor() {}
	};

	Application* CreateApplication()
	{
		return new JuniperEditor();
	}

}