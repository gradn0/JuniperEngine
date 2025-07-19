#pragma once

#include "pch.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Juniper/Core/Core.h"

namespace Juniper {

	class Shader
	{
	public:
		Shader(const std::string& vsPath, const std::string& fsPath);
		~Shader();

		inline uint32_t GetId() const { return m_Id; }

		void Bind() const;
		void BindUniformBlock(const std::string& name, uint32_t slot = 0) const;

		void setUniform1i(const std::string& name, int value) const;
		void setUniformArrayi(const std::string& name, size_t count, const int* data) const;

		void setUniform1f(const std::string& name, float value) const;
		void setUniform2f(const std::string& name, const glm::vec2& value) const;
		void setUniform3f(const std::string& name, const glm::vec3& value) const;
		void setUniform4f(const std::string& name, const glm::vec4& value) const;

		void setUniformMat3(const std::string& name, const glm::mat3& value) const;
		void setUniformMat4(const std::string& name, const glm::mat4& value) const;

	private:
		uint32_t m_Id = 0;
		mutable std::unordered_map<std::string, int> m_uniformLocationCache;

		const std::string ReadShaderFile(const std::string& filepath);
		uint32_t CompileShader(uint32_t type, const char* source);

		int getUniformLocation(const std::string& name) const;
	};

}