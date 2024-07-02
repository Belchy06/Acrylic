#include "Acrylic.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSandbox, Log);
DEFINE_LOG_CATEGORY(LogSandbox);

class ExampleLayer : public Acrylic::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
		Camera = std::unique_ptr<Acrylic::OrthographicCamera>(new Acrylic::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f));
		CameraPosition = Camera->GetPosition();
		CameraRotation = static_cast<int>(Camera->GetRotation());

		// clang-format off
		float Vertices[4 * 7] = {
			-0.75f, -0.75f, 0.0f, 0.8f, 0.2f, 0.8f, 1.f,
			 0.75f, -0.75f, 0.0f, 0.2f, 0.3f, 0.8f, 1.f,
			 0.75f,   0.75f, 0.0f, 0.8f, 0.8f, 0.2f, 1.f,
			-0.75f,   0.75f, 0.0f, 0.2f, 0.8f, 0.8f, 1.f
		};
		// clang-format on

		std::shared_ptr<Acrylic::IVertexBuffer> ExampleVertexBuffer = std::shared_ptr<Acrylic::IVertexBuffer>(Acrylic::IVertexBuffer::Create(Vertices, sizeof(Vertices) / sizeof(float)));
		ExampleVertexBuffer->SetLayout({ { Acrylic::EDataType::Float3, "a_Position" },
			{ Acrylic::EDataType::Float4, "a_Color" } });

		// clang-format off
		uint32_t Indices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		// clang-format on
		std::shared_ptr<Acrylic::IIndexBuffer> ExampleIndexBuffer = std::shared_ptr<Acrylic::IIndexBuffer>(Acrylic::IIndexBuffer::Create(Indices, sizeof(Indices) / sizeof(uint32_t)));

		std::shared_ptr<Acrylic::IVertexArray> ExampleVertexArray = std::shared_ptr<Acrylic::IVertexArray>(Acrylic::IVertexArray::Create());
		ExampleVertexArray->AddVertexBuffer(ExampleVertexBuffer);
		ExampleVertexArray->SetIndexBuffer(ExampleIndexBuffer);

		std::string VertexSrc = R"(
			#version 330 core

			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
  
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			})";

		std::string FragmentSrc = R"(
			#version 330 core

			layout (location = 0) out vec4 color;
	  
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			})";

		std::shared_ptr<Acrylic::IShader> ExampleShader = std::shared_ptr<Acrylic::IShader>(Acrylic::IShader::Create(VertexSrc, FragmentSrc));

		ShaderVertexArrayPairs.push_back({ ExampleShader, ExampleVertexArray });
	}

	virtual void OnUpdate() override
	{
		if (Acrylic::IInput::IsKeyPressed(AC_KEY_W))
		{
			CameraPosition.y += 0.1f;
		}
		else if (Acrylic::IInput::IsKeyPressed(AC_KEY_S))
		{
			CameraPosition.y -= 0.1f;
		}

		if (Acrylic::IInput::IsKeyPressed(AC_KEY_A))
		{
			CameraPosition.x -= 0.1f;
		}
		else if (Acrylic::IInput::IsKeyPressed(AC_KEY_D))
		{
			CameraPosition.x += 0.1f;
		}

		if (Acrylic::IInput::IsKeyPressed(AC_KEY_Q))
		{
			CameraRotation = (CameraRotation + 1) % 360;
		}
		else if (Acrylic::IInput::IsKeyPressed(AC_KEY_E))
		{
			CameraRotation = (CameraRotation - 1) % 360;
		}

		Acrylic::CommandList::SetClearColour({ 1.f, 0.1f, 0.1f, 1.f });
		Acrylic::CommandList::Clear();

		Camera->SetPosition(CameraPosition);
		Camera->SetRotation(static_cast<float>(CameraRotation));

		Acrylic::Renderer::BeginScene();

		for (const auto& ShaderVertexArrayPair : ShaderVertexArrayPairs)
		{
			Acrylic::Renderer::Submit(ShaderVertexArrayPair.first, ShaderVertexArrayPair.second);
		}

		Acrylic::Renderer::EndScene();
	}

	virtual void OnEvent(Acrylic::Event& e) override
	{
	}

private:
	std::vector<std::pair<std::shared_ptr<Acrylic::IShader>, std::shared_ptr<Acrylic::IVertexArray>>> ShaderVertexArrayPairs;

	std::shared_ptr<Acrylic::OrthographicCamera> Camera;
	glm::vec3									 CameraPosition;
	int											 CameraRotation;
};

class Sandbox : public Acrylic::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	virtual ~Sandbox() = default;
};

Acrylic::Application* Acrylic::CreateApplication()
{
	return new Sandbox();
}