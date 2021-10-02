
#include "particle_emitter.h"
#include "game_object.h"
#include <vector>
#include <iostream>

Particle_Emitter::Particle_Emitter(Shader shader, Texture2D texture, unsigned int amount) 
	: shader(shader), texture(texture), amount(amount)
{
	init();
}

void Particle_Emitter::init() {
	// set up mesh and attribute properties
	unsigned int VBO;
	float particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	// create this->amount default particle instances
	for (unsigned int i = 0; i < this->amount; i++)
		this->particles.push_back(Particle());
}
void Particle_Emitter::Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset) {
	// add new particles
	for (unsigned int i = 0; i < newParticles; i++) {
		int unusedParticle = FirstUnusedParticle();
		this->RespawnParticle(this->particles[unusedParticle], object, offset);
	}
	// update all particles
	for (unsigned int i = 0; i < amount; i++) {
		Particle& p = particles[i];
		p.Life -= dt;
		if (p.Life > 0.0f) {
			// particles is alive, thus update
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5;
		}
	}
}

unsigned int lastUsedParticle = 0;
unsigned int Particle_Emitter::FirstUnusedParticle() {
	// search from last used partaicle, this will usually return almost instantly
	for (unsigned int i = lastUsedParticle; i < this->amount; i++) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// otherwise do a linear search
	for (unsigned int i = 0; i < lastUsedParticle; i++) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// override first particle if all others are alive
	lastUsedParticle = 0;
	return 0;
}

void Particle_Emitter::RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset) {
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = object.Position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}

void Particle_Emitter::Draw() {

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.Use();
	for (Particle particle : particles)
	{
		if (particle.Life > 0.0f) {
			this->shader.SetVector2f("offset", particle.Position);
			this->shader.SetVector4f("color", particle.Color);
			this->texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}