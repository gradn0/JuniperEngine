#pragma once

#include <Juniper.h>

namespace Juniper {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		bool OnKeyPress(KeyPressEvent& event);
		bool OnMouseMove(MouseMoveEvent& event);

		virtual void OnEvent(Event& event) override;
		virtual void OnAttach() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnImGuiRender() override;

	private:
		Application& m_App;
		
		OrthographicCamera m_Camera;
		float m_CameraSpeed = 5.0f;
		float m_MouseSensitivity = 0.1f;
		float m_HasCameraUpdated = false;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_Quad;
		std::shared_ptr<Texture2D> m_TextureLeaf;
		std::shared_ptr<Texture2D> m_TextureStone;
		std::shared_ptr<Texture2D> m_SpritesheetFloor;
		std::shared_ptr<SubTexture2D> m_Grass;

		void updateCamera(float dt); 
	};

}
