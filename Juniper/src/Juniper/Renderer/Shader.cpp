#include "pch.h"
#include <glad/glad.h>
#include "Juniper/Core/Log.h"
#include "Shader.h"

namespace Juniper {

	Shader::Shader(const std::string& vsPath, const std::string& fsPath)
	{
		JP_CORE_TRACE("Creating shader: [ vs: {0}, fs: {1} ]", vsPath, fsPath);

		uint32_t vs = CompileShader(GL_VERTEX_SHADER, ReadShaderFile(vsPath).c_str());
		uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, ReadShaderFile(fsPath).c_str());

		if (!vs || !fs) return;

		m_Id = glCreateProgram();
		glAttachShader(m_Id, vs);
		glAttachShader(m_Id, fs);
		glLinkProgram(m_Id);

		// Check for link errors
		GLint success;
		glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(m_Id, 512, nullptr, infoLog);
			JP_CORE_ERROR("Shader link error: {0}", infoLog);
		}

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_Id);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_Id);
	}

	void Shader::BindUniformBlock(const std::string& name, uint32_t slot) const
	{
		uint32_t blockIndex = glGetUniformBlockIndex(m_Id, name.c_str());
		glUniformBlockBinding(m_Id, blockIndex, slot);
	}

	const std::string Shader::ReadShaderFile(const std::string& filepath)
	{
		std::ifstream fs(filepath);
		JP_CORE_ASSERT(fs, "Failed to open shader: {0}", filepath);
		
		std::stringstream buffer;
		buffer << fs.rdbuf();
		return buffer.str();
	}

	uint32_t Shader::CompileShader(uint32_t type, const char* source)
	{
		uint32_t shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		// Check for errors
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			JP_CORE_ERROR("Shader compilation error: {0}", infoLog);	
			return 0;
		}

		return shader;
	}

	void Shader::setUniform1i(const std::string& name, int value) const
	{
		glUniform1i(getUniformLocation(name), value);
	}

	void Shader::setUniformArrayi(const std::string& name, int count, const int* values) const
	{
		glUniform1iv(getUniformLocation(name), count, values);
	}

	void Shader::setUniform1f(const std::string& name, float value) const
	{
		glUniform1f(getUniformLocation(name), value);
	}

	void Shader::setUniform2f(const std::string& name, const glm::vec2& value) const
	{
		glUniform2f(getUniformLocation(name), value.x, value.y);
	}

	void Shader::setUniform3f(const std::string& name, const glm::vec3& value) const
	{
		glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
	}

	void Shader::setUniform4f(const std::string& name, const glm::vec4& value) const
	{
		glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void Shader::setUniformMat3(const std::string& name, const glm::mat3& value) const
	{
		glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
	}

	void Shader::setUniformMat4(const std::string& name, const glm::mat4& value) const
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
	}

	int Shader::getUniformLocation(const std::string& name) const {
		if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
			return m_uniformLocationCache[name];

		int location = glGetUniformLocation(m_Id, name.c_str());
		if (location == -1)
			JP_CORE_WARN("Location not found for uniform {0}", name);

		m_uniformLocationCache[name] = location;
		return location;
	}

}