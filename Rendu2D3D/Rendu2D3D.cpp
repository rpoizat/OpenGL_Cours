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
#include "tiny_obj_loader.h"

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
using namespace std;

GLShader basic_shader;
GLuint VBO;
GLuint VAO;

GLuint IBOobj;
GLuint VBOobj;
GLuint VAOobj;

struct Vertex
{
	float x, y, z;
	float r, g, b;
};

std::string fileName = "./models/suzanne.obj";
tinyobj::attrib_t attrib;
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

std::string warn;
std::string err;

//chargement des vertices de l'objet dans un tableau
std::vector<Vertex> triangleObj;
std::vector<unsigned int> indicesObj;



bool operator ==(const Vertex& v1, const Vertex& v2)
{
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
	{
		return true;
	}
	else return false;
}

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

	//chargement de l'obj
	bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, fileName.c_str());

	if (!warn.empty()) 
	{
		std::cout << warn << std::endl;
	}

	if (!err.empty()) 
	{
		std::cerr << err << std::endl;
	}

	if (!ok) exit(1);

	//pour chaque vertice du fichier
	for (int i = 0; i < attrib.vertices.size() - 2; i+=3)
	{
		//on crée un nouveau Vertex et on cherche dans la liste s'il n'y est pas déjà
		Vertex v = {attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2], 0.3, 0.3, 0.3};

		unsigned int indice = 0;
		bool trouve = false;

		for each (Vertex ver in triangleObj)
		{
			//si on trouve le vertex dans la liste, on ajoute son indice au tableau des indices
			if (ver == v && !trouve)
			{
				indicesObj.push_back(indice);
				trouve = true;
			}
			indice++;
		}

		//s'il on n'a pas trouvé le vertex dans la liste, on l'ajoute ainsi que son indice
		if (!trouve)
		{
			triangleObj.push_back(v);
			indicesObj.push_back(indice);
		}
	}

	glGenBuffers(1, &VAOobj);
	glBindBuffer(GL_ARRAY_BUFFER, VAOobj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleObj), &triangleObj, GL_STATIC_DRAW);

	glGenBuffers(1, &IBOobj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOobj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesObj.size(), &indicesObj, GL_STATIC_DRAW);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Idem mais en OpenGL moderne
	// defini une liste de sommets, 2D, de type float
	//const Vertex triangles[] = {
	//	{-0.5f, -0.5f, -1, 1.0, 0.0, 0.0},
	//	{+0.5f, +0.5f, -1, 0.0, 0.0, 1.0},
	//	{-0.5f, +0.5f, -1, 0.0, 1.0, 0.0},
	//	
	//	// second triangle
	//	{-0.5f, -0.5f, -1, 1.0, 0.0, 0.0},
	//	{+0.5f, -0.5f, -1, 1.0, 1.0, 1.0},
	//	{+0.5f, +0.5f, -1, 0.0, 0.0, 1.0}
	//};

	/*glGenBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);*/

	int basicProgram = basic_shader.GetProgram();
	glUseProgram(basicProgram);

	int location = glGetAttribLocation(basicProgram, "a_position");
	int locationColor = glGetAttribLocation(basicProgram, "a_color");

	// Specifie la structure des donnees envoyees au GPU
	glVertexAttribPointer(location, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(locationColor, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, r));
	// indique que les donnees sont sous forme de tableau
	glEnableVertexAttribArray(location);
	glEnableVertexAttribArray(locationColor);

	
}


void Shutdown()
{
	basic_shader.Destroy();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAOobj);
	glDeleteBuffers(1, &VBOobj);

	glDeleteVertexArrays(1, &IBOobj);
	glDeleteBuffers(1, &IBOobj);
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

	//matrix
	mat4 mat = mat4();
	mat4 matScale = mat4();
	mat4 matTranslate = mat4();
	mat4 matProjOrtho = mat4();
	mat4 matProjPersp = mat4();

	mat.Rotate(glfwGetTime());

	matScale.Identity();
	matScale.Scale(cos(glfwGetTime()));

	matTranslate.Identity();
	matTranslate.Translate(cos(glfwGetTime()) * sin(glfwGetTime()), cos(glfwGetTime()) * sin(glfwGetTime()), 0);

	matProjOrtho.Ortho(0, 15,  -1, 1, 1, -1);

	matProjPersp.Persp(0, 15, float(width), float(height));

	//récupérer l'identifiant du Uniform du shader
	int identifiant_mat = glGetUniformLocation(basicProgram, "u_matRot");
	int identifiant_matScale = glGetUniformLocation(basicProgram, "u_matScale");
	int identifiant_matProj = glGetUniformLocation(basicProgram, "u_matProjOrtho");
	int identifiant_matTrans = glGetUniformLocation(basicProgram, "u_matTrans");
	int identifiant_matProjPersp = glGetUniformLocation(basicProgram, "u_matProjPersp");

	glUniformMatrix4fv(identifiant_mat, 1, GL_FALSE, mat.data);
	glUniformMatrix4fv(identifiant_matScale, 1, GL_FALSE, matScale.data);
	glUniformMatrix4fv(identifiant_matProj, 1, GL_FALSE, matProjOrtho.data);
	glUniformMatrix4fv(identifiant_matTrans, 1, GL_FALSE, matTranslate.data);
	glUniformMatrix4fv(identifiant_matProjPersp, 1, GL_FALSE, matProjPersp.data);


	// dessine deux triangles
	glBindVertexArray(IBOobj);
	glDrawElements(GL_TRIANGLES, indicesObj.size(), GL_UNSIGNED_INT, NULL);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
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

	glEnable(GL_DEPTH_TEST);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//glEnable(GL_CULL_FACE);

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