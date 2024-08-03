#include "Animation.hpp"

Animation::Animation(const AnimationTag& name, const sf::Texture& t, size_t frameCount, size_t speed)
    : m_name(name)
    , m_sprite(t)
    , m_frameCount(frameCount)
    , m_currentFrame(0)
    , m_speed(speed)
{
    this->m_size = Vec2f(static_cast<float>(t.getSize().x) / frameCount,
        static_cast<float>(t.getSize().y));
    this->m_sprite.setOrigin(this->m_size.x / 2.0f,
        this->m_size.y / 2.0f);
    this->m_sprite.setTextureRect(sf::IntRect(floor(this->m_currentFrame) * this->m_size.x,
        0, this->m_size.x, this->m_size.y));
}

void Animation::update() {
    if (this->m_speed && this->m_frameCount > 1) {
        this->m_currentFrame++;

        this->m_sprite.setTextureRect(
            sf::IntRect(floor(this->m_currentFrame / this->m_speed % this->m_frameCount) * this->m_size.x,
                0, this->m_size.x, this->m_size.y));
    }
}

const Vec2f& Animation::getSize() const {
    return this->m_size;
}

const sf::Sprite& Animation::getSprite() const {
    return this->m_sprite;
}

const AnimationTag& Animation::getName() const {
    return this->m_name;
}

sf::Sprite& Animation::getSprite() {
    return this->m_sprite;
}

bool Animation::hasEnded() const {
    return this->m_frameCount > 1 &&
        this->m_currentFrame >= this->m_frameCount * this->m_speed;
}