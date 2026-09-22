#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <unordered_set>
#include <SFML/Graphics.hpp>

#define BOX_SIZE 80

// Easy diff
#define BOX_NUMBER 10
#define BOX_SCALE 0.5f

extern bool is_timer_on;
extern bool is_entity_changed;

int prepare_engine_declarations(sf::RenderWindow& window);
void set_ingame_draws(sf::RenderWindow& window, sf::Clock clock);
void set_ingame_events(sf::RenderWindow& window, sf::Event& event, sf::Clock clock);

#endif