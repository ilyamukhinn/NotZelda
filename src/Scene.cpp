#include "Scene.hpp"
#include "GameEngine.hpp"

#include <iostream>

void Scene::registerAction(int inputKey, const uint32_t& actionName) {
	if (this->m_actionMap.find(inputKey) == this->m_actionMap.end()) {
		this->m_actionMap[inputKey] = actionName;
	}
	else {
		std::cerr << "Warning: Action is already registered: " << this->m_actionMap[inputKey] << std::endl;
	}
}

void Scene::setPaused(bool paused) {
	this->m_paused = paused;
}

void Scene::simulate(const size_t frames) {
	for (size_t i = 0; i < frames; i++) {
		this->update();
		this->m_currentFrame += 1;
	}
}

size_t Scene::width() const {
	return this->m_game->window().getSize().x;
}

size_t Scene::height() const {
	return this->m_game->window().getSize().y;
}

size_t Scene::currentFrame() const {
	return this->m_currentFrame;
}

const ActionMap& Scene::getActionMap() const {
	return this->m_actionMap;
}

void Scene::drawLine(const Vec2f& p1, const Vec2f& p2) {
	sf::Vertex line[] = { sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y) };
	this->m_game->window().draw(line, 2, sf::Lines);
}