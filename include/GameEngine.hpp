#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "Scene.hpp"
#include "Assets.hpp"
#include "Enums.hpp"

#include <memory>

typedef std::map<SceneTag, std::shared_ptr<Scene>> SceneMap;

class GameEngine {
protected:
    sf::RenderWindow    m_window;
    Assets              m_assets;
    SceneTag            m_currentScene;
    SceneMap            m_sceneMap;
    size_t              m_simulationSpeed{ 1 };
    bool                m_running{ true };
    sf::Clock			m_DeltaClock;

    void init(const std::string& path);
    void update();

    void sUserInput();

    std::shared_ptr<Scene> currentScene();

public:
    GameEngine(const std::string& path);

    void changeScene(const SceneTag& sceneTag, std::shared_ptr<Scene> scene,
        bool endCurrentScene = false);

    void quit();
    void run();

    void playSound(const SoundTag& soundName);
    void stopSound(const SoundTag& soundName);

    sf::RenderWindow& window();
    const Assets& assets() const;
    bool isRunning();
};

#endif // !GAME_ENGINE_H
