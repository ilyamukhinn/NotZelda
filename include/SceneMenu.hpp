#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include <map>
#include <memory>
#include <deque>

#include "Scene.hpp"
#include "EntityManager.hpp"

class SceneMenu : public Scene {
protected:
	std::string                 m_title;
	std::vector<std::string>	m_menuStrings;
	std::vector<std::string>	m_levelPaths;
	sf::Text					m_menuText;
	size_t						m_selectedMenuIndex{ 0 };

	void init();
	void update() override;
	void onEnd() override;
	void sDoAction(const Action& action) override;

public:
	SceneMenu(GameEngine* gameEngine = nullptr);
	void sRender();
};

#endif // !SCENE_MENU_H
