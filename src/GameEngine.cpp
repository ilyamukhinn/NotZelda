#include "GameEngine.hpp"
#include "Assets.hpp"
#include "Enums.hpp"
#include "ScenePlay.hpp"
#include "SceneMenu.hpp"
#include "imgui.h"
#include "imconfig-SFML.h"

#include <iostream>

GameEngine::GameEngine(const std::string& path) {
	this->init(path);
}

void GameEngine::init(const std::string& path) {
	this->m_assets.loadFromFile(path);

	this->m_window.create(sf::VideoMode(1280, 768), "Definitely Not Mario");
	this->m_window.setFramerateLimit(60);

	ImGui::SFML::Init(this->m_window);

	this->changeScene(SceneTag::MENU, std::make_shared<SceneMenu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene() {
	return this->m_sceneMap.at(this->m_currentScene);
}

bool GameEngine::isRunning() {
	return this->m_running && this->m_window.isOpen();
}

sf::RenderWindow& GameEngine::window() {
	return this->m_window;
}

void GameEngine::run() {
	while (this->isRunning()) {
		this->update();
	}
	ImGui::SFML::Shutdown();
}

void GameEngine::playSound(const SoundTag& soundName) {
	this->m_assets.getSound(soundName).play();
}

void GameEngine::stopSound(const SoundTag& soundName) {
	this->m_assets.getSound(soundName).stop();
}

void GameEngine::sUserInput() {
	sf::Event event;
	while (this->m_window.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed) {
			this->quit();
		}

		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::X) {
				std::cout << "screenshot saved to " << "test.png" << std::endl;
				sf::Texture texture;
				texture.create(this->m_window.getSize().x,
					this->m_window.getSize().y);
				texture.update(this->m_window);
				if (texture.copyToImage().saveToFile("test.png")) {
					std::cout << "screenshot saved to " << "test.png" << std::endl;
				}
			}
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
			if (this->currentScene()->getActionMap().find(event.key.code) ==
				this->currentScene()->getActionMap().end()) {
				continue;
			}

			const ActionType actionType = (event.type == sf::Event::KeyPressed) ?
				ActionType::START : ActionType::END;

			this->currentScene()->sDoAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
		}
	}
}

void GameEngine::changeScene(const SceneTag& sceneTag, std::shared_ptr<Scene> scene,
	bool endCurrentScene) {
	if (scene) {
		this->m_sceneMap[sceneTag] = scene;
	}
	else {
		if (this->m_sceneMap.find(sceneTag) == this->m_sceneMap.end()) {
			std::cerr << "Warning: Scene does not exist: " << sceneTagStrMap.at(sceneTag) << std::endl;
		}
	}

	if (endCurrentScene) {
		this->m_sceneMap.erase(this->m_sceneMap.find(this->m_currentScene));
	}

	this->m_currentScene = sceneTag;
}

const Assets& GameEngine::assets() const {
	return this->m_assets;
}

void GameEngine::quit() {
	this->m_running = false;
}

void GameEngine::update() {
	if (!this->isRunning() ||
		this->m_sceneMap.empty()) {
		return;
	}

	ImGui::SFML::Update(this->m_window, m_DeltaClock.restart());

	this->sUserInput();
	this->currentScene()->simulate(this->m_simulationSpeed);
	this->currentScene()->sRender();
	ImGui::SFML::Render(this->m_window);
	this->m_window.display();
}