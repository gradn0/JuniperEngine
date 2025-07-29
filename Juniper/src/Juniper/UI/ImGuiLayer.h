#pragma once

#include "pch.h"
#include "Juniper/Core/Layer.h"
#include "Juniper/Core/InputCodes.h"
#include "Juniper/Events/Event.h"
#include "Juniper/Events/InputEvents.h"
#include "Juniper/Events/WindowEvents.h"

namespace Juniper {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;

		void Begin();
		void End();

	private:
		float m_Time;
	};

}