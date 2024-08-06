#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H

#include <map>
#include <memory>

#include "Scene.hpp"
#include "EntityManager.hpp"

class ScenePlay : public Scene {
	struct PlayerConfig {
		float X, Y, CW, CH, SPEED, HEALTH;
	};

protected:
	std::string			m_levelPath;
	PlayerConfig		m_playerConfig;
	bool				m_drawTextures	{ true };
	bool				m_drawCollision	{ false };
	bool				m_drawGrid		{ false };
	bool				m_follow		{ false };
	const Vec2f			m_gridSize		{ 64.0f, 64.0f };
	sf::Text			m_gridText;

	void init(const std::string& levelPath);
	void loadLevel(const std::string& filename);
	void spawnPlayer();
	void spawnSword(std::shared_ptr<Entity> e);
	Vec2f getPosition(int rx, int ry, int tx, int ty) const;
	std::shared_ptr<Entity> player();
	void update() override;
	void onEnd() override;

	void sMovement();
	void sAI();
	void sStatus();
	void sCollision();
	void sAnimation();
	void sCamera();
	void sGUI();
	void sRender();
	void sDoAction(const Action& action) override;

	std::pair<Vec2f, Vec2f> calcAnimationUV(const Animation& animation);

public:
	ScenePlay(GameEngine* gameEngine,
		const std::string& levelPath);
};

#endif // !SCENE_PLAY_H
