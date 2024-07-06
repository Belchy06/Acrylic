#pragma once

#include "Core/Containers/UnorderedMap.h"
#include "Core/Logging.h"
#include "Renderer/Shader.h"

namespace Acrylic
{
	DECLARE_LOG_CATEGORY_EXTERN(LogOpenGLShader, Log)

	class OpenGLShader : public IShader
	{
	public:
		OpenGLShader(const String& Path);
		OpenGLShader(const String& VertexSrc, const String& FragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadUniformInt(const String& Name, int Value) const override;

		virtual void UploadUniformFloat(const String& Name, float Value) const override;
		virtual void UploadUniformFloat2(const String& Name, const glm::vec2& Vector) const override;
		virtual void UploadUniformFloat3(const String& Name, const glm::vec3& Vector) const override;
		virtual void UploadUniformFloat4(const String& Name, const glm::vec4& Vector) const override;

		virtual void UploadUniformMat3(const String& Name, const glm::mat3& Matrix) const override;
		virtual void UploadUniformMat4(const String& Name, const glm::mat4& Matrix) const override;

	private:
		String								ReadFile(const String& Path);
		TUnorderedMap<unsigned int, String> Preprocess(const String& Source);
		void								Compile(const TUnorderedMap<unsigned int, String>& Sources);

	private:
		uint32_t RendererId;
	};
} // namespace Acrylic