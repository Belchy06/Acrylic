#pragma once

#include "Camera/Camera.h"
#include "Core/Containers/Array.h"
#include "Core/Core.h"
#include "Core/Timestep.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Acrylic
{
	struct ACRYLIC_API ParticleProps
	{
		struct Position
		{
			glm::vec3 Translation;
			glm::vec3 Rotation;
		} Position;

		struct Velocity
		{
			struct Linear
			{
				glm::vec3 Initial;
				glm::vec3 Variation;
			} Linear;
			struct Angular
			{
				glm::vec3 Initial;
				glm::vec3 Variation;
			} Angular;
		} Velocity;

		struct Colour
		{
			glm::vec4 Begin;
			glm::vec4 End;
		} Colour;

		struct Size
		{
			float Begin;
			float End;
			float Variation;
		} Size;

		float Lifetime = 1.f;
	};

	class ACRYLIC_API ParticleSystem
	{
	public:
		ParticleSystem(size_t MaxParticles = 100000);

		void OnUpdate(Timestep ts);
		void OnRender(TSharedPtr<ICamera> Camera);

		void Emit(const ParticleProps& Props);

	private:
		struct Particle
		{
			// Current Settings
			struct Position
			{
				glm::vec3 Translation;
				glm::vec3 Rotation;
			} Position;

			struct Velocity
			{
				glm::vec3 Linear;
				glm::vec3 Angular;
			} Velocity;

			float LifeRemaining = 0.f;

			bool bActive = false;

			// Initial Settings
			ParticleProps InitialProps;
		};

		TVector<Particle> ParticlePool;
		uint32_t		  PoolIndex = 0;

		TSharedPtr<IVertexArray> QuadVertexArray = 0;
		TUniquePtr<IShader>		 Shader;

		int ShaderViewProj = -1;
		int ShaderTransform = -1;
		int ShaderColour = -1;
	};
} // namespace Acrylic