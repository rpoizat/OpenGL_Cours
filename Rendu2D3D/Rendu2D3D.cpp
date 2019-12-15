//
//
//

// GLEW_STATIC force le linkage statique
// c-a-d que le code de glew est directement injecte dans l'executable
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "mat.h"
#include <../../common/GLShader.h>

// les repertoires d'includes sont:
// ../libs/glfw-3.3/include			fenetrage
// ../libs/glew-2.1.0/include		extensions OpenGL
// ../libs/stb						gestion des images (entre autre)

// les repertoires des libs sont (en 64-bit):
// ../libs/glfw-3.3/lib-vc2015
// ../libs/glew-2.1.0/lib/Release/x64

// Pensez a copier les dll dans le repertoire x64/Debug, cad:
// glfw-3.3/lib-vc2015/glfw3.dll
// glew-2.1.0/bin/Release/x64/glew32.dll		si pas GLEW_STATIC

// _WIN32 indique un programme Windows
// _MSC_VER indique la version du compilateur VC++
#if defined(_WIN32) && defined(_MSC_VER)
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32s.lib")			// glew32.lib si pas GLEW_STATIC
#pragma comment(lib, "opengl32.lib")
#elif defined(__APPLE__)
#elif defined(__linux__)
#endif

#include <iostream>

GLShader basic_shader;
GLuint VBO;
GLuint VAO;

struct Vertex 
{
	float x, y;
	float r, g, b;
};

void Initialize()
{
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "erreur d'initialisation de GLEW!"
			<< std::endl;
	}

	std::cout << "Version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Vendor : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer : " << glGetString(GL_RENDERER) << std::endl;

	basic_shader.LoadVertexShader("basic.vs");
	basic_shader.LoadFragmentShader("basic.fs");
	basic_shader.Create();

	

	// Idem mais en OpenGL moderne
	// defini une liste de sommets, 2D, de type float
	const Vertex triangles[] = {
		{-0.5f, -0.5f, 1.0, 0.0, 0.0},
		{-0.5f, +0.5f, 0.0, 1.0, 0.0},
		{+0.5f, +0.5f, 0.0, 0.0, 1.0},
		// second triangle
		{-0.5f, -0.5f, 1.0, 0.0, 0.0},
		{+0.5f, +0.5f, 0.0, 0.0, 1.0},
		{+0.5f, -0.5f, 0.0, 1.0, 0.0}
	};

	glGenBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

	int basicProgram = basic_shader.GetProgram();
	glUseProgram(basicProgram);

	int location = glGetAttribLocation(basicProgram, "a_position");
	int locationColor = glGetAttribLocation(basicProgram, "a_color");

	// Specifie la structure des donnees envoyees au GPU
	glVertexAttribPointer(location, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(locationColor, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, r));
	// indique que les donnees sont sous forme de tableau
	glEnableVertexAttribArray(location);
	glEnableVertexAttribArray(locationColor);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Shutdown()
{
	basic_shader.Destroy();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Display(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	int basicProgram = basic_shader.GetProgram();
	glUseProgram(basicProgram);

	glClearColor(1.f, 1.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Desactive le "scissoring"
	glDisable(GL_SCISSOR_TEST);
	// Defini le viewport en pleine fenetre
	glViewport(0, 0, width, height);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//glEnableVertexAttribArray(0);

	
	// dessine deux triangles
	glBindVertexArray(VAO);

	//matrix
	mat4 mat = mat4();
	mat4 matScale = mat4();

	mat.Rotate(glfwGetTime());

	matScale.Identity();
	matScale.Scale(cos(glfwGetTime()));

	//récupérer l'identifiant du Uniform du shader
	int identifiant_mat = glGetUniformLocation(basicProgram, "u_mat");
	int identifiant_matScale = glGetUniformLocation(basicProgram, "u_matScale");

	glUniformMatrix4fv(identifiant_mat, 1, GL_FALSE, mat.data);
	glUniformMatrix4fv(identifiant_matScale, 1, GL_FALSE, matScale.data);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// toutes nos initialisations vont ici
	Initialize();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		Display(window);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// ne pas oublier de liberer la memoire etc...
	Shutdown();

	glfwTerminate();
	return 0;
}