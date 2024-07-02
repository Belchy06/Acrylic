#include "acpch.h"

#include "Application.h"
#include "Events/ApplicationEvent.h"

#include "Renderer/Renderer.h"

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogApplication);

	Application* Application::Singleton = nullptr;

	Application::Application()
	{
		Singleton = this;
		Window = std::unique_ptr<IWindow>(IWindow::Create());
		Window->SetEventCallback(AC_BIND_EVENT_FN(Application::OnEvent));

		Stack.PushOverlay(new ImGuiLayer());

		// clang-format off
		float Vertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,   0.75f, 0.0f,
			-0.75f,   0.75f, 0.0f
		};
		// clang-format on

		std::shared_ptr<IVertexBuffer> ExampleVertexBuffer = std::shared_ptr<IVertexBuffer>(IVertexBuffer::Create(Vertices, sizeof(Vertices) / sizeof(float)));
		ExampleVertexBuffer->SetLayout({ { EDataType::Float3, "a_Position" } });

		// clang-format off
		uint32_t Indices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		// clang-format on
		std::shared_ptr<IIndexBuffer> ExampleIndexBuffer = std::shared_ptr<IIndexBuffer>(IIndexBuffer::Create(Indices, sizeof(Indices) / sizeof(uint32_t)));

		std::shared_ptr<IVertexArray> ExampleVertexArray = std::shared_ptr<IVertexArray>(IVertexArray::Create());
		ExampleVertexArray->AddVertexBuffer(ExampleVertexBuffer);
		ExampleVertexArray->SetIndexBuffer(ExampleIndexBuffer);

		std::string VertexSrc = R"(
			#version 330 core

			layout (location = 0) in vec3 a_Position;
  
			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			})";

		std::string FragmentSrc = R"(
			#version 330 core

			layout (location = 0) out vec4 color;
	  
			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			})";

		std::shared_ptr<Shader> ExampleShader = std::shared_ptr<Shader>(new Shader(VertexSrc, FragmentSrc));

		Shaders.push_back({ ExampleShader, ExampleVertexArray });
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<WindowCloseEvent>(AC_BIND_EVENT_FN(Application::OnWindowClosed));

		AC_LOG(LogApplication, VeryVerbose, "{0}", e.ToString());

		for (std::vector<Layer*>::iterator It = Stack.end(); It != Stack.begin();)
		{
			(*--It)->OnEvent(e);
			if (e.IsHandled())
			{
				break;
			}
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		bRunning = false;
		return true;
	}

	void Application::PushLayer(Layer* InLayer)
	{
		Stack.PushLayer(InLayer);
	}

	void Application::PushOverlay(Layer* InOverlay)
	{
		Stack.PushOverlay(InOverlay);
	}

	void Application::Run()
	{
		while (bRunning)
		{
			CommandList::SetClearColour({ 1.f, 0.1f, 0.1f, 1.f });
			CommandList::Clear();

			Renderer::BeginScene();

			for (const auto& pair : Shaders)
			{
				pair.first->Bind();
				Renderer::Submit(pair.second);
			}

			Renderer::EndScene();

			for (Layer* l : Stack)
			{
				l->OnUpdate();
			}

			GUILayer->Begin();
			for (Layer* l : Stack)
			{
				l->OnImGuiRender();
			}
			GUILayer->End();

			Window->OnUpdate();
		}
	}
} // namespace Acrylic