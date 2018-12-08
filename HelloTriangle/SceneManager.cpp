#include "SceneManager.h"

#include <queue>

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;
static queue<int> filaMovimentos;
static bool inicializacao;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	inicializacao = true;
	
	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();
}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Hello Transform", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Build and compile our shader program
	loadShaders();

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez
	
	setupCamera2D();
}

void SceneManager::loadShaders()
{
	string vFilename = "../shaders/transformations.vs";
	string fFilename = "../shaders/transformations.frag";
	shader = new Shader(vFilename.c_str(), fFilename.c_str());
}

void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) {
			keys[key] = true;
			//cout << endl << "pressionou tecla : " << key;
			//if (key >= 262 && key <= 265) {
			if ((key >= 321 && key <= 324) || (key >= 326 && key <= 329)) {
				//adiciona em uma fila para ser consumida ao renderizar
				filaMovimentos.push(key); 
			}
		}
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

glm::mat4 SceneManager::atualizarPosicaoPersonagem()
{
	//consome a fila de movimentos para atualizar a posicao do personagem
	glm::mat4 newModelSprite = sprite.getModelSprite();
	int heightTile = tileset[0].getHeight();
	int widthTile = tileset[0].getWidth();

	while (!filaMovimentos.empty()) {
		switch (filaMovimentos.front())
		{
			/*
			case 262:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(widthTile / 2, heightTile / 2, 0.0));
				break;
			case 263:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(-widthTile / 2, -heightTile / 2, 0.0));
				break;
			case 264:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(-widthTile / 2, heightTile / 2, 0.0));
				break;
			case 265:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(widthTile / 2, -heightTile / 2, 0.0));
				break;
			*/
			/*
			case 262:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(widthTile / 2, -heightTile / 2, 0.0));
				break;
			case 263:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(-widthTile / 2, heightTile / 2, 0.0));
				break;
			case 264:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(widthTile / 2, heightTile / 2, 0.0));
				break;
			case 265:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(-widthTile / 2, -heightTile / 2, 0.0));
				break;
			*/
			/*
			case 262:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(widthTile, 0.0, 0.0));
				break;
			case 263:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(-widthTile, 0, 0.0));
				break;
			case 264:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(0.0, heightTile, 0.0));
				break;
			case 265:
				newModelSprite = glm::translate(newModelSprite, glm::vec3(0.0, -heightTile, 0.0));
				break;
			*/	
			case 326: //L
				newModelSprite = glm::translate(newModelSprite, glm::vec3(widthTile, 0.0, 0.0));
				break;
			case 324: //O
				newModelSprite = glm::translate(newModelSprite, glm::vec3(-widthTile, 0, 0.0));
				break;
			case 322: //S
				newModelSprite = glm::translate(newModelSprite, glm::vec3(0.0, heightTile, 0.0));
				break;
			case 328: //N
				newModelSprite = glm::translate(newModelSprite, glm::vec3(0.0, -heightTile, 0.0));
				break;
			case 329: //NE
				newModelSprite = glm::translate(newModelSprite, glm::vec3(widthTile / 2, -heightTile / 2, 0.0));
				break;
			case 321: //SO
				newModelSprite = glm::translate(newModelSprite, glm::vec3(-widthTile / 2, heightTile / 2, 0.0));
				break;
			case 323: //SE
				newModelSprite = glm::translate(newModelSprite, glm::vec3(widthTile / 2, heightTile / 2, 0.0));
				break;
			case 327: //NO
				newModelSprite = glm::translate(newModelSprite, glm::vec3(-widthTile / 2, -heightTile / 2, 0.0));
				break;
			default:
				break;
		}
		filaMovimentos.pop();
	}
	return newModelSprite;
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}


void SceneManager::do_movement()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene
	shader->Use();

	
	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	glm::mat4 modelIniSprite;
	//guardamos temporariamente os tiles desenhados nesta 
	//variavel para testarmos colisao ao desenhar o sprite
	//glm::mat4 tilesDesenhados[8][8];
	vector <glm::mat4> tilesDesenhados;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			TileIso tile = tileset[map[i][j]];
			int xi = width/2 - tile.getWidth()/2; //desloca no eixo X 
			int px = xi + (i-j) * (tile.getWidth()/2);
			int py = (i+j) * (tile.getHeight()/2);

			glm::mat4 model = glm::mat4();
			model = glm::translate(model, glm::vec3(px, py, 0.0));  //posX e posY da tela
			if (i == 3 && j == 3)
				modelIniSprite = model;
			if(tile.getCaminhavel())
				tilesDesenhados.push_back(model);
			tile.draw(model);
		}
	}
	//desenha o sprite
	if (inicializacao)
	{
		inicializacao = false;
		sprite.draw(modelIniSprite);
	}
	else 
	{
		glm::mat4 newPosition = sprite.getModelSprite();
		if (filaMovimentos.empty())
		{
			sprite.draw(newPosition);
		} else 
		{
			newPosition = atualizarPosicaoPersonagem();
			for (int i = 0; i < tilesDesenhados.size(); i++)
			{
				if (tilesDesenhados[i] == newPosition)
				{
					cout << "achou tile na mesma posicao" << endl;
					sprite.draw(newPosition);
					break;
				}
			}
		}
	}
}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		do_movement();

		//Render scene
		render();
		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	//Carrega texturas
	setupTexture();
	
	//Carrega Tileset
	TileIso tile;
	tile.setShader(shader);
	tile.setTexture(texture);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 12; j++) {
			vector<glm::vec2> coord1 = getCoordTextureTile(i, j);
			tile.setTextureCoord(coord1);
			tile.inicializar(64, 128);
			tile.setCaminhavel(ehCaminhavel(i, j));
			tileset.push_back(tile);
		}
	}
	//carrega map
	loadTilemapFile();

	//cria e inicializa sprite do personagem
	sprite.setShader(shader);
	sprite.setTexture(textureSprite);
	sprite.inicializar(64, 128);
}

bool SceneManager::ehCaminhavel(int col, int ln)
{
	//as imagens com rocha ou tronco de arvore estao sendo marcadas como NAO caminhavel
	return ((col >= 4 && col <= 7) && (ln == 2 || ln == 5 || ln == 8 || ln == 11)) ? false : true;
}

vector<glm::vec2> SceneManager::getCoordTextureTile(unsigned int tileColumn, unsigned int tileLine)
{
	float desloc_x = 1 / (numTilesTexture.x * 2);
	float desloc_y = 1 / (numTilesTexture.y * 2);
	
	float px = (1 / (numTilesTexture.x * 2)) + (tileColumn * (1 / numTilesTexture.x));
	float py = (1 / (numTilesTexture.y * 2)) + (tileLine * (1 / numTilesTexture.y));

	glm::vec2 p1 = glm::vec2(px, py - desloc_y);
	glm::vec2 p2 = glm::vec2(px - desloc_x, py);
	glm::vec2 p3 = glm::vec2(px, py + desloc_y);
	glm::vec2 p4 = glm::vec2(px + desloc_x, py);

	vector<glm::vec2> retorno;
	retorno.push_back(p1);
	retorno.push_back(p2);
	retorno.push_back(p3);
	retorno.push_back(p4);
	return retorno;
}

void SceneManager::setupCamera2D()
{
	glViewport(0, 0, width, height);

	//corrigindo o aspecto
	float ratio;
	float xMin = 0.0, xMax = 800, yMin = 600, yMax = 0.0, zNear = -1.0, zFar = 1.0;
	if (width >= height)
	{
		ratio = 1;//width / (float)height;
		projection = glm::ortho(xMin*ratio, xMax*ratio, yMin, yMax, zNear, zFar);
	}
	else
	{
		ratio = height / (float)width;
		projection = glm::ortho(xMin, xMax, yMin*ratio, yMax*ratio, zNear, zFar);
	}

	// Get their uniform location
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void SceneManager::setupTexture()
{
	//**********************************/
	/*    CONFIGURA TEXTURA DO TILE    */
	/***********************************/
	// load and create a texture 
	// -------------------------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int t_width, t_height, nrChannels;
	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char *data = stbi_load("../textures/tileset2.png", &t_width, &t_height, &nrChannels, 0);
	
	cout << "Nro de canais: " << nrChannels << endl;
	cout << "largura x altura: " << t_width << " x " << t_height << endl;

	/*Configura o numero de tiles existentes na textura*/
	numTilesTexture.x = 8.0f;
	numTilesTexture.y = 12.0f;

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//**********************************/
	/*    CONFIGURA TEXTURA DO SPRITE  */
	/***********************************/
	glGenTextures(1, &textureSprite);
	glBindTexture(GL_TEXTURE_2D, textureSprite); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char *data2 = stbi_load("../textures/mario4.png", &t_width, &t_height, &nrChannels, 0);

	cout << "Nro de canais: " << nrChannels << endl;
	cout << "largura x altura: " << t_width << " x " << t_height << endl;

	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data2);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void SceneManager::loadTilemapFile()
{
	ifstream arqEntrada;
	arqEntrada.open("../textures/mapa.txt");
	if (!arqEntrada)
	{
		cout << "Arquivo nao encontrado!" << endl;
	}
	else
	{
		int linha = 0;
		while (!arqEntrada.eof())
		{
			int t1, t2, t3, t4, t5, t6, t7, t8;
			arqEntrada >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8;
			map[0][linha] = t1;
			map[1][linha] = t2;
			map[2][linha] = t3;
			map[3][linha] = t4;
			map[4][linha] = t5;
			map[5][linha] = t6;
			map[6][linha] = t7;
			map[7][linha] = t8;
			//cout << "leu linha " << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8;
			linha++;
		}
		arqEntrada.close();
	}
}