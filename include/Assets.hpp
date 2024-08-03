#ifndef ASSETS_H
#define ASSETS_H

#include "Animation.hpp"
#include "Enums.hpp"

#include <SFML/Audio.hpp>

class Assets
{
    std::unordered_map<std::string, sf::Texture>    m_textureMap;
    std::unordered_map<AnimationTag, Animation>     m_animationMap;
    std::unordered_map<std::string, sf::Font>       m_fontMap;
    std::unordered_map<SoundTag, sf::Sound>         m_soundMap;
    std::unordered_map<SoundTag, sf::SoundBuffer>   m_soundBufferMap;

    void addTexture(const std::string& textureName, const std::string& path, bool smooth = true);
    void addAnimation(const AnimationTag& animationName, const std::string& textureName, size_t frameCount, size_t speed);
    void addFont(const std::string& fontName, const std::string& path);
    void addSound(const SoundTag& soundName, const std::string& path);

public:

    Assets() = default;

    void loadFromFile(const std::string& path);

    const std::unordered_map<SoundTag, sf::Sound>& getSounds() const { return this->m_soundMap; };
    const std::unordered_map<AnimationTag, Animation>& getAnimations() const { return this->m_animationMap; };
    const std::unordered_map<std::string, sf::Texture>& getTextures() const { return this->m_textureMap; };

    const sf::Texture& getTexture(const std::string& textureName) const;
    const Animation& getAnimation(const AnimationTag& animationName) const;
    const sf::Font& getFont(const std::string& fontName) const;
    sf::Sound& getSound(const SoundTag& soundName);
};

#endif // !ASSETS_H
