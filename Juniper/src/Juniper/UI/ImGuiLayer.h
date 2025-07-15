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

		virtual void OnEvent(Event& event) override;
		virtual void OnAttach() override;
		virtual void OnUpdate(float dt) override;

		void Begin();
		void End();

	private:
		float m_Time;

		bool OnKeyPressEvent(KeyPressEvent& e);
		bool OnKeyReleaseEvent(KeyReleaseEvent& e);
		bool OnCharEvent(CharEvent& e);
		bool OnMouseMoveEvent(MouseMoveEvent& e);
		bool OnMouseButtonPressEvent(MouseButtonPressEvent& e);
		bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& e);
		bool OnMouseScrollEvent(MouseScrollEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
		bool OnWindowFocusEvent(WindowFocusEvent& e);
		bool OnWindowFocusLostEvent(WindowFocusLostEvent& e);
	};

}