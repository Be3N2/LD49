#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "game_object.h"
#include "shader.h"
#include "texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float     Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

class Particle_Emitter
{
public:
	Particle_Emitter(Shader shader, Texture2D texture, unsigned int amount);
	void Update(float dt, GameObject &object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void Draw();
private:
	std::vector<Particle> particles;
	unsigned int amount;

	// render state
	Shader shader;
	Texture2D texture;
	unsigned int VAO;

	void init();

	unsigned int FirstUnusedParticle();

	void RespawnParticle(Particle& particle, GameObject& gameObject, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif