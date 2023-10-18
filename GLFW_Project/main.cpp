#include "Framework.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_size_callback(GLFWwindow* window, int width, int height);
void show_glfw_error(int error, const char* description);

Vector2 mousePos;
Vector2 mouseVelocity;
float last_delta;
float delta;

int main() {
	mousePos = Vector2(0.0f, 0.0f);
	last_delta = 0.0f;
	delta = 0.0f;

	// glfw init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 버전 4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // .6
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Window creation
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Example", NULL, NULL);
	if (!window) {
		std::cerr << "윈도우 생성 실패" << '\n';
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	
	// CallBacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	// glfw키마 너무 극혐이라 ImGui 쓰기로 함

	{ // ImGui
		// Context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	glfwSwapInterval(1);

	// Info
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "GLEW 초기화 실패 " << glewGetErrorString(err) << '\n';
		glfwTerminate();
		exit(-1);
	}
	std::cout << "Version : " << glGetString(GL_VERSION) << '\n'; // version
	//{ // print extensions
	//	int nr_extensions = 0;
	//	glGetIntegerv(GL_NUM_EXTENSIONS, &nr_extensions);
	//	for (int i = 0; i < nr_extensions; ++i)
	//		std::cout << glGetStringi(GL_EXTENSIONS, i) << '\n';
	//}

	// init
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(FLOAT4_CLEAR_COLOR);

	// MainLoop
	GameManager* game = new GameManager();
	while (!glfwWindowShouldClose(window)) {
		{ // Time
			float current_delta = static_cast<float>(glfwGetTime());
			delta = current_delta - last_delta;
			last_delta = current_delta;
		}

		game->Update();
		game->Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
		{ // Mouse
			ImVec2 mPos = MOUSE_POSITION;
			Vector2 prevMousePos = mousePos;
			mousePos = { mPos.x, mPos.y };
			mouseVelocity = mousePos - prevMousePos;
		}
	}
	SAFE_DELETE(game);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void show_glfw_error(int error, const char* description) {
	std::cerr << "Error: " << description << '\n';
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
void window_size_callback(GLFWwindow* window, int width, int height) {
	std::cout << "Window resized, new window size: " << width << " x " << height << '\n';
	glViewport(0, 0, width, height);
	glfwSwapBuffers(window);
}