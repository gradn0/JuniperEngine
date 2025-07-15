#pragma once

#include "pch.h"
#include "Core.h"
#include "InputCodes.h"

namespace Juniper {

	class Input
	{
	public:
		static bool IsKeyPressed(int key);
		static bool IsKeyReleased(int key);
		static bool IsMouseButtonPressed(int button);
		static bool IsMouseButtonReleased(int button);
		static std::pair<double, double> GetMousePosition();
	};

}

