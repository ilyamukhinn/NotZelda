#include "SceneMenu.hpp"
#include "ScenePlay.hpp"
#include "Assets.hpp"
#include "GameEngine.hpp"
#include "Components.hpp"
#include "Action.hpp"

SceneMenu::SceneMenu(GameEngine* gameEngine)
	: Scene(gameEngine) {
	this->init();
}

void SceneMenu::init() {
	this->registerAction(sf::Keyboard::W, static_cast<uint32_t>(SceneMenuAction::UP));
	this->registerAction(sf::Keyboard::S, static_cast<uint32_t>(SceneMenuAction::DOWN));
	this->registerAction(sf::Keyboard::D, static_cast<uint32_t>(SceneMenuAction::PLAY));
	this->registerAction(sf::Keyboard::Escape, static_cast<uint32_t>(SceneMenuAction::QUIT));

	this->m_title = "Not Zelda";
	this->m_menuStrings.push_back("Level 1");
	this->m_menuStrings.push_back("Level 2");
	this->m_menuStrings.push_back("Level 3");

	this->m_levelPaths.push_back("configs/level1.txt");
	this->m_levelPaths.push_back("configs/level2.txt");
	this->m_levelPaths.push_back("configs/level3.txt");

	this->m_menuText.setFont(this->m_game->assets().getFont("Megaman"));
	this->m_menuText.setCharacterSize(64);
	this->m_game->playSound(SoundTag::MUSIC_TITLE);
}

void SceneMenu::update() {
	this->m_entityManager.update();
}

void SceneMenu::sDoAction(const Action& action) {
	if (action.type() == ActionType::START) {
		switch (action.name())
		{
		case static_cast<uint32_t>(SceneMenuAction::UP): {
			if (this->m_selectedMenuIndex > 0) { this->m_selectedMenuIndex--; }
			else { this->m_selectedMenuIndex = this->m_menuStrings.size() - 1; }
			break;
		}
		case static_cast<uint32_t>(SceneMenuAction::DOWN): {
			this->m_selectedMenuIndex = (this->m_selectedMenuIndex + 1) % this->m_menuStrings.size();
			break;
		}
		case static_cast<uint32_t>(SceneMenuAction::PLAY): {
			this->m_game->changeScene(SceneTag::PLAY,
				std::make_shared<ScenePlay>(this->m_game, this->m_levelPaths[this->m_selectedMenuIndex]));
			this->m_game->stopSound(SoundTag::MUSIC_TITLE);
			break;
		}
		case static_cast<uint32_t>(SceneMenuAction::QUIT): {
			this->onEnd();
			break;
		}
		default:
			break;
		}
	}
}


void SceneMenu::onEnd() {
	this->m_game->quit();
}

void SceneMenu::sRender() {
	m_game->window().setView(m_game->window().getDefaultView());
	m_game->window().clear(sf::Color(100, 100, 255));

	m_menuText.setCharacterSize(48);
	m_menuText.setString(m_title);
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setPosition(sf::Vector2f(10, 10));
	m_game->window().draw(m_menuText);

	for (size_t i = 0; i < m_menuStrings.size(); ++i){
		m_menuText.setString(m_menuStrings[i]);
		m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(0, 0, 0));
		m_menuText.setPosition(sf::Vector2f(10, 110 + i * 72));
		m_game->window().draw(m_menuText);
	}

	m_menuText.setCharacterSize(20);
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setString("up: w     down: s    play: d      back: esc");
	m_menuText.setPosition(sf::Vector2f(10, 690));
	m_game->window().draw(m_menuText);
}
