#include "Shader.h"

Shader::Shader(GLenum shaderID, string shaderName) : m_shaderID(shaderID), m_shaderName(shaderName) {}

void Shader::Use()
{
	glUseProgram(m_shaderID);
}


GLenum Shader::GetShaderID()
{
	return m_shaderID;
}

string Shader::GetShaderName()
{
	return m_shaderName;
}


GLuint Shader::GetUniformLocation(char * uniformName)
{
	return glGetUniformLocation(m_shaderID, uniformName);
}

void Shader::SetUniformValue(char * uniformName, mat4 &matrix)
{
	glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, value_ptr(matrix));
}

void Shader::SetUniformValue(char *uniformName, const vector<mat4> &matrix)
{
	Use();
	char name[100];
	for (int i = 0; i < matrix.size(); i++)
	{
		sprintf(name, "%s[%d]", uniformName, i);
		int location = glGetUniformLocation(m_shaderID, name);
		mat4 m = matrix[i];
		float *f = value_ptr(m);
		//value_ptr(matrix[i])
		glUniformMatrix4fv(location, 1, GL_FALSE, f);
		//glUniformMatrix4fv(location, matrix.size(), GL_FALSE, value_ptr(*matrix.data()));
	}
}
