#include "pch.h"
#include "Layer.h"

namespace Juniper {

    Layer::Layer(const std::string& name) : m_Name(name) {}
    Layer::~Layer() {}

    const std::string& Layer::GetName() const
    {
        return m_Name;
    }

    void LayerStack::Push(Layer* layer)
    {
        auto it = m_Layers.insert(m_Layers.begin() + m_Top++, std::unique_ptr<Layer>(layer));
    }

    void LayerStack::Pop()
    {
        if (m_Top == 0) return;

        m_Top--;
        auto it = m_Layers.begin() + m_Top;
        it->get()->OnDetach();
        m_Layers.erase(it);
    }

}