#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

#include "Enums.hpp"
#include "Vec2.hpp"

class Animation {
    sf::Sprite      m_sprite;
    size_t          m_frameCount    { 1 };
    size_t          m_currentFrame  { 0 };
    size_t          m_speed         { 0 };
    Vec2f           m_size          { 0.0f, 0.0f };
    AnimationTag    m_name          { AnimationTag::UNDEFINED };

public:

    Animation() = default;
    Animation(const AnimationTag& name, const sf::Texture& t) :
        Animation(name, t, 1, 0) {};
    Animation(const AnimationTag& name, const sf::Texture& t, size_t frameCount, size_t speed);

    void update();
    bool hasEnded() const;
    const AnimationTag& getName() const;
    const Vec2f& getSize() const;
    const sf::Sprite& getSprite() const;
    sf::Sprite& getSprite();
};

#endif // !ANIMATION_H
