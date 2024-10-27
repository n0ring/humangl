#include <fstream>
#include <sstream>
#include "Shader.hpp"

Shader::Shader(const std::string &filePath, const std::unordered_map<std::string, std::string>& varibles) : m_filePath(filePath), m_renderID(0)
{
	ShaderProgramSourse source = parseShader(varibles);
	m_renderID = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_renderID);
}

void Shader::bind() const
{
	glUseProgram(m_renderID);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::setUniform1i(const std::string &name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform1f(const std::string &name, float value)
{
	glUniform1f(getUniformLocation(name), value);
}


void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniformMat4f(const std::string &name, nrg::mat4 &matrix)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniformVec3(const std::string &name, nrg::vec3 data)
{
	glUniform3f(getUniformLocation(name), data.x, data.y, data.z);
}

void Shader::setUniformbuffer(const std::string &name, unsigned int ubo_id, unsigned int objId)
{
	GLuint bindingPoint = objId;
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo_id);

	GLuint blockIndex = glGetUniformBlockIndex(m_renderID, name.c_str());
	glUniformBlockBinding(m_renderID, blockIndex, bindingPoint);
}

int Shader::getUniformLocation(const std::string &name)
{
	if (m_uniformLocationCache.count(name))
		return m_uniformLocationCache[name];
	int location = glGetUniformLocation(m_renderID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform " << name << " not found\n";
	m_uniformLocationCache[name] = location;
	return location;
}

ShaderProgramSourse Shader::parseShader(const std::unordered_map<std::string, std::string>& varibles)
{
	std::ifstream file(m_filePath);
	std::string line;
	size_t var_start;

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;
	std::stringstream ss[2];

	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			var_start = line.find("$");
			if (var_start != std::string::npos)
			{
				size_t var_len = 1;
				while (var_start < line.size() && line[var_start + var_len++] != '$')
					;
				std::string var_name = line.substr(var_start + 1, var_len - 2);
				if (varibles.count(var_name))
				{
					line.replace(var_start, var_len, varibles.at(var_name));
				}
			}
			ss[(int)type] << line << '\n';
		}
	}
	return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::compileShader(unsigned int type, const std::string source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
	int linkResult;
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
	if (linkResult == GL_FALSE)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char *message = (char *)alloca(length * sizeof(char));
		glGetProgramInfoLog(program, length, &length, message);
		std::cout << "Failed to link the shader program!" << std::endl;
		std::cout << message << std::endl;
		glDeleteProgram(program);
		return 0;
	}

	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}