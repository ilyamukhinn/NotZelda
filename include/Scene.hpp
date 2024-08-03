#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "Action.hpp"
#include "EntityManager.hpp"

class GameEngine;

typedef std::map<int, uint32_t> ActionMap;

class Scene {
protected:
    GameEngine* m_game{ nullptr };
    EntityManager   m_entityManager;
    ActionMap       m_actionMap;
    bool            m_paused{ false };
    size_t          m_currentFrame{ 0 };

    virtual void onEnd() = 0;
    void setPaused(bool paused);

public:
    Scene() = default;
    Scene(GameEngine* gameEngine)
        : m_game(gameEngine) {}


    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;

    void simulate(const size_t frames);
    void registerAction(int inputKey,
        const uint32_t& actionName);

    size_t width() const;
    size_t height() const;
    size_t currentFrame() const;
    bool hasEnded() const {};
    const ActionMap& getActionMap() const;
    void drawLine(const Vec2f& p1, const Vec2f& p2);
};

#endif // !SCENE_H
