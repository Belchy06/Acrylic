#include "acpch.h"
#include "OpenGLShader.h"

#include "Core/Containers/Pair.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <filesystem>

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogOpenGLShader);

	static GLenum ShaderTypeFromString(String Type)
	{
		if (Type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (Type == "fragment" || Type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		AC_ASSERT(false);
		return 0;
	}

	OpenGLShader::OpenGLShader(const String& Path, const String& InName)
		: RendererId(0)
	{
		String ShaderSource = ReadFile(Path);
		if (ShaderSource.empty())
		{
			return;
		}

		TUnorderedMap<GLenum, String> ShaderSources = Preprocess(ShaderSource);

		Compile(ShaderSources);

		if (InName.empty())
		{
			// Extract file name from path
			std::filesystem::path FSPath = Path;
			Name = FSPath.stem().string(); 
		}
		else
		{
			Name = InName;
		}
	}

	OpenGLShader::OpenGLShader(const String& VertexSrc, const String& FragmentSrc, const String& Name)
		: RendererId(0)
		, Name(Name)
	{
		TUnorderedMap<GLenum, String> ShaderSources;
		ShaderSources[GL_VERTEX_SHADER] = VertexSrc;
		ShaderSources[GL_FRAGMENT_SHADER] = FragmentSrc;

		Compile(ShaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
	}

	String OpenGLShader::ReadFile(const String& Path)
	{
		String		  Result;
		std::ifstream Stream(Path, std::ios::in | std::ios::binary);
		if (!Stream)
		{
			AC_LOG(LogOpenGLShader, Error, "No shader found at path: {0}", Path);
			return "";
		}

		Stream.seekg(0, std::ios::end);
		Result.resize(Stream.tellg());

		Stream.seekg(std::ios::beg);
		Stream.read(&Result[0], Result.size());
		Stream.close();

		if (Result.empty())
		{
			AC_LOG(LogOpenGLShader, Error, "Shader source at [{0}] is empty!", Path);
			return "";
		}

		return Result;
	}

	TUnorderedMap<GLenum, String> OpenGLShader::Preprocess(const String& Source)
	{
		TUnorderedMap<GLenum, String> ShaderSources;

		const char* TypeToken = "#type";
		size_t		TypeTokenLength = strlen(TypeToken);
		size_t		Pos = Source.find(TypeToken, 0);
		while (Pos != std::string::npos)
		{
			size_t EndOfLine = Source.find_first_of("\r\n", Pos);
			AC_ASSERT(EndOfLine != std::string::npos);

			size_t Begin = Pos + TypeTokenLength + 1;
			String Type = Source.substr(Begin, EndOfLine - Begin);
			AC_ASSERT(ShaderTypeFromString(Type));

			size_t NextLinePos = Source.find_first_not_of("\r\n", EndOfLine);
			Pos = Source.find(TypeToken, NextLinePos);
			ShaderSources[ShaderTypeFromString(Type)] = Source.substr(NextLinePos, Pos - (NextLinePos == std::string::npos ? Source.size() - 1 : NextLinePos));
		}

		return ShaderSources;
	}

	void OpenGLShader::Compile(const TUnorderedMap<GLenum, String>& Sources)
	{
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint GLProgram = glCreateProgram();

		TArray<GLenum> ShaderIds;
		ShaderIds.reserve(Sources.size());

		for (const TPair<GLenum, String>& SourcePair : Sources)
		{
			GLuint Shader = glCreateShader(SourcePair.first);

			const GLchar* SourceCString = (const GLchar*)SourcePair.second.c_str();
			glShaderSource(Shader, 1, &SourceCString, 0);

			glCompileShader(Shader);

			GLint bIsCompiled = 0;
			glGetShaderiv(Shader, GL_COMPILE_STATUS, &bIsCompiled);
			if (bIsCompiled == GL_FALSE)
			{
				GLint MaxLength = 0;
				glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &MaxLength);

				TArray<GLchar> InfoLog(MaxLength);
				glGetShaderInfoLog(Shader, MaxLength, &MaxLength, &InfoLog[0]);

				glDeleteShader(Shader);

				String InfoString(InfoLog.begin(), InfoLog.end());
				AC_LOG(LogOpenGLShader, Error, "Failed to compile shader: {0}", InfoString);

				return;
			}

			glAttachShader(GLProgram, Shader);
			ShaderIds.push_back(Shader);
		}

		glLinkProgram(GLProgram);

		GLint isLinked = 0;
		glGetProgramiv(GLProgram, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint MaxLength = 0;
			glGetProgramiv(GLProgram, GL_INFO_LOG_LENGTH, &MaxLength);

			TArray<GLchar> InfoLog(MaxLength);
			glGetProgramInfoLog(GLProgram, MaxLength, &MaxLength, &InfoLog[0]);

			glDeleteProgram(GLProgram);

			for (GLenum& ShaderId : ShaderIds)
			{
				glDeleteShader(ShaderId);
			}

			String InfoString(InfoLog.begin(), InfoLog.end());
			AC_LOG(LogOpenGLShader, Error, "Failed to link program: {0}", InfoString);

			return;
		}

		for (GLenum& ShaderId : ShaderIds)
		{
			glDetachShader(GLProgram, ShaderId);
		}

		RendererId = GLProgram;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(RendererId);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const String& Name, int Value) const
	{
		// BIND PROGRAM BEFORE UPLOADING

		GLint Location = glGetUniformLocation(RendererId, Name.c_str());
		if (Location == -1)
		{
			AC_LOG(LogOpenGLShader, Error, "Float uniform not found: {0}", Name);
			return;
		}
		glUniform1i(Location, Value);
	}

	void OpenGLShader::UploadUniformFloat(const String& Name, float Value) const
	{
		// BIND PROGRAM BEFORE UPLOADING

		GLint Location = glGetUniformLocation(RendererId, Name.c_str());
		if (Location == -1)
		{
			AC_LOG(LogOpenGLShader, Error, "Float uniform not found: {0}", Name);
			return;
		}
		glUniform1f(Location, Value);
	}

	void OpenGLShader::UploadUniformFloat2(const String& Name, const glm::vec2& Vector) const
	{
		// BIND PROGRAM BEFORE UPLOADING

		GLint Location = glGetUniformLocation(RendererId, Name.c_str());
		if (Location == -1)
		{
			AC_LOG(LogOpenGLShader, Error, "Vec2 uniform not found: {0}", Name);
			return;
		}
		glUniform2f(Location, Vector.x, Vector.y);
	}

	void OpenGLShader::UploadUniformFloat3(const String& Name, const glm::vec3& Vector) const
	{
		// BIND PROGRAM BEFORE UPLOADING

		GLint Location = glGetUniformLocation(RendererId, Name.c_str());
		if (Location == -1)
		{
			AC_LOG(LogOpenGLShader, Error, "Vec3 uniform not found: {0}", Name);
			return;
		}
		glUniform3f(Location, Vector.x, Vector.y, Vector.z);
	}

	void OpenGLShader::UploadUniformFloat4(const String& Name, const glm::vec4& Vector) const
	{
		// BIND PROGRAM BEFORE UPLOADING

		GLint Location = glGetUniformLocation(RendererId, Name.c_str());
		if (Location == -1)
		{
			AC_LOG(LogOpenGLShader, Error, "Vec4 uniform not found: {0}", Name);
			return;
		}
		glUniform4f(Location, Vector.x, Vector.y, Vector.z, Vector.w);
	}

	void OpenGLShader::UploadUniformMat3(const String& Name, const glm::mat3& Matrix) const
	{
		// BIND PROGRAM BEFORE UPLOADING

		GLint Location = glGetUniformLocation(RendererId, Name.c_str());
		if (Location == -1)
		{
			AC_LOG(LogOpenGLShader, Error, "Mat4 uniform not found: {0}", Name);
			return;
		}
		glUniformMatrix3fv(Location, 1, GL_FALSE, glm::value_ptr(Matrix));
	}

	void OpenGLShader::UploadUniformMat4(const String& Name, const glm::mat4& Matrix) const
	{
		// BIND PROGRAM BEFORE UPLOADING

		GLint Location = glGetUniformLocation(RendererId, Name.c_str());
		if (Location == -1)
		{
			AC_LOG(LogOpenGLShader, Error, "Mat4 uniform not found: {0}", Name);
			return;
		}
		glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(Matrix));
	}
} // namespace Acrylic
