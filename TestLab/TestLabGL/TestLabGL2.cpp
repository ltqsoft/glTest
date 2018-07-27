


using namespace std;
struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
};


int main()
{
	//$ Create context
	if (glfwInit() == GLFW_FALSE) { system("pause"); return -1; }
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	cout << "GLEW init status: " << (glewInit() == GLEW_OK ? "Success" : "Failure") << endl;
	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			camera.ProcessMouseMovement(xoffset, yoffset);
	});

	int winWidth = 0, winHeight = 0;
	glfwGetFramebufferSize(window, &winWidth, &winHeight);
	std::cout << "Framebuffer size: " << winWidth << " x " << winHeight << std::endl;

	Shader framebufferShader("framebufferv.glsl", "framebufferf.glsl");
	Shader fbDrawShader("fb_screenv.glsl", "fb_screenf.glsl");

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// load textures
	// -------------
	unsigned int cubeTexture = loadTexture("resources/textures/marble.jpg");
	unsigned int floorTexture = loadTexture("resources/textures/metal.png");

	// shader configuration
	// --------------------
	framebufferShader.use();
	framebufferShader.setInt("texture1", 0);
	framebufferShader.setInt("texture2", 1);

	fbDrawShader.use();
	///fbDrawShader.setInt("screenTexture", 1);


	//$ Framebuffer
	QFramebuffer framebuffer(800, 600);
	///framebuffer.bindColorBuffers(1);
	///framebufferShader.setInt("texture2", 1);

	//$ImGui
	///IMGUI_CHECKVERSION();
	///ImGui::CreateContext();
	///ImGuiIO& io = ImGui::GetIO(); (void)io;
	///ImGui_ImplGlfw_InitForOpenGL(window, true);
	///ImGui_ImplOpenGL3_Init("#version 330 core");
	///ImGui::StyleColorsDark();

	///ImVec4 cube1pos = ImVec4(-1.0f, 0.2f, -1.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		///ImGui_ImplOpenGL3_NewFrame();
		///ImGui_ImplGlfw_NewFrame();
		///ImGui::NewFrame();
		///if(showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);
		///glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);


		// render
		// ------
		// bind to framebuffer and draw scene as we normally would to color texture 
		///glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		framebuffer.bind();
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

								 // make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		framebufferShader.use();
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		framebufferShader.setMat4("view", view);
		framebufferShader.setMat4("projection", projection);
		framebufferShader.setFloat("intensity", intensity);

		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, translation);
		model = glm::rotate(model, rotateAngle, glm::vec3(1.0f, 1.0f, 0.0f));
		framebufferShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4();
		//-----------------------------------------------------------
		model = glm::translate(model, glm::vec3(2.0f, 1.2f, 0.0f));
		framebufferShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// floor
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		framebufferShader.setMat4("model", glm::mat4());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		framebuffer.bindDefault();
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
								  // clear all relevant buffers


		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		fbDrawShader.use();
		glBindVertexArray(quadVAO);
		framebuffer.bindColorBuffer(1, 0);	//$ In fragment shader we already bound the screen texture to TEXTURE0.
		glDrawArrays(GL_TRIANGLES, 0, 6);

		{
			/*/
			///ImGui::Begin("Move a cube");                          // Create a window called "Hello, world!" and append into it.

			///ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			///ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
			///ImGui::Checkbox("Another Window", &show_another_window);

			///ImGui::SliderFloat3("Translate", (float*)&translation, 0.0f, 10.0f);
			///ImGui::SliderFloat("Rotate", &rotateAngle, 0.0f, 7.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			///ImGui::ColorEdit3("clear color", (float*)&cube1pos); // Edit 3 floats representing a color

			///if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			///	counter++;
			///ImGui::SameLine();
			///ImGui::Text("counter = %d", counter);
			///ImGui::Text("counter = %d", 31415);

			///ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			///ImGui::End();
			///if (ImGui::IsMouseDragging()) isMouseOnDebugWindow = true;
			/*/
			///double x, y;
			///glfwGetCursorPos(window, &x, &y);
			///std::cout << "Cursor pos: " << x << ", " << y << std::endl;
		}


		//$ Render ImGui
		///ImGui::Render();
		///ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	///ImGui_ImplOpenGL3_Shutdown();
	///ImGui_ImplGlfw_Shutdown();

	///ImGui::DestroyContext();
	glfwDestroyWindow(window);
	system("pause");
	glfwTerminate();
	return 0;
}
