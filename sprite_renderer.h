#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

struct SpriteAnimation {
	unsigned int startIndex;
	unsigned int tileCount;
	unsigned int playbackSpeed;
	float sheetWidth, sheetHeight;
	float tileWidth, tileHeight;
	bool repeats;

	SpriteAnimation ()
		: startIndex(0), tileCount(1), playbackSpeed(100000), sheetWidth(10.0f), sheetHeight(10.0f), tileWidth(32.0f), tileHeight(32.0f), repeats(true) {}
	SpriteAnimation(unsigned int startI, unsigned int tileC, unsigned int playbackS, float width, float height, bool repeat)
		: startIndex(startI), tileCount(tileC), playbackSpeed(playbackS), sheetWidth(width), sheetHeight(height), tileWidth(32.0f), tileHeight(32.0f), repeats(repeat) {}
};

class SpriteRenderer
{
public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	void DrawSprite(Texture2D& texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
	void DrawSprite(Texture2D& texture, glm::vec2 position, 
		SpriteAnimation animation, double time, int animStartTime,
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
private:
	Shader		 shader;
	unsigned int quadVBO, quadVAO;

	void initRenderData();
};

#endif