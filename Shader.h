#pragma once

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using namespace glm;

class Shader
{
private:
	string m_shaderName;
	GLenum m_shaderID;

public:
	Shader(GLenum shaderID, string shaderName);

public:
	void Use();

	GLenum GetShaderID();
	string GetShaderName();

	GLuint GetUniformLocation(char *uniformName);

	void SetUniformValue(char *uniformName, mat4 &matrix);
	void SetUniformValue(char *uniformName, const vector<mat4> &matrix);
};