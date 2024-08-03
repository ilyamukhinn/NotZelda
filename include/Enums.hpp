#ifndef ENUMS_H
#define ENUMS_H

#include <string>
#include <unordered_map>
#include <type_traits>

template<typename Enum>
class EnumMask {
public:
    using Underlying = typename std::underlying_type<Enum>::type;

    EnumMask() : value(0) {}
    EnumMask(Enum e) : value(static_cast<Underlying>(e)) {}

    EnumMask& operator|=(EnumMask other) {
        value |= other.value;
        return *this;
    }

    EnumMask& operator&=(EnumMask other) {
        value &= other.value;
        return *this;
    }

    EnumMask& operator^=(EnumMask other) {
        value ^= other.value;
        return *this;
    }

    EnumMask operator|(EnumMask other) const {
        return EnumMask(static_cast<Enum>(value | other.value));
    }

    EnumMask operator&(EnumMask other) const {
        return EnumMask(static_cast<Enum>(value & other.value));
    }

    EnumMask operator^(EnumMask other) const {
        return EnumMask(static_cast<Enum>(value ^ other.value));
    }

    EnumMask operator~() const {
        return EnumMask(static_cast<Enum>(~value));
    }

    bool has(EnumMask mask) const {
        return (value & mask.value) == mask.value;
    }

private:
    Underlying value;
};

enum class SceneTag : uint32_t {
    UNDEFINED = 0,
    MENU = 1 << 0,
    PLAY = 1 << 1
};

static std::unordered_map<std::string, SceneTag> const strSceneTagMap = {
    { "Undefined",  SceneTag::UNDEFINED },
    { "Menu",       SceneTag::MENU },
    { "Play",       SceneTag::PLAY }
};

static std::unordered_map<SceneTag, std::string> const sceneTagStrMap = {
    { SceneTag::UNDEFINED,  "Undefined" },
    { SceneTag::MENU,       "Menu" },
    { SceneTag::PLAY,       "Play" }
};

enum class EntityTag : uint32_t {
    UNDEFINED   = 0,
    PLAYER      = 1 << 0,
    TILE        = 1 << 1,
    NPC         = 1 << 2,
    SWORD       = 1 << 3,
    EXPLOSION   = 1 << 4,
    PORTAL      = 1 << 5,
    HEART       = 1 << 6
};

static std::unordered_map<std::string, EntityTag> const strEntityTagMap = {
    { "Undefined",  EntityTag::UNDEFINED },
    { "Player",     EntityTag::PLAYER },
    { "Tile",       EntityTag::TILE},
    { "NPC",        EntityTag::NPC },
    { "Sword",      EntityTag::SWORD },
    { "Portal",     EntityTag::PORTAL },
    { "Heart",      EntityTag::HEART }
};

static std::unordered_map<EntityTag, std::string> const entityTagStrMap = {
    { EntityTag::UNDEFINED, "Undefined" },
    { EntityTag::PLAYER,    "Player" },
    { EntityTag::TILE,      "Tile" },
    { EntityTag::NPC,       "NPC" },
    { EntityTag::SWORD,     "Sword" },
    { EntityTag::PORTAL,    "Portal" },
    { EntityTag::HEART,     "Heart" }
};

enum class AssetTag : uint32_t {
    UNDEFINED   = 0,
    TEXTURE     = 1 << 0,
    ANIMATION   = 1 << 1,
    FONT        = 1 << 2,
    SOUND       = 1 << 3
};

static std::unordered_map<std::string, AssetTag> const strAssetTagMap = {
    { "Undefined",  AssetTag::UNDEFINED },
    { "Texture",    AssetTag::TEXTURE },
    { "Animation",  AssetTag::ANIMATION },
    { "Font",       AssetTag::FONT },
    { "Sound",      AssetTag::SOUND }
};

static std::unordered_map<AssetTag, std::string> const assetTagStrMap = {
    { AssetTag::UNDEFINED,  "Undefined" },
    { AssetTag::TEXTURE,    "Texture" },
    { AssetTag::ANIMATION,  "Animation" },
    { AssetTag::FONT,       "Font" },
    { AssetTag::SOUND,      "Sound" }
};

enum class PlayerState : uint32_t {
    UNDEFINED       = 0,
    STAND_DOWN      = 1 << 0,
    STAND_UP        = 1 << 1,
    STAND_RIGHT     = 1 << 2,
    RUN_DOWN        = 1 << 3,
    RUN_UP          = 1 << 4,
    RUN_RIGHT       = 1 << 5,
    ATTACK_DOWN     = 1 << 6,
    ATTACK_UP       = 1 << 7,
    ATTACK_RIGHT    = 1 << 8
};

static std::unordered_map<std::string, PlayerState> const strPlayerStateMap = {
    { "Undefined",      PlayerState::UNDEFINED },
    { "StandDown",      PlayerState::STAND_DOWN },
    { "StandUp",        PlayerState::STAND_UP },
    { "StandRight",     PlayerState::STAND_RIGHT },
    { "RunDown",        PlayerState::RUN_DOWN },
    { "RunUp",          PlayerState::RUN_UP },
    { "RunRight",       PlayerState::RUN_RIGHT },
    { "AtkDown",        PlayerState::ATTACK_DOWN },
    { "AtkUp",          PlayerState::ATTACK_UP },
    { "AtkRight",       PlayerState::ATTACK_RIGHT }
};

static std::unordered_map<PlayerState, std::string> const playerStateStrMap = {
    { PlayerState::UNDEFINED,       "Undefined" },
    { PlayerState::STAND_DOWN,      "StandDown" },
    { PlayerState::STAND_UP,        "StandUp" },
    { PlayerState::STAND_RIGHT,     "StandRight" },
    { PlayerState::RUN_DOWN,        "RunDown" },
    { PlayerState::RUN_UP,          "RunUp" },
    { PlayerState::RUN_RIGHT,       "RunRight" },
    { PlayerState::ATTACK_DOWN,     "AtkDown" },
    { PlayerState::ATTACK_UP,       "AtkUp" },
    { PlayerState::ATTACK_RIGHT,    "AtkRight" }
};

enum class SceneMenuAction : uint32_t {
    UNDEFINED = 0,
    UP = 1 << 0,
    DOWN = 1 << 1,
    PLAY = 1 << 2,
    QUIT = 1 << 3
};

static std::unordered_map<std::string, SceneMenuAction> const strSceneMenuActionMap = {
    { "Undefined",  SceneMenuAction::UNDEFINED },
    { "Up",         SceneMenuAction::UP },
    { "Down",       SceneMenuAction::DOWN },
    { "Play",       SceneMenuAction::PLAY },
    { "Quit",       SceneMenuAction::QUIT }
};

static std::unordered_map<SceneMenuAction, std::string> const sceneMenuActionStrMap = {
    { SceneMenuAction::UNDEFINED,   "Undefined" },
    { SceneMenuAction::UP,          "Up" },
    { SceneMenuAction::DOWN,        "Down" },
    { SceneMenuAction::PLAY,        "Play" },
    { SceneMenuAction::QUIT,        "Quit" }
};

enum class ScenePlayAction : uint32_t {
    UNDEFINED           = 0,
    UP                  = 1 << 0,
    DOWN                = 1 << 1,
    PAUSE               = 1 << 2,
    QUIT                = 1 << 3,
    TOGGLE_TEXTURE      = 1 << 4,
    TOGGLE_COLLISION    = 1 << 5,
    TOGGLE_GRID         = 1 << 6,
    LEFT                = 1 << 7,
    RIGHT               = 1 << 8,
    ATTACK              = 1 << 9,
    TOGGLE_FOLLOW       = 1 << 10
};

static std::unordered_map<std::string, ScenePlayAction> const strScenePlayActionMap = {
    { "Undefined",          ScenePlayAction::UNDEFINED },
    { "Up",                 ScenePlayAction::UP },
    { "Down",               ScenePlayAction::DOWN },
    { "Pause",              ScenePlayAction::PAUSE },
    { "Quit",               ScenePlayAction::QUIT },
    { "Toggle_texture",     ScenePlayAction::TOGGLE_TEXTURE },
    { "Toggle_collision",   ScenePlayAction::TOGGLE_COLLISION },
    { "Toggle_grid",        ScenePlayAction::TOGGLE_GRID },
    { "Toggle_follow",      ScenePlayAction::TOGGLE_FOLLOW },
    { "Left",               ScenePlayAction::LEFT },
    { "Right",              ScenePlayAction::RIGHT },
    { "Attack",             ScenePlayAction::ATTACK },
};

static std::unordered_map<ScenePlayAction, std::string> const scenePlayActionStrMap = {
    { ScenePlayAction::UNDEFINED,           "Undefined" },
    { ScenePlayAction::UP,                  "Up" },
    { ScenePlayAction::DOWN,                "Down" },
    { ScenePlayAction::PAUSE,               "Pause" },
    { ScenePlayAction::QUIT,                "Quit" },
    { ScenePlayAction::TOGGLE_TEXTURE,      "Toggle_texture" },
    { ScenePlayAction::TOGGLE_COLLISION,    "Toggle_collision" },
    { ScenePlayAction::TOGGLE_GRID,         "Toggle_grid" },
    { ScenePlayAction::TOGGLE_FOLLOW,       "Toggle_follow" },
    { ScenePlayAction::LEFT,                "Left" },
    { ScenePlayAction::RIGHT,               "Right" },
    { ScenePlayAction::ATTACK,              "Attack" }
};

enum class ActionType : uint32_t {
    UNDEFINED   = 0,
    START       = 1 << 0,
    END         = 1 << 1
};

static std::unordered_map<std::string, ActionType> const strActionTypeMap = {
    { "Undefined",  ActionType::UNDEFINED },
    { "Start",      ActionType::START},
    { "End",        ActionType::END }
};

static std::unordered_map<ActionType, std::string> const actionTypeStrMap = {
    { ActionType::UNDEFINED,    "Undefined" },
    { ActionType::START,        "Start" },
    { ActionType::END,          "End" }
};

enum class AnimationTag : uint64_t {
    UNDEFINED       = 0ULL,
    EXPLOSION       = 1ULL << 0ULL,
    RUN_DOWN        = 1ULL << 1ULL,
    RUN_RIGHT       = 1ULL << 2ULL,
    RUN_UP          = 1ULL << 3ULL,
    STAND_DOWN      = 1ULL << 4ULL,
    STAND_RIGHT     = 1ULL << 5ULL,
    STAND_UP        = 1ULL << 6ULL,
    TEKTITE         = 1ULL << 7ULL,
    KNIGHT          = 1ULL << 8ULL,
    ATK_UP          = 1ULL << 9ULL,
    ATK_DOWN        = 1ULL << 10ULL,
    ATK_RIGHT       = 1ULL << 11ULL,
    SWORD_UP        = 1ULL << 12ULL,
    SWORD_RIGHT     = 1ULL << 13ULL,
    BLACK           = 1ULL << 14ULL,
    ROCK_TL         = 1ULL << 15ULL,
    ROCK_TM         = 1ULL << 16ULL,
    ROCK_TR         = 1ULL << 17ULL,
    ROCK_BL         = 1ULL << 18ULL,
    ROCK_BM         = 1ULL << 19ULL,
    ROCK_BR         = 1ULL << 20ULL,
    WATER_TL        = 1ULL << 21ULL,
    WATER_TM        = 1ULL << 22ULL,
    WATER_TR        = 1ULL << 23ULL,
    WATER_ML        = 1ULL << 24ULL,
    WATER_MM        = 1ULL << 25ULL,
    WATER_MR        = 1ULL << 26ULL,
    WATER_BL        = 1ULL << 27ULL,
    WATER_BM        = 1ULL << 28ULL,
    WATER_BR        = 1ULL << 29ULL,
    TREE_TL         = 1ULL << 30ULL,
    TREE_TM         = 1ULL << 31ULL,
    TREE_TR         = 1ULL << 32ULL,
    TREE_BL         = 1ULL << 33ULL,
    TREE_BR         = 1ULL << 34ULL,
    BUSH            = 1ULL << 35ULL,
    HEART           = 1ULL << 36ULL
};

static std::unordered_map<std::string, AnimationTag> const strAnimationTagMap = {
    { "Undefined",  AnimationTag::UNDEFINED },
    { "Explosion",  AnimationTag::EXPLOSION },
    { "RunDown",    AnimationTag::RUN_DOWN },
    { "RunRight",   AnimationTag::RUN_RIGHT },
    { "RunUp",      AnimationTag::RUN_UP },
    { "StandDown",  AnimationTag::STAND_DOWN },
    { "StandRight", AnimationTag::STAND_RIGHT },
    { "StandUp",    AnimationTag::STAND_UP },
    { "Tektite",    AnimationTag::TEKTITE },
    { "Knight",     AnimationTag::KNIGHT },
    { "AtkUp",      AnimationTag::ATK_UP },
    { "AtkDown",    AnimationTag::ATK_DOWN },
    { "AtkRight",   AnimationTag::ATK_RIGHT },
    { "SwordUp",    AnimationTag::SWORD_UP },
    { "SwordRight", AnimationTag::SWORD_RIGHT },
    { "Black",      AnimationTag::BLACK },
    { "RockTL",     AnimationTag::ROCK_TL },
    { "RockTM",     AnimationTag::ROCK_TM },
    { "RockTR",     AnimationTag::ROCK_TR },
    { "RockBL",     AnimationTag::ROCK_BL },
    { "RockBM",     AnimationTag::ROCK_BM},
    { "RockBR",     AnimationTag::ROCK_BR},
    { "WaterTL",    AnimationTag::WATER_TL },
    { "WaterTM",    AnimationTag::WATER_TM },
    { "WaterTR",    AnimationTag::WATER_TR },
    { "WaterML",    AnimationTag::WATER_ML },
    { "WaterMM",    AnimationTag::WATER_MM},
    { "WaterMR",    AnimationTag::WATER_MR},
    { "WaterBL",    AnimationTag::WATER_BL },
    { "WaterBM",    AnimationTag::WATER_BM},
    { "WaterBR",    AnimationTag::WATER_BR},
    { "TreeTL",     AnimationTag::TREE_TL},
    { "TreeTM",     AnimationTag::TREE_TM},
    { "TreeTR",     AnimationTag::TREE_TR },
    { "TreeBL",     AnimationTag::TREE_BL},
    { "TreeBR",     AnimationTag::TREE_BR},
    { "Bush",       AnimationTag::BUSH},
    { "Heart",      AnimationTag::HEART},

};

static std::unordered_map<AnimationTag, std::string> const animationTagStrMap = {
    { AnimationTag::UNDEFINED,      "Undefined" },
    { AnimationTag::EXPLOSION,      "Explosion" },
    { AnimationTag::RUN_DOWN,       "RunDown" },
    { AnimationTag::RUN_RIGHT,      "RunRight" },
    { AnimationTag::RUN_UP,         "RunUp" },
    { AnimationTag::STAND_DOWN,     "StandDown" },
    { AnimationTag::STAND_RIGHT,    "StandRight" },
    { AnimationTag::STAND_UP,       "StandUp" },
    { AnimationTag::TEKTITE,        "Tektite" },
    { AnimationTag::KNIGHT,         "Knight" },
    { AnimationTag::ATK_UP,         "AtkUp" },
    { AnimationTag::ATK_DOWN,       "AtkDown" },
    { AnimationTag::ATK_RIGHT,      "AtkRight" },
    { AnimationTag::SWORD_UP,       "SwordUp" },
    { AnimationTag::SWORD_RIGHT,    "SwordRight" },
    { AnimationTag::BLACK,          "Black" },
    { AnimationTag::ROCK_TL,        "RockTL" },
    { AnimationTag::ROCK_TM,        "RockTM" },
    { AnimationTag::ROCK_TR,        "RockTR" },
    { AnimationTag::ROCK_BL,        "RockBL" },
    { AnimationTag::ROCK_BM,        "RockBM" },
    { AnimationTag::ROCK_BR,        "RockBR" },
    { AnimationTag::WATER_TL,       "WaterTL" },
    { AnimationTag::WATER_TM,       "WaterTM" },
    { AnimationTag::WATER_TR,       "WaterTR" },
    { AnimationTag::WATER_ML,       "WaterML" },
    { AnimationTag::WATER_MM,       "WaterMM" },
    { AnimationTag::WATER_MR,       "WaterMR" },
    { AnimationTag::WATER_BL,       "WaterBL" },
    { AnimationTag::WATER_BM,       "WaterBM" },
    { AnimationTag::WATER_BR,       "WaterBR" },
    { AnimationTag::TREE_TL,        "TreeTL" },
    { AnimationTag::TREE_TM,        "TreeTM" },
    { AnimationTag::TREE_TR,        "TreeTR" },
    { AnimationTag::TREE_BL,        "TreeBL" },
    { AnimationTag::TREE_BR,        "TreeBR" },
    { AnimationTag::BUSH,           "Bush" },
    { AnimationTag::HEART,          "Heart" }
};

enum class AIType : uint32_t {
    UNDEFINED = 0,
    FOLLOW = 1 << 0,
    PATROL = 1 << 1
};

static std::unordered_map<std::string, AIType> const strAITypeMap = {
    { "Undefined",  AIType::UNDEFINED },
    { "Follow",     AIType::FOLLOW },
    { "Patrol",     AIType::PATROL }
};

static std::unordered_map<AIType, std::string> const AITypeStrMap = {
    { AIType::UNDEFINED,    "Undefined" },
    { AIType::FOLLOW,       "Follow" },
    { AIType::PATROL,       "Patrol" }
};

enum class SoundTag : uint32_t {
    UNDEFINED   = 0,
    SWORD_SLASH = 1 << 0,
    LINK_HURT   = 1 << 1,
    LINK_DIE    = 1 << 2,
    ENEMY_HIT   = 1 << 3,
    ENEMY_DIE   = 1 << 4,
    GET_ITEM    = 1 << 5,
    MUSIC_TITLE = 1 << 6,
    MUSIC_PLAY  = 1 << 7
};

static std::unordered_map<std::string, SoundTag> const strSoundTagMap = {
    { "Undefined",  SoundTag::UNDEFINED },
    { "SwordSlash", SoundTag::SWORD_SLASH },
    { "LinkHurt",   SoundTag::LINK_HURT },
    { "LinkDie",    SoundTag::LINK_DIE },
    { "EnemyHit",   SoundTag::ENEMY_HIT },
    { "EnemyDie",   SoundTag::ENEMY_DIE },
    { "GetItem",    SoundTag::GET_ITEM },
    { "MusicTitle", SoundTag::MUSIC_TITLE },
    { "MusicPlay",  SoundTag::MUSIC_PLAY }
};

static std::unordered_map<SoundTag, std::string> const soundTagStrMap = {
    { SoundTag::UNDEFINED,      "Undefined" },
    { SoundTag::SWORD_SLASH,    "SwordSlash" },
    { SoundTag::LINK_HURT,      "LinkHurt" },
    { SoundTag::LINK_DIE,       "LinkDie" },
    { SoundTag::ENEMY_HIT,      "EnemyHit" },
    { SoundTag::ENEMY_DIE,      "EnemyDie" },
    { SoundTag::GET_ITEM,       "GetItem" },
    { SoundTag::MUSIC_TITLE,    "MusicTitle" },
    { SoundTag::MUSIC_PLAY,     "MusicPlay" }
};

using EntityTagMask         = EnumMask<EntityTag>;
using AssetTagMask          = EnumMask<AssetTag>;
using PlayerStateMask       = EnumMask<PlayerState>;
using SceneMenuActionMask   = EnumMask<SceneMenuAction>;
using ScenePlayActionMask   = EnumMask<ScenePlayAction>;
using ActionTypeMask        = EnumMask<ActionType>;
using SceneTagMask          = EnumMask<SceneTag>;
using AnimationTagMask      = EnumMask<AnimationTag>;
using AITypeMask            = EnumMask<AIType>;
using SoundTagMask          = EnumMask<SoundTag>;

#endif // !ENUMS_H