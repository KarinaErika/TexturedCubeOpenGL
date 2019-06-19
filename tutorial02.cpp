#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
// Include das funções de transformação das matrizes
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>

int main(void)
{
	// Inicializa GLFW
	if (!glfwInit()){
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Abre uma janela e cria seu contexto OpenGL
	window = glfwCreateWindow(1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Inicializa GLEW
	glewExperimental = true; // Necessário para core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Garante que podemos capturar a tecla esc sendo pressionada abaixo
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Deixa o pano de fundo azul escuro
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);



	// Ativar teste de profundidade (o novo fragmento está mais próximo do que o anterior?)
	glEnable(GL_DEPTH_TEST);
	// Aceitar fragmento se estiver mais perto da câmera do que o anterior
	glDepthFunc(GL_LESS);



	//Cria um objeto Vertex Array (VAO) e o define como o atual
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Cria e compila nosso programa GLSL a partir dos shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");


	//Obtentendo um handle para o "MVP" uniforme
	//Somente durante a inicialização
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	
	////Matriz de projeção: Campo de visão de 45 °, relação 4: 3, faixa de exibição: 0,1 unidade <-> 100 unidades
	//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	

	
	//Ou, para uma câmera ortogonal:
	//glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); //em coordenadas mundiais


	////Tentando fazer a obliqua
	//glm::mat4 oblique = glm::mat4(1.0f);

	//oblique[0][2] = -1 * (glm::cos(3.14 / 4));

	//glm::mat4 Projection = oblique * ortho(-3.0f, 3.0f, -3.0f, 3.0f, -0.1f, 100.0f);

	//Fim tentando fazer a obliqua


	
		////Tentando fazer a obliqua
		//glm::mat4 oblique = glm::mat4(1.0f);

		//oblique[0][2] = -1 * (glm::cos(3.14 / 4));

		//glm::mat4 Projection = oblique * ortho(-3.0f, 3.0f, -3.0f, 3.0f, -0.1f, 100.0f);

		//// Camera matrix
		//glm::mat4 View = glm::lookAt(
		//	glm::vec3(10, 0, 3), // A câmera está em (4,3,3), no World Space
		//	glm::vec3(0, 0, 0), // e olha a origem
		//	glm::vec3(0, 1, 0)  // Head is up (definido como 0, -1,0 para ver de cabeça para baixo)
		//);



		//// Matriz do modelo: uma matriz de identidade (o modelo estará na origem)
		//glm::mat4 Model = glm::mat4(1.0f);

		////Nosso ModelViewProjection: multiplicação de nossas 3 matrizes
		//glm::mat4 mvp = Projection * View * Model; //A multiplicação de matrizes é o contrário
	////Fim Tentando fazer a obliqua

	 
		//Matriz de projeção: Campo de visão de 45 °, relação 4: 3, faixa de exibição: 0,1 unidade <-> 100 unidades
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(4, 3, 3), // A câmera está em (4,3,3), no World Space
			glm::vec3(0, 0, 0), // e olha a origem
			glm::vec3(0, 1, 0)  // Head is up (definido como 0, -1,0 para ver de cabeça para baixo)
		);

		

	

	// Matriz do modelo: uma matriz de identidade (o modelo estará na origem)
	glm::mat4 Model = glm::mat4(1.0f);

	//Nosso ModelViewProjection: multiplicação de nossas 3 matrizes
	glm::mat4 mvp = Projection * View * Model; //A multiplicação de matrizes é o contrário
	

	//// Camera matrix
	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(4, 3, 3), // A câmera está em (4,3,3), no World Space
	//	glm::vec3(0, 0, 0), // e olha a origem
	//	glm::vec3(0, 1, 0)  // Head is up (definido como 0, -1,0 para ver de cabeça para baixo)
	//);
	// Camera matrix pra obliqua
	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(10, 0, 3), // A câmera está em (4,3,3), no World Space
	//	glm::vec3(0, 0, 0), // e olha a origem
	//	glm::vec3(0, 1, 0)  // Head is up (definido como 0, -1,0 para ver de cabeça para baixo)
	//);

	//DEscomentar
	//// Matriz do modelo: uma matriz de identidade (o modelo estará na origem)
	//glm::mat4 Model = glm::mat4(1.0f);

	////Nosso ModelViewProjection: multiplicação de nossas 3 matrizes
	//glm::mat4 mvp = Projection * View * Model; //A multiplicação de matrizes é o contrário
	//Descomentar

	//Carrega a textura usando dois métodos
	//GLuint Texture = loadBMP_custom("C:/ogl-master  build/tutorial02_red_triangle.dir/Debug/uvtemplate.bmp");
	GLuint Texture = loadDDS("C:/ogl-master  build/tutorial02_red_triangle.dir/Debug/uvtemplate.DDS");

	// Get a handle for our "myTextureSampler" uniform
	// Obtenha um identificador para o  uniforme "myTextureSampler"
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	//// Uma matriz de 3 vetores que representa 3 vértices (Para triângulo) 
	//static const GLfloat g_vertex_buffer_data[] = {
	//	-1.0f, -1.0f, 0.0f,
	//	 1.0f, -1.0f, 0.0f,
	//	 0.0f,  1.0f, 0.0f,
	//};

	// nossos vértices. Três flutuadores consecutivos fornecem um vértice 3D; Três vértices consecutivos dão um triângulo.
	// Um cubo tem 6 faces com 2 triângulos cada, então isso faz 6 * 2 = 12 triângulos e 12 * 3 vértices
	static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
	};


	//// Uma cor para cada vértice. Elas foram gerados aleatoriamente.
	//static const GLfloat g_color_buffer_data[] = {
	//	0.583f,  0.771f,  0.014f,
	//	0.609f,  0.115f,  0.436f,
	//	0.327f,  0.483f,  0.844f,
	//	0.822f,  0.569f,  0.201f,
	//	0.435f,  0.602f,  0.223f,
	//	0.310f,  0.747f,  0.185f,
	//	0.597f,  0.770f,  0.761f,
	//	0.559f,  0.436f,  0.730f,
	//	0.359f,  0.583f,  0.152f,
	//	0.483f,  0.596f,  0.789f,
	//	0.559f,  0.861f,  0.639f,
	//	0.195f,  0.548f,  0.859f,
	//	0.014f,  0.184f,  0.576f,
	//	0.771f,  0.328f,  0.970f,
	//	0.406f,  0.615f,  0.116f,
	//	0.676f,  0.977f,  0.133f,
	//	0.971f,  0.572f,  0.833f,
	//	0.140f,  0.616f,  0.489f,
	//	0.997f,  0.513f,  0.064f,
	//	0.945f,  0.719f,  0.592f,
	//	0.543f,  0.021f,  0.978f,
	//	0.279f,  0.317f,  0.505f,
	//	0.167f,  0.620f,  0.077f,
	//	0.347f,  0.857f,  0.137f,
	//	0.055f,  0.953f,  0.042f,
	//	0.714f,  0.505f,  0.345f,
	//	0.783f,  0.290f,  0.734f,
	//	0.722f,  0.645f,  0.174f,
	//	0.302f,  0.455f,  0.848f,
	//	0.225f,  0.587f,  0.040f,
	//	0.517f,  0.713f,  0.338f,
	//	0.053f,  0.959f,  0.120f,
	//	0.393f,  0.621f,  0.362f,
	//	0.673f,  0.211f,  0.457f,
	//	0.820f,  0.883f,  0.371f,
	//	0.982f,  0.099f,  0.879f
	//};


	//// Duas coordenadas UV para cada vértice. Eles foram criados com o Blender.
	//static const GLfloat g_uv_buffer_data[] = {
	//	0.583f,  0.771f,  0.014f,
	//	0.609f,  0.115f,  0.436f,
	//	0.327f,  0.483f,  0.844f,
	//	0.822f,  0.569f,  0.201f,
	//	0.435f,  0.602f,  0.223f,
	//	0.310f,  0.747f,  0.185f,
	//	0.597f,  0.770f,  0.761f,
	//	0.559f,  0.436f,  0.730f,
	//	0.359f,  0.583f,  0.152f,
	//	0.483f,  0.596f,  0.789f,
	//	0.559f,  0.861f,  0.639f,
	//	0.195f,  0.548f,  0.859f,
	//	0.014f,  0.184f,  0.576f,
	//	0.771f,  0.328f,  0.970f,
	//	0.406f,  0.615f,  0.116f,
	//	0.676f,  0.977f,  0.133f,
	//	0.971f,  0.572f,  0.833f,
	//	0.140f,  0.616f,  0.489f,
	//	0.997f,  0.513f,  0.064f,
	//	0.945f,  0.719f,  0.592f,
	//	0.543f,  0.021f,  0.978f,
	//	0.279f,  0.317f,  0.505f,
	//	0.167f,  0.620f,  0.077f,
	//	0.347f,  0.857f,  0.137f,
	//	0.055f,  0.953f,  0.042f,
	//	0.714f,  0.505f,  0.345f,
	//	0.783f,  0.290f,  0.734f,
	//	0.722f,  0.645f,  0.174f,
	//	0.302f,  0.455f,  0.848f,
	//	0.225f,  0.587f,  0.040f,
	//	0.517f,  0.713f,  0.338f,
	//	0.053f,  0.959f,  0.120f,
	//	0.393f,  0.621f,  0.362f,
	//	0.673f,  0.211f,  0.457f,
	//	0.820f,  0.883f,  0.371f,
	//	0.982f,  0.099f,  0.879f
	//};	
	
	//Duas coordenadas UV para cada vértice. Eles foram criados com o Blender.
	static const GLfloat g_uv_buffer_data[] = {
		0.000059f, 1.0f - 0.000004f,
		0.000103f, 1.0f - 0.336048f,
		0.335973f, 1.0f - 0.335903f,
		1.000023f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.336024f, 1.0f - 0.671877f,
		0.667969f, 1.0f - 0.671889f,
		1.000023f, 1.0f - 0.000013f,
		0.668104f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.000059f, 1.0f - 0.000004f,
		0.335973f, 1.0f - 0.335903f,
		0.336098f, 1.0f - 0.000071f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.336024f, 1.0f - 0.671877f,
		1.000004f, 1.0f - 0.671847f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.668104f, 1.0f - 0.000013f,
		0.335973f, 1.0f - 0.335903f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.668104f, 1.0f - 0.000013f,
		0.336098f, 1.0f - 0.000071f,
		0.000103f, 1.0f - 0.336048f,
		0.000004f, 1.0f - 0.671870f,
		0.336024f, 1.0f - 0.671877f,
		0.000103f, 1.0f - 0.336048f,
		0.336024f, 1.0f - 0.671877f,
		0.335973f, 1.0f - 0.335903f,
		0.667969f, 1.0f - 0.671889f,
		1.000004f, 1.0f - 0.671847f,
		0.667979f, 1.0f - 0.335851f
	};


	//Dar este triângulo (ou cubo) ao OpenGL, fazemos isso criando um buffer: 

	// Para o triângulo
	GLuint vertexbuffer; //Isso irá identificar nosso buffer de vértices
	glGenBuffers(1, &vertexbuffer); // Gera 1 buffer, coloca o identificador resultante no vertexbuffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); //// fala sobre nosso buffer 'vertexbuffer'
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); //// Dá nossos vértices para o OpenGL.

	/*GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);*/

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	do {

		// Limpa a tela, o que deixará com o fundo azul escuro por causa da definição anterior
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Informando ao opengl para usar o shader
		glUseProgram(programID);

		// Envia nossa transformação para o shader atualmente vinculado, no uniforme "MVP"
		// Isso é feito no loop principal, já que cada modelo terá uma matriz MVP diferente (pelo menos para a parte M)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		// 1º buffer de atributos: vértices (Para triângulo)
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // atributo 0. Nenhum motivo específico para 0, mas deve corresponder ao layout no shader.
			3,                  // tamanho
			GL_FLOAT,           // tipo
			GL_FALSE,           // normalizado?
			0,                  // stride (passo largo)
			(void*)0            // deslocamento de buffer de matriz
		);

		//// 2° buffer de atributos: cores
		//glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		//glVertexAttribPointer(
		//	1,                                // atributo. Nenhuma razão específica para 1, mas deve corresponder ao layout no shader.
		//	3,                                // tamanho
		//	GL_FLOAT,                         // tipo
		//	GL_FALSE,                         // normalizado?
		//	0,                                // stride (passo largo)
		//	(void*)0                          // deslocamento de buffer de matriz

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // atributo. Nenhuma razão específica para 1, mas deve corresponder ao layout no shader.
			2,                                // tamanho : U+V => 2
			GL_FLOAT,                         // tipo
			GL_FALSE,                         // normalizado?
			0,                                // stride (passo largo)
			(void*)0                          // deslocamento de buffer de matriz

		);

		// Desenha o triângulo
		//glDrawArrays(GL_TRIANGLES, 0, 3); //3 índices começando em 0, o que resutará em um triângulo

		//Desenha um cubo
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12 * 3 índices começando em 0 -> 12 triângulos -> 6 quadrados

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Verifica se a tecla ESC foi pressionada ou se a janela foi fechada
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	//Limpa VBO e shader
	glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &colorbuffer); 
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
	

	// Fecha a janela do OpenGL e termina o GLFW
	glfwTerminate();

	return 0;
}

/*
cubo:
projeções: ortogonal, obliqua, pespectiva
câmera: 
Textura: 
*/

