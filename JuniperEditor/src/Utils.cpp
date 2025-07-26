#include "Utils.h"
#include <windows.h>
#include <commdlg.h>

namespace Juniper { namespace Utils {

	std::string OpenFileDialog()
	{
		char filename[MAX_PATH] = {};
		OPENFILENAMEA ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFilter = "Model files\0*.obj;*.fbx;*.gltf\0All Files\0*.*\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_FILEMUSTEXIST;
		ofn.lpstrTitle = "Open Model File";

		if (GetOpenFileNameA(&ofn))
			return std::string(filename);

		return "";
	}

}}