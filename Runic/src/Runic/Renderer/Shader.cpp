#include "pch.hpp"
#include "Runic/Renderer/Shader.hpp"

#include "glad/gl.h"
#include "glm/gtc/type_ptr.hpp"

namespace Runic
{
Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	const GLuint vertexShader{glCreateShader(GL_VERTEX_SHADER)};

	auto source{vertexSrc.c_str()};
	glShaderSource(vertexShader, 1, &source, nullptr);
	glCompileShader(vertexShader);

	GLint isCompiled{0};
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength{0};
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(vertexShader);

		RUNIC_CORE_ERROR("{}", infoLog.data());
		RUNIC_CORE_ASSERT(false, "Vertex shader compilation failed!");

		return;
	}

	const GLuint fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};

	source = fragmentSrc.c_str();
	glShaderSource(fragmentShader, 1, &source, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength{0};
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		RUNIC_CORE_ERROR("{}", infoLog.data());
		RUNIC_CORE_ASSERT(false, "Fragment shader compilation failed!");

		return;
	}

	_rendererID = glCreateProgram();

	glAttachShader(_rendererID, vertexShader);
	glAttachShader(_rendererID, fragmentShader);
	glLinkProgram(_rendererID);

	GLint isLinked{0};
	glGetProgramiv(_rendererID, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength{0};
		glGetProgramiv(_rendererID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(_rendererID, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(_rendererID);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		RUNIC_CORE_ERROR("{}", infoLog.data());
		RUNIC_CORE_ASSERT(false, "Shader linking failed!");

		return;
	}

	glDetachShader(_rendererID, vertexShader);
	glDetachShader(_rendererID, fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(_rendererID);
}

void Shader::bind() const
{
	glUseProgram(_rendererID);
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::uploadUniformFloat4(const std::string& name, const glm::vec4& vector) const
{
	const GLint location = glGetUniformLocation(_rendererID, name.c_str());
	glUniform4fv(location, 1, glm::value_ptr(vector));
}

void Shader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
{
	const GLint location = glGetUniformLocation(_rendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
}
