#pragma once
#include <GLM\glm.hpp>

namespace Dragon
{
	namespace Math
	{
		using namespace glm;
		class Rect
		{
		public:
			Rect(ivec2 rt, int width, int height) : m_rt(rt)
			{
				m_width = width;
				m_height = height;
			}

			const ivec2& GetRT(void) { return m_rt; }
			int GetWidth(void) { return m_width; }
			int GetHeight(void) { return m_height; }

		private:
			ivec2 m_rt;
			int m_width, m_height;
		};

		class MathUtility
		{
		public:
			static bool RectInterSect(Rect &rect1, Rect &rect2);
		};
	}
}