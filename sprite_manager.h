#ifndef SPRITE_READER_H
#define SPRITE_READER_H

#include <SFML/Graphics.hpp>
#include <string>

class sprite_manager {
public:
	static bool read_entities(std::string location);
	static sf::Texture return_entity_texture(int index);
	static sf::Sprite return_entity_model(int index);

	static bool read_emojis(std::string location);
	static sf::Texture return_emoji_texture(int index);
	static sf::Sprite return_emoji_model(int index);
private:
	static std::vector<sf::Texture> entity_memories;
	static std::vector<sf::Texture> emoji_memories;
};

#endif