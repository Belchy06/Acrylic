#include "acpch.h"
#include "Particle.h"

#include "Core/Random.h"

#include "Renderer/Renderer2D.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

namespace Acrylic
{
	ParticleSystem::ParticleSystem(size_t MaxParticles)
	{
		ParticlePool.resize(MaxParticles);
	}

	void ParticleSystem::OnUpdate(Timestep ts)
	{
		for (Particle& Part : ParticlePool)
		{
			if (!Part.bActive)
			{
				continue;
			}

			if (Part.LifeRemaining <= 0.f)
			{
				Part.bActive = false;
				continue;
			}

			// TODO: Maybe update velocity with an acceleration (or deceleration) component

			Part.Position.Translation += Part.Velocity.Linear * (float)ts;
			Part.Position.Rotation += Part.Velocity.Angular * (float)ts;

			Part.LifeRemaining -= ts;
		}
	}

	void ParticleSystem::OnRender(TSharedPtr<ICamera> Camera)
	{
		Renderer2D::BeginScene(Camera);
		for (Particle& Part : ParticlePool)
		{
			if (!Part.bActive)
			{
				continue;
			}

			float	  Life = Part.LifeRemaining / Part.InitialProps.Lifetime;
			glm::vec4 Colour = glm::lerp(Part.InitialProps.Colour.End, Part.InitialProps.Colour.Begin, Life);

			float Size = glm::lerp(Part.InitialProps.Size.End, Part.InitialProps.Size.Begin, Life);

			Renderer2D::DrawQuad({
				.Position = Part.Position.Translation, //
				.Size = { Size, Size },				   //
				.Rotation = Part.Position.Rotation.z,  //
				.Colour = Colour					   //
			});
		}
		Renderer2D::EndScene();
	}

	void ParticleSystem::Emit(const ParticleProps& Props)
	{
		Particle& Part = ParticlePool[PoolIndex];
		Part.bActive = true;
		Part.InitialProps = Props;
		Part.LifeRemaining = Props.Lifetime;
		Part.Position.Translation = Props.Position.Translation;
		Part.Position.Rotation = Props.Position.Rotation;
		Part.Velocity.Linear = Props.Velocity.Linear.Initial + Props.Velocity.Linear.Variation * (Random::Float() - 0.5f);
		Part.Velocity.Angular = Props.Velocity.Angular.Initial + Props.Velocity.Angular.Variation * (Random::Float() - 0.5f);

		PoolIndex = ++PoolIndex % ParticlePool.size();
	}
} // namespace Acrylic