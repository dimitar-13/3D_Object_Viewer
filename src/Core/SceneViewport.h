#pragma once
#include "pch.h"
namespace OBJ_Viewer {

	struct Viewport
	{
		int x;
		int y;
		int width;
		int height;
		bool operator ==(const Viewport& other)const
		{
			return this->x == other.x         &&
				   this->y == other.y		  &&
				   this->width == other.width &&
				   this->height == other.height;
		}
		bool operator !=(const Viewport& other)const
		{
			return !Viewport::operator==(other);
		}
	};

	class SceneViewport
	{
	public:
		SceneViewport(const Viewport& viewport):m_viewport(viewport)
		{ RecalculateViewportMatrix();}

		Size2D GetViewportOffset()const { return Size2D{ m_viewport.x,m_viewport.y }; }
		Size2D GetViewportSize()const { return Size2D{ m_viewport.width,m_viewport.height }; }
		glm::mat3& GetViewportMatrix() { return m_viewPortMatrix; }
		const glm::mat3& GetViewportMatrix()const { return m_viewPortMatrix; }
		const Viewport& GetViewport()const { return m_viewport; }
		void UpdateSceneViewport(const Viewport& newViewport)
		{
			assert(!(newViewport.x < 0 || newViewport.y < 0 || newViewport.width < 0 || newViewport.height < 0));

			m_viewport = newViewport;
			RecalculateViewportMatrix();
		}
	private:
		void RecalculateViewportMatrix()
		{
			m_viewPortMatrix = glm::mat3(1);

			m_viewPortMatrix[0][0] = (float)m_viewport.width / 2.f;
			m_viewPortMatrix[1][1] = (float)m_viewport.height / 2.f;

			m_viewPortMatrix[2][0] = m_viewPortMatrix[0][0];
			m_viewPortMatrix[2][1] = m_viewPortMatrix[1][1];
		}
	private:
		Viewport m_viewport;
		glm::mat3 m_viewPortMatrix = glm::mat3(1);
	};
}