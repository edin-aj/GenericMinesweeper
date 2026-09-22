#include <iostream>

#include "sprite_manager.h"
#include "game_engine.h"

// These can be changed by the user
#define VIDEO_WIDTH 1024
#define VIDEO_HEIGHT 720
#define VIDEO_FRAMELIMIT 30

int main() {
    sf::RenderWindow window(sf::VideoMode(VIDEO_WIDTH, VIDEO_HEIGHT), "Generic Minesweeper!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(VIDEO_FRAMELIMIT);

    // TODO:
    // Build opening screen, design first!

    prepare_engine_declarations(window);

    sf::Clock clock_frame, clock_timer, clock_entity;
    sf::Time frame_timer = sf::seconds(1.0f / (VIDEO_FRAMELIMIT * 1.0f));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else {
                set_ingame_events(window, event, clock_timer);
            }
        }
        
        if (clock_entity.getElapsedTime() >= sf::seconds(1.0f) && is_timer_on) {
            is_entity_changed = true;
            clock_entity.restart();
        }

        set_ingame_draws(window, clock_timer);
        sf::sleep(frame_timer - clock_frame.restart());
    }
    return 0;
}