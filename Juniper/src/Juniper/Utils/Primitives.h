#pragma once

#include "pch.h"
#include "glm/glm.hpp"
#include "Juniper/Renderer/Buffers.h"

namespace Juniper { namespace Primitives {

	std::shared_ptr<VertexArray> Quad(glm::vec4 tint);

}}
