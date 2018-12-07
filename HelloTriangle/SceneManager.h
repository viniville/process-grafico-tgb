#pragma once

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "TileIso.h"
#include "Sprite.h"
#include <vector>
#include <fstream>

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void initialize(GLuint width, GLuint height);
	void initializeGraphics();
	void loadShaders();

	//GLFW callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	void do_movement();
	void render();
	void run();
	void finish();
	glm::mat4 atualizarPosicaoPersonagem();

	// scene
	void setupScene();
	void setupCamera2D();
	void setupTexture(); //apenas mostra como criar uma textura
	vector<glm::vec2> getCoordTextureTile(unsigned int tileColumn, unsigned int tileLine);
	bool ehCaminhavel(int col, int ln);
	void loadTilemapFile();
private:
	
	//GFLW window
	GLFWwindow *window;

	//our shader program
	Shader *shader;
	Shader *shaderSprite;

	//scene attributes
	GLuint VAO;

	//Transformations - Model Matrix
	glm::mat4 model;

	//2D Camera - Projection matrix
	glm::mat4 projection;

	//Texture index
	unsigned int texture;
	unsigned int textureSprite;

	glm::vec2 numTilesTexture;

	//Conjunto de tiles
	vector <TileIso> tileset;

	Sprite sprite;

	//Aqui ser� o mapa com os indices dos tiles
	int map[8][8];

};

