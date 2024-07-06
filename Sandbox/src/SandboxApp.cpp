#include "Acrylic.h"
#include <imgui.h>

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
		CameraRotation = Camera->GetRotation();

		// clang-format off
		float Vertices[4 * 7] = {
			-0.75f, -0.75f, 0.0f, 0.8f, 0.2f, 0.8f, 1.f,
			 0.75f, -0.75f, 0.0f, 0.2f, 0.3f, 0.8f, 1.f,
			 0.75f,   0.75f, 0.0f, 0.8f, 0.8f, 0.2f, 1.f,
			-0.75f,   0.75f, 0.0f, 0.2f, 0.8f, 0.8f, 1.f
		};
		// clang-format on

		Acrylic::TSharedPtr<Acrylic::IVertexBuffer> ExampleVertexBuffer = Acrylic::TSharedPtr<Acrylic::IVertexBuffer>(Acrylic::IVertexBuffer::Create(Vertices, sizeof(Vertices) / sizeof(float)));
		// clang-format off
		ExampleVertexBuffer->SetLayout({ 
			{ Acrylic::EDataType::Float3, "a_Position" },
			{ Acrylic::EDataType::Float4, "a_Color" } 
		});
		// clang-format on

		// clang-format off
		uint32_t Indices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		// clang-format on
		Acrylic::TSharedPtr<Acrylic::IIndexBuffer> ExampleIndexBuffer = Acrylic::TSharedPtr<Acrylic::IIndexBuffer>(Acrylic::IIndexBuffer::Create(Indices, sizeof(Indices) / sizeof(uint32_t)));

		Acrylic::TSharedPtr<Acrylic::IVertexArray> ExampleVertexArray = Acrylic::TSharedPtr<Acrylic::IVertexArray>(Acrylic::IVertexArray::Create());
		ExampleVertexArray->AddVertexBuffer(ExampleVertexBuffer);
		ExampleVertexArray->SetIndexBuffer(ExampleIndexBuffer);

		Acrylic::String VertexSrc = R"(
			#version 330 core

			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
  
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			})";

		Acrylic::String FragmentSrc = R"(
			#version 330 core

			layout (location = 0) out vec4 color;
	  
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			})";

		Acrylic::TSharedPtr<Acrylic::IShader> ExampleShader = Acrylic::TSharedPtr<Acrylic::IShader>(Acrylic::IShader::Create(VertexSrc, FragmentSrc));

		ShaderVertexArrayPairs.push_back({ ExampleShader, ExampleVertexArray });
	}

	virtual void OnUpdate(Acrylic::Timestep ts) override
	{
		AC_LOG(LogSandbox, Log, "Delta time: {0}s ({1}ms)", ts, ts.GetMilliseconds());

		if (Acrylic::IInput::IsKeyPressed(AC_KEY_W))
		{
			CameraPosition.y += 5.f * ts;
		}
		else if (Acrylic::IInput::IsKeyPressed(AC_KEY_S))
		{
			CameraPosition.y -= 5.f * ts;
		}

		if (Acrylic::IInput::IsKeyPressed(AC_KEY_A))
		{
			CameraPosition.x -= 5.f * ts;
		}
		else if (Acrylic::IInput::IsKeyPressed(AC_KEY_D))
		{
			CameraPosition.x += 5.f * ts;
		}

		if (Acrylic::IInput::IsKeyPressed(AC_KEY_Q))
		{
			CameraRotation += 180 * ts;
		}
		else if (Acrylic::IInput::IsKeyPressed(AC_KEY_E))
		{
			CameraRotation -= 180 * ts;
		}

		Acrylic::CommandList::SetClearColour({ 1.f, 0.1f, 0.1f, 1.f });
		Acrylic::CommandList::Clear();

		Camera->SetPosition(CameraPosition);
		Camera->SetRotation(CameraRotation);

		Acrylic::Renderer::BeginScene(Camera);

		for (const auto& ShaderVertexArrayPair : ShaderVertexArrayPairs)
		{
			Acrylic::Renderer::Submit(ShaderVertexArrayPair.first, ShaderVertexArrayPair.second);
		}

		Acrylic::Renderer::EndScene();
	}

	virtual void OnEvent(Acrylic::Event& e) override
	{
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

	}

private:
	Acrylic::TArray<std::pair<Acrylic::TSharedPtr<Acrylic::IShader>, Acrylic::TSharedPtr<Acrylic::IVertexArray>>> ShaderVertexArrayPairs;

	Acrylic::TSharedPtr<Acrylic::OrthographicCamera> Camera;
	glm::vec3									 CameraPosition;
	float										 CameraRotation;
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