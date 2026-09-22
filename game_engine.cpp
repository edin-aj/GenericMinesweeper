#include "game_engine.h"
#include "sprite_manager.h"
#include "resources_list.h"
#include <iostream>

std::vector<sf::Text> total_textual_sprites;
std::vector<sf::Sprite> total_entity_sprites;
std::vector<sf::Sprite> total_emoji_sprites;

std::unordered_set<int> mines_positions;

bool is_timer_on = false;
long timer_count = 0;

sf::Color background(0, 51, 20);
sf::Color number_mines_colors[9];

sf::Font font_text;
sf::Font font_counter;
sf::RectangleShape table_builder;

int count_score = 0;
int score_iterator = 1;
int blocks_expanded = 0;

float machine_time_runner = 0.0;
float machine_time_drawer = 0.0;

bool is_mine_found = false;
bool is_face_clicked = false;
bool is_entity_changed = true;

enum EMOJI_FACES {
    FACE_NEUTRAL,
    FACE_EXPRESSION,
    FACE_CLICKED,

    // For counting/iteration purposes
    FACE_COUNT
};
enum ENTITY_TEXTURE {
    ENTITY_EMPTY,
    ENTITY_FLAGGED,
    ENTITY_MINE,
    ENTITY_UNOPEN,

    // For counting/iteration purposes
    ENTITY_COUNT
};

std::string list_expressions[FACE_COUNT] = {
    "models/m_face_neutral.texture",
    "models/m_face_expression.texture",
    "models/m_face_clicked.texture"
};
std::string list_entities[ENTITY_COUNT] = {
    "models/m_block_empty.texture",
    "models/m_block_flagged.texture",
    "models/m_block_mine.texture",
    "models/m_block_unopen.texture"
};

int flags_count = -1;
int iterator_face_condition = FACE_NEUTRAL;
sf::Sprite global_face_status;

std::string get_time_format(long timer) {
    std::string time_format;
    time_format += (timer / 3600 < 10) ? "0" : "";
    time_format += std::to_string(timer / 3600);
    time_format += (timer / 60 < 10) ? ":0" : ":";
    time_format += std::to_string(timer / 60);
    time_format += (timer % 60 < 10) ? ":0" : ":";
    time_format += std::to_string(timer % 60);
    return time_format;
}
float get_dynamically_text_size(sf::Text text) {
    float dynamic_size = 1;
    while (true) {
        sf::Text buffer = text;
        buffer.setCharacterSize(dynamic_size);
        sf::FloatRect text_bounds = buffer.getLocalBounds();

        // 3.5 + 3.5 + 2.0 = 9.0 [(size outerline x 2) + space before/after text]
        if (text_bounds.height < table_builder.getLocalBounds().height - 9 && text_bounds.width < table_builder.getLocalBounds().width - 9) dynamic_size++;
        else break;
    }

    return dynamic_size;
}

void show_all_blocks(std::unordered_set<int> mines) {
    for (int i = 0; i < total_entity_sprites.size(); i++) {
        if (mines_positions.find(i) != mines_positions.end()) {
            sf::Sprite buffer = sprite_manager::return_entity_model(ENTITY_MINE);
            buffer.setPosition(total_entity_sprites.at(i).getPosition().x, total_entity_sprites.at(i).getPosition().y);
            buffer.setScale(BOX_SCALE, BOX_SCALE);
            total_entity_sprites.at(i) = buffer;
        }
    }
}

void restart_mines_blocks(sf::RenderWindow& window) {
    int offset_pos_x = (window.getSize().x - BOX_NUMBER * BOX_SIZE * BOX_SCALE) / 2.0f;

    iterator_face_condition = FACE_NEUTRAL;
    is_mine_found = false;

    if (total_entity_sprites.size())
        total_entity_sprites.clear();

    for (int i = 0; i < BOX_NUMBER; i++) {
        for (int j = 0; j < BOX_NUMBER; j++) {
            sf::Sprite buffer = sprite_manager::return_entity_model(ENTITY_UNOPEN);
            buffer.setPosition((i * BOX_SIZE) * BOX_SCALE + offset_pos_x, window.getSize().y - (j + 1) * BOX_SIZE * BOX_SCALE);
            buffer.setScale(BOX_SCALE, BOX_SCALE);
            total_entity_sprites.push_back(buffer);
        }
    }
    
    if(total_textual_sprites.size())
        total_textual_sprites.clear();

    if (mines_positions.size())
        mines_positions.clear();

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    while (mines_positions.size() < BOX_NUMBER - 1) {
        int randomizer = std::rand() % total_entity_sprites.size();
        if (mines_positions.find(randomizer) == mines_positions.end())
            mines_positions.insert(randomizer);
    }

    count_score = 0;
}
void get_numbers_coloring() {
    number_mines_colors[0] = sf::Color::Transparent;
    number_mines_colors[1] = sf::Color(1, 0, 255);
    number_mines_colors[2] = sf::Color(0, 126, 0);
    number_mines_colors[3] = sf::Color(253, 1, 0);
    number_mines_colors[4] = sf::Color(1, 0, 127);
    number_mines_colors[5] = sf::Color(127, 1, 2);
    number_mines_colors[6] = sf::Color(0, 128, 127);
    number_mines_colors[7] = sf::Color(0, 0, 0);
    number_mines_colors[8] = sf::Color(128, 128, 128);
}
int get_mine_surroundings(int index, int element_per_row, int element_per_coloumn, std::unordered_set<int> mines) {
    int counter = 0;

    // Careful, you might bleed from your eyes after reading how hardcoded this is
    if ((index - 1) / element_per_row == (index / element_per_row)) {
        if (mines.find(index - 1) != mines.end())
            ++counter;
    }

    if ((index + 1) / element_per_row == (index / element_per_row)) {
        if (mines.find(index + 1) != mines.end())
            ++counter;
    }

    if (index / element_per_row > 0) {
        if ((index - element_per_row) / element_per_row == ((index - element_per_row) / element_per_row)) {
            if (mines.find(index - element_per_row) != mines.end())
                ++counter;
        }

        if ((index - element_per_row - 1) / element_per_row == ((index - element_per_row) / element_per_row)) {
            if (mines.find(index - element_per_row - 1) != mines.end())
                ++counter;
        }

        if ((index - element_per_row + 1) / element_per_row == ((index - element_per_row) / element_per_row)) {
            if (mines.find(index - element_per_row + 1) != mines.end())
                ++counter;
        }
    }

    if (index / element_per_row < element_per_coloumn - 1) {
        if ((index + element_per_row) / element_per_row == ((index + element_per_row) / element_per_row)) {
            if (mines.find(index + element_per_row) != mines.end())
                ++counter;
        }

        if ((index + element_per_row - 1) / element_per_row == ((index + element_per_row) / element_per_row)) {
            if (mines.find(index + element_per_row - 1) != mines.end())
                ++counter;
        }

        if ((index + element_per_row + 1) / element_per_row == ((index + element_per_row) / element_per_row)) {
            if (mines.find(index + element_per_row + 1) != mines.end())
                ++counter;
        }
    }

    return counter;
}
void expand_blocks_recursive(int index, std::vector<sf::Text>& textual_sprites, std::vector<sf::Sprite>& entity_sprites, std::unordered_set<int>& mines_positions) {
    if (sprite_manager::return_entity_model(ENTITY_UNOPEN).getTexture() != entity_sprites.at(index).getTexture())
        return;

    sf::Sprite sprite_emptiness = sprite_manager::return_entity_model(ENTITY_EMPTY);
    sprite_emptiness.setPosition(entity_sprites.at(index).getPosition().x, entity_sprites.at(index).getPosition().y);
    sprite_emptiness.setScale(BOX_SCALE, BOX_SCALE);
    entity_sprites.at(index) = sprite_emptiness;
    is_entity_changed = true;
    count_score += 1 * score_iterator++;
    ++blocks_expanded;

    if (entity_sprites.size() - blocks_expanded - 1 < mines_positions.size()) {
        for (int i = 0; i < entity_sprites.size(); i++) {
            if (entity_sprites.at(i).getTexture() == sprite_manager::return_entity_model(ENTITY_UNOPEN).getTexture()) {
                sf::Sprite sprite_completed = sprite_manager::return_entity_model(ENTITY_FLAGGED);
                sprite_completed.setPosition(entity_sprites.at(i).getPosition().x, entity_sprites.at(i).getPosition().y);
                sprite_completed.setScale(BOX_SCALE, BOX_SCALE);
                entity_sprites.at(i) = sprite_completed;
            }
        }

        iterator_face_condition = FACE_EXPRESSION;
        is_timer_on = false;
        flags_count = 0;
    }

    int count = get_mine_surroundings(index, BOX_NUMBER, BOX_NUMBER, mines_positions);
    if (count) {
        sf::Text text(std::to_string(count), font_counter);
        text.setCharacterSize(entity_sprites.at(index).getGlobalBounds().width);
        text.setFillColor(number_mines_colors[count]);
        text.setPosition(entity_sprites.at(index).getGlobalBounds().left + (entity_sprites.at(index).getGlobalBounds().width - text.getLocalBounds().width) / 2, entity_sprites.at(index).getGlobalBounds().top - (entity_sprites.at(index).getGlobalBounds().height - text.getLocalBounds().height) / 2);
        textual_sprites.push_back(text);
        return;
    }

    // Careful, you might bleed from your eyes after reading how hardcoded this is
    if ((index - 1) / BOX_NUMBER == (index / BOX_NUMBER) && (index - 1) >= 0)
        expand_blocks_recursive(index - 1, textual_sprites, entity_sprites, mines_positions);

    if ((index + 1) / BOX_NUMBER == (index / BOX_NUMBER) && (index + 1) < entity_sprites.size())
        expand_blocks_recursive(index + 1, textual_sprites, entity_sprites, mines_positions);

    if ((index - BOX_NUMBER) / BOX_NUMBER == ((index - BOX_NUMBER) / BOX_NUMBER) && (index - BOX_NUMBER) >= 0)
        expand_blocks_recursive(index - BOX_NUMBER, textual_sprites, entity_sprites, mines_positions);

    if ((index + BOX_NUMBER) / BOX_NUMBER == ((index + BOX_NUMBER) / BOX_NUMBER) && (index + BOX_NUMBER) < entity_sprites.size())
        expand_blocks_recursive(index + BOX_NUMBER, textual_sprites, entity_sprites, mines_positions);

    if ((index - BOX_NUMBER - 1) % BOX_NUMBER < (index % BOX_NUMBER) && (index - BOX_NUMBER - 1) >= 0)
        expand_blocks_recursive(index - BOX_NUMBER - 1, textual_sprites, entity_sprites, mines_positions);

    if ((index - BOX_NUMBER + 1) % BOX_NUMBER > (index % BOX_NUMBER) && (index - BOX_NUMBER + 1) >= 0)
        expand_blocks_recursive(index - BOX_NUMBER + 1, textual_sprites, entity_sprites, mines_positions);

    if ((index + BOX_NUMBER - 1) % BOX_NUMBER < (index % BOX_NUMBER) && (index + BOX_NUMBER - 1) < entity_sprites.size())
        expand_blocks_recursive(index + BOX_NUMBER - 1, textual_sprites, entity_sprites, mines_positions);

    if ((index + BOX_NUMBER + 1) % BOX_NUMBER > (index % BOX_NUMBER) && (index + BOX_NUMBER + 1) < entity_sprites.size())
        expand_blocks_recursive(index + BOX_NUMBER + 1, textual_sprites, entity_sprites, mines_positions);
}

int prepare_engine_declarations(sf::RenderWindow& window) {
    if (!font_counter.loadFromMemory(font_counter_data, font_counter_data_size))
        return -3;

    if (!font_text.loadFromMemory(font_text_data, font_text_data_size))
        return -2;

    for (int i = 0; i < ENTITY_COUNT; i++) {
        if (!(sprite_manager::read_entities(list_entities[i]))) {
            return -1;
        }
    }

    restart_mines_blocks(window);
    get_numbers_coloring();

    for (int i = 0; i < FACE_COUNT; i++) {
        if (!(sprite_manager::read_emojis(list_expressions[i]))) {
            return 0;
        }
    }

    table_builder.setFillColor(sf::Color(0, 92, 36));
    table_builder.setOutlineColor(sf::Color(58, 122, 0));
    table_builder.setOutlineThickness(3.5);
    flags_count = mines_positions.size();
    return 1;
}
void set_ingame_draws(sf::RenderWindow& window, sf::Clock clock) {
    if (!is_entity_changed)
        return;

    is_entity_changed = false;
    window.clear(background);

    for (const auto& entity : total_entity_sprites)
        window.draw(entity);

    for (const auto& text : total_textual_sprites)
        window.draw(text);

    global_face_status = sprite_manager::return_emoji_model(iterator_face_condition);
    global_face_status.setPosition((window.getSize().x / 2) - (sprite_manager::return_emoji_model(iterator_face_condition).getGlobalBounds().getSize().x / 2), (sprite_manager::return_emoji_model(iterator_face_condition).getGlobalBounds().getSize().y / 5));
    window.draw(global_face_status);

    if (is_face_clicked) {
        sf::Sprite buffer = sprite_manager::return_emoji_model(FACE_CLICKED);
        buffer.setPosition(global_face_status.getPosition().x + 3.0, global_face_status.getPosition().y + 3.0);
        window.draw(buffer);
    }

    // Flags table
    table_builder.setPosition(sf::Vector2f(global_face_status.getGlobalBounds().getSize().x / 3, global_face_status.getGlobalBounds().getPosition().y + 3.5)); // 3.5 = outerline thickness
    table_builder.setSize(sf::Vector2f((window.getSize().x / 2) - (global_face_status.getGlobalBounds().getSize().x / 3) - (global_face_status.getGlobalBounds().getSize().x / 2) - 20, (global_face_status.getGlobalBounds().getSize().y / 2) - 20)); // 20 = my desired number to give space between objects
    window.draw(table_builder);
    
    std::string buffer = "Flags remaining: " + std::to_string(flags_count);
    sf::Text flags_text(buffer, font_text, 1);
    flags_text.setFillColor(sf::Color(0, 212, 83));
    flags_text.setCharacterSize(get_dynamically_text_size(flags_text) - 1);
    flags_text.setOrigin(flags_text.getLocalBounds().left + flags_text.getLocalBounds().width / 2.0f, flags_text.getLocalBounds().top + flags_text.getLocalBounds().height / 2.0f);
    flags_text.setPosition(sf::Vector2f(table_builder.getPosition().x + table_builder.getSize().x / 2, table_builder.getPosition().y + table_builder.getSize().y / 2));
    window.draw(flags_text);

    // Score table
    table_builder.setPosition(sf::Vector2f(global_face_status.getGlobalBounds().getSize().x / 3, global_face_status.getGlobalBounds().getPosition().y + (global_face_status.getGlobalBounds().getSize().x / 2) + 16.5)); // Seperation - thickness = 16.5
    table_builder.setSize(sf::Vector2f((window.getSize().x / 2) - (global_face_status.getGlobalBounds().getSize().x / 3) - (global_face_status.getGlobalBounds().getSize().x / 2) - 20, (global_face_status.getGlobalBounds().getSize().y / 2) - 20));
    window.draw(table_builder);

    buffer = "Score: " + std::to_string(count_score);
    sf::Text score_text(buffer, font_text, 1);
    score_text.setFillColor(sf::Color(0, 212, 83));
    score_text.setCharacterSize(get_dynamically_text_size(score_text) - 10);
    score_text.setOrigin(score_text.getLocalBounds().left + score_text.getLocalBounds().width / 2.0f, score_text.getLocalBounds().top + score_text.getLocalBounds().height / 2.0f);
    score_text.setPosition(sf::Vector2f(table_builder.getPosition().x + table_builder.getSize().x / 2, table_builder.getPosition().y + table_builder.getSize().y / 2));
    window.draw(score_text);

    // Time text table
    table_builder.setPosition(sf::Vector2f((window.getSize().x / 2) + (global_face_status.getGlobalBounds().getSize().x / 2) + 20, global_face_status.getGlobalBounds().getPosition().y + 3.5));
    table_builder.setSize(sf::Vector2f((window.getSize().x / 2) - (global_face_status.getGlobalBounds().getSize().x / 3) - (global_face_status.getGlobalBounds().getSize().x / 2) - 20, (global_face_status.getGlobalBounds().getSize().y / 2) - 3.5));
    window.draw(table_builder);

    sf::Text time_text("Time counter", font_text, 1);
    time_text.setFillColor(sf::Color(0, 212, 83));
    time_text.setCharacterSize(get_dynamically_text_size(time_text) - 10);
    time_text.setOrigin(time_text.getLocalBounds().left + time_text.getLocalBounds().width / 2.0f, time_text.getLocalBounds().top + time_text.getLocalBounds().height / 2.0f);
    time_text.setPosition(sf::Vector2f(table_builder.getPosition().x + table_builder.getSize().x / 2, table_builder.getPosition().y + table_builder.getSize().y / 2));
    window.draw(time_text);

    // Time variable table
    table_builder.setPosition(sf::Vector2f((window.getSize().x / 2) + (global_face_status.getGlobalBounds().getSize().x / 2) + 20, global_face_status.getGlobalBounds().getPosition().y + (global_face_status.getGlobalBounds().getSize().y / 2)));
    table_builder.setSize(sf::Vector2f((window.getSize().x / 2) - (global_face_status.getGlobalBounds().getSize().x / 3) - (global_face_status.getGlobalBounds().getSize().x / 2) - 20, (global_face_status.getGlobalBounds().getSize().y / 2) - 3.5));
    window.draw(table_builder);

    static long timer;
    if (is_timer_on) timer = static_cast<long>(clock.getElapsedTime().asSeconds() - timer_count);
    else if (timer_count == 0) {
        is_timer_on = false;
        timer = 0;
    }

    sf::Text time_variable_text(get_time_format(timer), font_text, 1);
    time_variable_text.setFillColor(sf::Color(0, 212, 83));
    time_variable_text.setCharacterSize(get_dynamically_text_size(time_variable_text) - 10);
    time_variable_text.setOrigin(time_variable_text.getLocalBounds().left + time_variable_text.getLocalBounds().width / 2.0f, time_variable_text.getLocalBounds().top + time_variable_text.getLocalBounds().height / 2.0f);
    time_variable_text.setPosition(sf::Vector2f(table_builder.getPosition().x + table_builder.getSize().x / 2, table_builder.getPosition().y + table_builder.getSize().y / 2));
    window.draw(time_variable_text);

    window.display();
}
void set_ingame_events(sf::RenderWindow& window, sf::Event& event, sf::Clock clock) {
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (is_face_clicked) {
            is_timer_on = false;
            timer_count = 0;
            blocks_expanded = 0;
            is_face_clicked = false; 
            is_entity_changed = true;
            iterator_face_condition = FACE_NEUTRAL;
            restart_mines_blocks(window);
            flags_count = mines_positions.size();
        }
        else if(!is_mine_found) {
            sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
            auto sprite_clicked = std::find_if(total_entity_sprites.begin(), total_entity_sprites.end(), [mouse_position](const sf::Sprite& sprite) {
                return sprite.getGlobalBounds().contains(static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y));
                });
            if (sprite_clicked != total_entity_sprites.end() && sprite_manager::return_entity_model(ENTITY_UNOPEN).getTexture() == sprite_clicked->getTexture()) {
                if (mines_positions.find(sprite_clicked - total_entity_sprites.begin()) != mines_positions.end()) {
                    sf::Sprite buffer = sprite_manager::return_entity_model(ENTITY_MINE);
                    buffer.setPosition(sprite_clicked->getPosition().x, sprite_clicked->getPosition().y);
                    buffer.setScale(BOX_SCALE, BOX_SCALE);
                    *sprite_clicked = buffer;
                    is_timer_on = false;
                    is_entity_changed = true;
                    is_mine_found = true;
                    iterator_face_condition = FACE_EXPRESSION;
                    show_all_blocks(mines_positions);
                }
                else {
                    if (!is_timer_on) {
                        is_timer_on = true;
                        timer_count = clock.getElapsedTime().asSeconds();
                    }

                    expand_blocks_recursive(sprite_clicked - total_entity_sprites.begin(), total_textual_sprites, total_entity_sprites, mines_positions);
                    score_iterator = 1;
                }
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right && !is_mine_found) {
        sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
        auto sprite_clicked = std::find_if(total_entity_sprites.begin(), total_entity_sprites.end(), [mouse_position](const sf::Sprite& sprite) {
            return sprite.getGlobalBounds().contains(static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y));
            });

        if (sprite_clicked != total_entity_sprites.end()) {
            if (sprite_manager::return_entity_model(ENTITY_EMPTY).getTexture() != sprite_clicked->getTexture() && sprite_manager::return_entity_model(ENTITY_MINE).getTexture() != sprite_clicked->getTexture()) {
                if (sprite_manager::return_entity_model(ENTITY_FLAGGED).getTexture() != sprite_clicked->getTexture() && flags_count > 0) {
                    sf::Sprite buffer = sprite_manager::return_entity_model(ENTITY_FLAGGED);
                    buffer.setPosition(sprite_clicked->getPosition().x, sprite_clicked->getPosition().y);
                    buffer.setScale(BOX_SCALE, BOX_SCALE);
                    *sprite_clicked = buffer;
                    is_entity_changed = true;
                    --flags_count;
                }
                else if (sprite_manager::return_entity_model(ENTITY_UNOPEN).getTexture() != sprite_clicked->getTexture() && flags_count < mines_positions.size()) {
                    sf::Sprite buffer = sprite_manager::return_entity_model(ENTITY_UNOPEN);
                    buffer.setPosition(sprite_clicked->getPosition().x, sprite_clicked->getPosition().y);
                    buffer.setScale(BOX_SCALE, BOX_SCALE);
                    *sprite_clicked = buffer;
                    is_entity_changed = true;
                    ++flags_count;
                }
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !is_face_clicked) {
        sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
        if (global_face_status.getGlobalBounds().contains(static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y))) {
            is_face_clicked = true;
            is_entity_changed = true;
        }
    }
}