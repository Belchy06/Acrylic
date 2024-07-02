#pragma once

#include "Core/Logging/Logging.h"
#include "Renderer/Shader.h"

namespace Acrylic
{
	DECLARE_LOG_CATEGORY_EXTERN(LogOpenGLShader, Log)

	class OpenGLShader : public IShader
	{
	public:
		OpenGLShader(const std::string& VertexSrc, const std::string& FragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadUniformMat4(const std::string& Name, const glm::mat4& Matrix) override;

	private:
		uint32_t RendererId;
	};
} // namespace Acrylic