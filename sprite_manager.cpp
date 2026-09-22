#include "sprite_manager.h"

#include <vector>

std::vector<sf::Texture> sprite_manager::entity_memories;
std::vector<sf::Texture> sprite_manager::emoji_memories;

bool sprite_manager::read_entities(std::string location) {
	sf::Texture texture;
	if (!texture.loadFromFile(location))
		return false;

	entity_memories.push_back(texture);
	return true;
}
sf::Texture sprite_manager::return_entity_texture(int index) {
	if (index < 0 || index >= entity_memories.size()) {
		return sf::Texture();
	}

	return entity_memories.at(index);
}
sf::Sprite sprite_manager::return_entity_model(int index) {
	if (index < 0 || index >= entity_memories.size()) {
		return sf::Sprite();
	}
	
	sf::Sprite buffer_sprite(entity_memories.at(index));
	return buffer_sprite;
}

bool sprite_manager::read_emojis(std::string location) {
	sf::Texture texture;
	if (!texture.loadFromFile(location))
		return false;

	emoji_memories.push_back(texture);
	return true;
}
sf::Texture sprite_manager::return_emoji_texture(int index) {
	if (index < 0 || index >= emoji_memories.size()) {
		return sf::Texture();
	}

	return emoji_memories.at(index);
}
sf::Sprite sprite_manager::return_emoji_model(int index) {
	if (index < 0 || index >= emoji_memories.size()) {
		return sf::Sprite();
	}

	sf::Sprite buffer_sprite(emoji_memories.at(index));
	return buffer_sprite;
}