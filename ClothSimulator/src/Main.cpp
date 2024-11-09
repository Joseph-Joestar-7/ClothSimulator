#include "Particle.hpp"
#include "Constraint.hpp"
#include "Input_handler.hpp"
#include <vector>

const int WIDTH=1000;
const int HEIGHT = 720;
const float PARTICLE_RADIUS = 10.0f;
const float GRAVITY = 10.0f;
const float TIME_STEP = 0.01f;

const int ROW = 10;
const int COL = 10;
const float REST_DISTANCE = 30.0f;

int main() {
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cloth Simulator");
	window.setFramerateLimit(60);

	std::vector<Particle> particles;
	std::vector<Constraint> constraints;

	for (int row = 0; row < ROW; row++) {
		for (int col = 0; col < COL; col++) {
			float x = col * REST_DISTANCE + WIDTH / 3;
			float y = row * REST_DISTANCE + HEIGHT / 3;
			bool pinned = (row == 0);
			particles.emplace_back(x, y, pinned);
		}
	}

	// Initialize constraints
	for (int row = 0; row < ROW; row++) {
		for (int col = 0; col < COL; col++) {
			if (col < COL - 1) {
				// Horizontal constraint
				constraints.emplace_back(&particles[row * COL + col], &particles[row * COL + col + 1]);
			}
			if (row < ROW - 1) {
				// Vertical constraint
				constraints.emplace_back(&particles[row * COL + col], &particles[(row + 1) * COL + col]);
			}
		}
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			// handle mouse clicks
			InputHandler::handle_mouse_click(event, particles, constraints);

		}

		//apply grav and update particles
		for (auto& particle : particles)
		{
			particle.apply_force(glm::vec2(0, GRAVITY));
			particle.update(TIME_STEP);
			particle.constraint_to_bounds(WIDTH, HEIGHT, PARTICLE_RADIUS);
		}

		for (size_t i = 0; i < 5; i++) {
			for (auto& constraint : constraints) {
				constraint.satisfy();
			}
		}

		window.clear(sf::Color::Black);

		//Draw Particles as balls
		/*for (const auto& particle : particles)
		{
			sf::CircleShape circle(PARTICLE_RADIUS);
			circle.setFillColor(sf::Color::White);
			circle.setPosition(sf::Vector2f(particle.position.x,particle.position.y));
			window.draw(circle);
		}*/

		// Draw particles as points
		for (const auto& particle : particles) {
			sf::Vertex point(sf::Vector2f(particle.position.x,particle.position.y), sf::Color::White);
			window.draw(&point, 1, sf::Points);
		}

		// Draw constraints as lines
		for (const auto& constraint : constraints) {
			if (!constraint.active) {
				continue;
			}
			sf::Vertex line[] = {
				sf::Vertex(sf::Vector2f(constraint.p1->position.x,constraint.p1->position.y), sf::Color::White),
				sf::Vertex(sf::Vector2f(constraint.p2->position.x,constraint.p2->position.y), sf::Color::White),
			};
			window.draw(line, 2, sf::Lines);
		}


		window.display();
	}
}