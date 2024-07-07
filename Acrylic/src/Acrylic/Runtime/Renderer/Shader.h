#pragma once

#include "Core/Core.h"
#include "Core/Memory.h"
#include "Core/Containers/String.h"
#include "Core/Containers/UnorderedMap.h"

#include <glm/glm.hpp>

namespace Acrylic
{
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const String& GetName() const = 0;

		// TEMP (to be removed)
		virtual void UploadUniformInt(const String& Name, int Value) const = 0;
		virtual void UploadUniformIntArray(const String& Name, int* Value, uint32_t Count) const = 0;

		virtual void UploadUniformFloat(const String& Name, float Value) const = 0;
		virtual void UploadUniformFloat2(const String& Name, const glm::vec2& Vector) const = 0;
		virtual void UploadUniformFloat3(const String& Name, const glm::vec3& Vector) const = 0;
		virtual void UploadUniformFloat4(const String& Name, const glm::vec4& Vector) const = 0;

		virtual void UploadUniformMat3(const String& Name, const glm::mat3& Matrix) const = 0;
		virtual void UploadUniformMat4(const String& Name, const glm::mat4& Matrix) const = 0;

		static TSharedPtr<IShader> Create(const String& Path, const String& Name = "");
		static TSharedPtr<IShader> Create(const String& VertexSrc, const String& FragmentSrc, const String& Name);
	};

	class ShaderLibrary
	{
	public:
		void Add(const TSharedPtr<IShader>& Shader);
		TSharedPtr<IShader> Load(const String& Path, const String& Name = "");
		TSharedPtr<IShader> Load(const String& VertexSrc, const String& FragmentSrc, const String& Name);

		TSharedPtr<IShader> Get(const String& Name = "");
	private:
		TUnorderedMap<String, TSharedPtr<IShader>> Shaders;
	};
}