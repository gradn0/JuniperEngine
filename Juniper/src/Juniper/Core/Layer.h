#pragma once

#include "pch.h"
#include "Juniper/Events/Event.h"
#include "Core.h"

namespace Juniper {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = 0;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnEvent(Event& e) {}
		virtual void OnImGuiRender() {}

		const std::string& GetName() const;

	private:
		std::string m_Name;
	};

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack() = default;

		LayerStack(const LayerStack&) = delete;
		LayerStack& operator=(const LayerStack&) = delete;

		LayerStack(LayerStack&&) = default;
		LayerStack& operator=(LayerStack&&) = default;

		void Push(Layer* layer);
		void Pop();

		std::vector<std::unique_ptr<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<std::unique_ptr<Layer>>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<std::unique_ptr<Layer>> m_Layers;
		uint32_t m_Top = 0;
	};

}