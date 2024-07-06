#include "acpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogOpenGLShader);

	OpenGLShader::OpenGLShader(const String& VertexSrc, const String& FragmentSrc)
		: RendererId(0)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that String's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)VertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			TArray<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that String's .c_str is NULL character terminated.
		source = (const GLchar*)FragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			TArray<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		RendererId = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(RendererId, vertexShader);
		glAttachShader(RendererId, fragmentShader);

		// Link our program
		glLinkProgram(RendererId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(RendererId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(RendererId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			TArray<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(RendererId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(RendererId);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(RendererId, vertexShader);
		glDetachShader(RendererId, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
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
}
