#pragma once

#include "Camera/Camera.h"
#include "Core/Core.h"
#include "Core/Misc/EnumClassFlags.h"
#include "Renderer/CommandList.h"
#include "Renderer/VertexArray.h"

namespace Acrylic
{
	enum class ERenderInterface : uint8_t
	{
		None = 0,
		OpenGL
	};

	ENUM_CLASS_FLAGS(ERenderInterface)

	class ACRYLIC_API Renderer
	{
	public:
		static void Init(); 
		static void BeginScene(std::shared_ptr<ICamera> Camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<IShader> Shader, const std::shared_ptr<IVertexArray>& VertexArray, const glm::mat4 TransformationMatrix = glm::mat4(1.f));

		static FORCEINLINE ERenderInterface GetRenderInterface() { return API; }

	private:
		inline static ERenderInterface API = ERenderInterface::OpenGL;

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		inline static SceneData* Data = new SceneData();
	};
} // namespace Acrylic
