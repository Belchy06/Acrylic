#pragma once

#include "Core/Logging/Logging.h"
#include "Renderer/Shader.h"

namespace Acrylic
{
	DECLARE_LOG_CATEGORY_EXTERN(LogOpenGLShader, Log)

	class OpenGLShader : public IShader
	{
	public:
		OpenGLShader(const String& VertexSrc, const String& FragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const String& Name, int Value);

		void UploadUniformFloat(const String& Name, float Value);
		void UploadUniformFloat2(const String& Name, const glm::vec2& Vector);
		void UploadUniformFloat3(const String& Name, const glm::vec3& Vector);
		void UploadUniformFloat4(const String& Name, const glm::vec4& Vector);

		void UploadUniformMat3(const String& Name, const glm::mat3& Matrix);
		void UploadUniformMat4(const String& Name, const glm::mat4& Matrix);

	private:
		uint32_t RendererId;
	};
} // namespace Acrylic