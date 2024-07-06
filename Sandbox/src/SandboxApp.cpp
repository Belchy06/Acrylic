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
		Camera = Acrylic::MakeShared<Acrylic::OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f);
		CameraPosition = Camera->GetPosition();
		CameraRotation = Camera->GetRotation();

		// clang-format off
		float Vertices[4 * 5] = {
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f, 
			 0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f,
		};
		// clang-format on

		Acrylic::TSharedPtr<Acrylic::IVertexBuffer> ExampleVertexBuffer = Acrylic::IVertexBuffer::Create(Vertices, sizeof(Vertices) / sizeof(float));
		// clang-format off
		ExampleVertexBuffer->SetLayout({ 
			{ Acrylic::EDataType::Float3, "a_Position" },
			{ Acrylic::EDataType::Float2, "a_TexCoord" } 
		});
		// clang-format on

		// clang-format off
		uint32_t Indices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		// clang-format on
		Acrylic::TSharedPtr<Acrylic::IIndexBuffer> ExampleIndexBuffer = Acrylic::IIndexBuffer::Create(Indices, sizeof(Indices) / sizeof(uint32_t));

		Acrylic::TSharedPtr<Acrylic::IVertexArray> ExampleVertexArray = Acrylic::IVertexArray::Create();
		ExampleVertexArray->AddVertexBuffer(ExampleVertexBuffer);
		ExampleVertexArray->SetIndexBuffer(ExampleIndexBuffer);

		Acrylic::TSharedPtr<Acrylic::IShader> ExampleShader = Acrylic::IShader::Create("assets/shaders/Texture.glsl");

		ShaderVertexArrayPairs.push_back({ ExampleShader, ExampleVertexArray });

		Texture = Acrylic::Texture2D::Create("assets/textures/Test.png");

		ExampleShader->Bind();
		ExampleShader->UploadUniformInt("u_Texture", 0);
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

		Acrylic::GCommandListExecutor->SetClearColour({ 1.f, 0.1f, 0.1f, 1.f });
		Acrylic::GCommandListExecutor->Clear();

		Camera->SetPosition(CameraPosition);
		Camera->SetRotation(CameraRotation);

		Acrylic::Renderer::BeginScene(Camera);

		Texture->Bind(0);

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
	Acrylic::TArray<std::pair<Acrylic::TSharedPtr<Acrylic::IShader>, Acrylic::TSharedPtr<Acrylic::IVertexArray>>> ShaderVertexArrayPairs;

	Acrylic::TSharedPtr<Acrylic::Texture2D> Texture;

	Acrylic::TSharedPtr<Acrylic::OrthographicCamera> Camera;
	glm::vec3										 CameraPosition;
	float											 CameraRotation;
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