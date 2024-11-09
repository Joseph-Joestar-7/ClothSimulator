#pragma once
#include <SFML/Graphics.hpp>
#include "vendor/glm/glm.hpp"

class Particle {
public:
	glm::vec2 position;
	glm::vec2 prev_position;
	glm::vec2 acceleration;
	bool is_pinned;


	Particle(float x, float y, bool pinned = false) : position(x, y), prev_position(x, y),
		acceleration(x, y), is_pinned(pinned) {}

	void apply_force(const glm::vec2& force)
	{
		if (!is_pinned) {
			acceleration += force;
		}
	}

	void update(float time_step)
	{
		//verlet integration
		glm::vec2 velocity = position - prev_position;
		prev_position = position;
		position += velocity + acceleration * time_step * time_step;
		acceleration = glm::vec2(0, 0);  //reset accn
	}

	void constraint_to_bounds(float width, float height, float radius)
	{
		if (position.x < radius)
			position.x = radius;

		if (position.x > width - radius)
			position.x = width - radius;

		if (position.y < radius)
			position.y = radius;

		if (position.y > height - radius)
			position.y = height - radius;
	}
};