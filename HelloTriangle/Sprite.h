#pragma once

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Sprite
{
public:
	Sprite();
	~Sprite();
	void inicializar(float th, float tw);
	void draw(glm::mat4 model);
	void setShader(Shader *shader) { this->shader = shader; };
	void setTexture(unsigned int idxTexture) { this->texture = idxTexture; }
	void setTextureCoord(vector<glm::vec2> coordenadas) { this->textureCoord = coordenadas; }

	float getWidth() { return tw; }
	float getHeight() { return th; }
	glm::mat4 getModelSprite() { return modelSprite; }
protected:
	GLuint VBO, EBO, VAO; //ids dos buffers (na real só precisaria guardar o VAO)

	Shader *shader;

	//Texture index
	unsigned int texture;
	vector<glm::vec2> textureCoord; //array com 4 coordenadas x,y - p1, p2, p3, p4 - {(x,y), (x,y),(x,y),(x,y)}
	glm::mat4 modelSprite;

	float tw, th;
};

