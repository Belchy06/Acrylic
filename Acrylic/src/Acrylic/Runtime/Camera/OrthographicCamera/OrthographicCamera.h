#pragma once

#include "Camera/Camera.h"

namespace Acrylic
{
	class OrthographicCamera : public ICamera
	{
	public:
		OrthographicCamera(float Left, float Right, float Bottom, float Top);

		virtual const glm::mat4& GetProjectionMatrix() override { return ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix() override { return ViewMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() override { return ViewProjectionMatrix; }
		virtual const glm::vec3& GetPosition() override { return Position; }
		virtual float			 GetRotation() override { return Rotation; }

		virtual void SetPosition(const glm::vec3& InPosition) override 
		{
			Position = InPosition;
			RecalculateViewMatrix();
		}
		virtual void SetRotation(float InRotation) override 
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
