#pragma once

#include <glm/glm.hpp>

namespace Acrylic
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float Left, float Right, float Bottom, float Top);

		const glm::mat4& GetProjectionMatrix() { return ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() { return ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() { return ViewProjectionMatrix; }
		const glm::vec3& GetPosition() { return Position; }
		float			 GetRotation() { return Rotation; }

		void SetPosition(const glm::vec3& InPosition)
		{
			Position = InPosition;
			RecalculateViewMatrix();
		}
		void SetRotation(float InRotation)
		{
			Rotation = InRotation;
			RecalculateViewMatrix();
		}

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ViewProjectionMatrix;

		glm::vec3 Position;
		float	  Rotation = 0.f;
	};
} // namespace Acrylic
