#include "sprite_renderer.h"
#include <cmath>
#include <iostream>

SpriteRenderer::SpriteRenderer(Shader& shader)
{
	this->shader = shader;
	this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::initRenderData()
{
	// configure VAO/VBO
	float vertices[] = {
		// pos    // tex
		0.0, 1.0, 0.0, 1.0f,
		1.0, 0.0, 1.0, 0.0f,
		0.0, 0.0, 0.0, 0.0f,

		0.0, 1.0, 0.0, 1.0f,
		1.0, 1.0, 1.0, 1.0f,
		1.0, 0.0, 1.0, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &this->quadVBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position,
	glm::vec2 size, float rotate, glm::vec3 color)
{
	// prepare transformations
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.SetMatrix4("model", model);
	this->shader.SetVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position, 
	SpriteAnimation animation, double time, int animStartTime,
	glm::vec2 size, float rotate, glm::vec3 color) 
{
	/*unsigned int startIndex;
	unsigned int tileCount;
	unsigned int playbackSpeed;
	unsigned int sheetWidth, sheetHeight;
	unsigned int tileWidth, tileHeight;*/
	int timePerTile = animation.playbackSpeed / animation.tileCount;
	int tileNumber;
	if (animation.repeats) {
		tileNumber = (int(time * 1000) % animation.playbackSpeed) / timePerTile;
	}
	else { // start from the beggining
		tileNumber = (int(time * 1000) - animStartTime) / timePerTile;
		if (tileNumber >= animation.tileCount) {
			tileNumber = animation.tileCount - 1;
		}
	}

	int tileIndex = animation.startIndex + tileNumber;

	float tx, ty, tw, th;

	tw = animation.tileWidth / (animation.sheetWidth * animation.tileWidth);
	th = animation.tileHeight / (animation.sheetHeight * animation.tileHeight);
	tx = (tileIndex % int(animation.sheetWidth)) * tw;
	ty = std::floor(tileIndex / int(animation.sheetWidth)) * th;

	float vertices[] = {
		// pos    // tex
		0.0, 1.0, tx,      ty + th,
		1.0, 0.0, tx + tw, ty,
		0.0, 0.0, tx,      ty,

		0.0, 1.0, tx,      ty + th,
		1.0, 1.0, tx + tw, ty + th,
		1.0, 0.0, tx + tw, ty
	};

	glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	DrawSprite(texture, position, size, rotate, color);

	float regularVertices[] = {
		// pos    // tex
		0.0, 1.0, 0.0, 1.0f,
		1.0, 0.0, 1.0, 0.0f,
		0.0, 0.0, 0.0, 0.0f,

		0.0, 1.0, 0.0, 1.0f,
		1.0, 1.0, 1.0, 1.0f,
		1.0, 0.0, 1.0, 0.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(regularVertices), regularVertices, GL_DYNAMIC_DRAW);

}