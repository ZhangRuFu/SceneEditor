#include <exception>
#include "FreeType.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "Shader.h"
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>

using std::exception;

#pragma comment(lib, "freetype271.lib")

FontRender::FontRender(string shaderName) : Drawer(shaderName)
{
	if (m_graphicBuffer == nullptr)
		Init();
	m_buffers = m_graphicBuffer;
}

void FontRender::Init(void)
{
	m_graphicBuffer = new GraphicsBuffer();
	unsigned int fontVAO = -1;
	unsigned int fontVBO = -1;
	glGenVertexArrays(1, &fontVAO);
	glGenBuffers(1, &fontVBO);
	m_graphicBuffer->m_vbo.push_back(fontVBO);
	m_graphicBuffer->m_vao.push_back(fontVAO);

	glBindVertexArray(fontVAO);
	glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * 5 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//加载字体
	vector<const unsigned char*> buffer;
	FreeType freetype("E:\\arial.ttf", m_rawFontSize);
	freetype.GetCharacters(m_charSet, buffer);
	int fontCount = m_charSet.size();
	unsigned int *textures = new unsigned int[fontCount];
	glGenTextures(fontCount, textures);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (int i = 0; i < fontCount; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_charSet[i].m_width, m_charSet[i].m_height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_charSet[i].m_texID = textures[i];
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] textures;
}

void FontRender::DrawText(const string &str, vec2 position, int fontSize, vec3 color, float depth)
{
	float scaleFactor = (float)fontSize / m_rawFontSize;
	mat4 model;
	model = glm::scale(model, vec3(scaleFactor));
	glUniform3fv(m_shader->GetUniformLocation("textColor"), 1, value_ptr(color));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_buffers->m_vao[0]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_shader->SetUniformValue("model", model);
	for (string::const_iterator iterater = str.begin(); iterater != str.end(); iterater++)
	{
		const Character &ch = m_charSet[(*iterater)];
		/*int fontX = position.x + ch.m_bearingX;
		int fontY = -position.y - (ch.m_height - ch.m_bearingY);
		float vertices[6][4]{
			fontX, fontY + ch.m_height, 0.0, 0.0,
			fontX, fontY, 0.0, 1.0,
			fontX + ch.m_width, fontY, 1.0, 1.0,
			fontX, fontY + ch.m_height, 0.0, 0.0,
			fontX + ch.m_width, fontY , 1.0, 1.0,
			fontX + ch.m_width, fontY + ch.m_height, 1.0, 0.0,
		};*/

		int fontX = position.x + ch.m_bearingX;
		int fontY = -position.y - ch.m_ascender + ch.m_bearingY;
		float vertices[6][5]{
			fontX, fontY, depth, 0.0, 0.0,
			fontX, fontY - ch.m_height, depth, 0.0, 1.0,
			fontX + ch.m_width, fontY - ch.m_height, depth, 1.0, 1.0,
			fontX, fontY, depth, 0.0, 0.0,
			fontX + ch.m_width, fontY - ch.m_height, depth, 1.0, 1.0,
			fontX + ch.m_width, fontY, depth, 1.0, 0.0,
		};

		glBindTexture(GL_TEXTURE_2D, ch.m_texID);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers->m_vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		position.x += (ch.m_advance >> 6);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void FontRender::GetDimension(const string &str, int & width, int & height)
{
	width = 0;
	for (string::const_iterator iterater = str.begin(); iterater != str.end(); iterater++)
	{
		const Character &ch = m_charSet[(*iterater)];
		width += (ch.m_advance >> 6);
		if (ch.m_height > height)
			height = ch.m_height + 2 * (ch.m_ascender - ch.m_bearingY);
	}
}

void FontRender::PublicSet(void)
{
	mat4 windowProjection = ResourceSystem::GetMainCamera()->GenWindowProjectionMatrix(10.0f, -10.0f);
	m_shader->SetUniformValue("projection", windowProjection);
}



FreeType::FreeType(string fontPath, int height, int width) : m_ftLibrary(nullptr), m_ftFace(nullptr), m_fontWidth(width), m_fontHeight(height)
{
	FT_Init_FreeType(&m_ftLibrary);
	if (!m_ftLibrary)
		throw exception("Error:FreeType->FreeType初始化失败");
	m_fontPath = fontPath;
	if (FT_New_Face(m_ftLibrary, m_fontPath.c_str(), 0, &m_ftFace))
	{
		m_ftFace = nullptr;
		throw exception("Error:FreeType->字体加载出错!");
	}
	FT_Set_Pixel_Sizes(m_ftFace, m_fontWidth, m_fontHeight);
}

FreeType::~FreeType(void)
{
	FT_Done_Face(m_ftFace);
	FT_Done_FreeType(m_ftLibrary);
}

void FreeType::GetCharacters(vector<Character> &charSet, vector<const unsigned char*> &buffer)
{
	charSet.resize(s_charCount);
	buffer.resize(s_charCount);
	unsigned char* fontBuffer = nullptr;
	Character::m_ascender = m_ftFace->size->metrics.ascender >> 6;
	for (unsigned long ch = 0; ch < s_charCount; ch++)
	{
		if (FT_Load_Char(m_ftFace, ch, FT_LOAD_RENDER))
		{
			//出错，待处理
		}
		FT_GlyphSlot glyph = m_ftFace->glyph;
		charSet[ch].m_width = glyph->bitmap.width;
		charSet[ch].m_height = glyph->bitmap.rows;
		charSet[ch].m_bearingX = glyph->bitmap_left;
		charSet[ch].m_bearingY = glyph->bitmap_top;
		charSet[ch].m_advance = glyph->advance.x;
		size_t bufferSize = charSet[ch].m_width * charSet[ch].m_height;
		fontBuffer = new unsigned char[charSet[ch].m_width * charSet[ch].m_height];
		memcpy(fontBuffer, glyph->bitmap.buffer, bufferSize);
		buffer[ch] = fontBuffer;
	}
}


GraphicsBuffer *FontRender::m_graphicBuffer = nullptr;
vector<Character> FontRender::m_charSet;
int Character::m_ascender = 0;