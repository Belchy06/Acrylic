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
		ShaderLibrary = Acrylic::MakeUnique<Acrylic::ShaderLibrary>();
		CameraController = Acrylic::MakeUnique<Acrylic::OrthographicCameraController>(1280.f / 720.f, true);

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

		Acrylic::TSharedPtr<Acrylic::IShader> ExampleShader = ShaderLibrary->Load("assets/shaders/Texture.glsl");

		ShaderVertexArrayPairs.push_back({ ExampleShader, ExampleVertexArray });

		Texture = Acrylic::Texture2D::Create("assets/textures/Test.png");

		ExampleShader->Bind();
		ExampleShader->UploadUniformInt("u_Texture", 0);
	}

	virtual void OnUpdate(Acrylic::Timestep ts) override
	{
		AC_LOG(LogSandbox, Log, "Delta time: {0}s ({1}ms)", ts, ts.GetMilliseconds());
		
		// Update
		CameraController->OnUpdate(ts);

		// Render
		Acrylic::GCommandListExecutor->SetClearColour({ 1.f, 0.1f, 0.1f, 1.f });
		Acrylic::GCommandListExecutor->Clear();

		Acrylic::Renderer::BeginScene(CameraController->GetCamera());

		Texture->Bind(0);

		for (const auto& ShaderVertexArrayPair : ShaderVertexArrayPairs)
		{
			Acrylic::Renderer::Submit(ShaderVertexArrayPair.first, ShaderVertexArrayPair.second);
		}

		Acrylic::Renderer::EndScene();
	}

	virtual void OnEvent(Acrylic::Event& e) override
	{
		CameraController->OnEvent(e);
	}

private:
	Acrylic::TUniquePtr<Acrylic::ShaderLibrary>																		   ShaderLibrary;
	Acrylic::TArray<Acrylic::TPair<Acrylic::TSharedPtr<Acrylic::IShader>, Acrylic::TSharedPtr<Acrylic::IVertexArray>>> ShaderVertexArrayPairs;

	Acrylic::TSharedPtr<Acrylic::Texture2D> Texture;

	Acrylic::TUniquePtr<Acrylic::OrthographicCameraController> CameraController;
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