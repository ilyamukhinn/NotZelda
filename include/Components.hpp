#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Animation.hpp"
#include "Assets.hpp"
#include "Enums.hpp"

class Component {
public:
    bool has{ false };
    Component() = default;
};

class CTransform : public Component {
public:
    Vec2f m_pos     { 0.0, 0.0 };
    Vec2f m_prevPos { 0.0, 0.0 };
    Vec2f m_scale   { 1.0, 1.0 };
    Vec2f m_velocity{ 0.0, 0.0 };
    // Vec2f m_facing  { 0.0f, 1.0f };
    float m_angle   { 0.0f };

    CTransform() = default;
    CTransform(const Vec2f& p)
        : m_prevPos(p), m_pos(p) {}
    CTransform(const Vec2f& p, const Vec2f& sc)
        : m_prevPos(p), m_pos(p), m_scale(sc) {}
    CTransform(const Vec2f& p, const Vec2f& v, const Vec2f& sc, float a)
        : m_pos(p), m_prevPos(p), m_velocity(v), m_scale(sc), m_angle(a) {}

};

class CLifeSpan : public Component {
public:
    int m_remaining{ 0 };
    int m_total{ 0 };

    CLifeSpan() = default;
    CLifeSpan(int total)
        : m_remaining(total), m_total(total) {}
};

class CDamage : public Component {
public:
    int m_damage{ 1 };

    CDamage() = default;
    CDamage(int d)
        : m_damage(d) {}
};

class CInput : public Component {
public:
    bool m_up       { false };
    bool m_down     { false };
    bool m_left     { false };
    bool m_right    { false };
    bool m_attack   { false };

    CInput() = default;
};

class CBoundingBox : public Component {
public:
    Vec2f   m_size          { 0.0f, 0.0f };
    Vec2f   m_halfSize      { 0.0f, 0.0f };
    bool    m_blockMove     { false };
    bool    m_blockVision   { false };

    CBoundingBox() = default;
    CBoundingBox(const Vec2f& s)
        : m_size(s), m_halfSize(s.x / 2, s.y / 2) {}
    CBoundingBox(const Vec2f& s, bool bm, bool bv)
        : m_size(s), m_halfSize(s.x / 2, s.y / 2), m_blockMove(bm), m_blockVision(bv) {}
};

class CAnimation : public Component {
public:
    Animation   m_animation;
    bool        m_repeat        { false };

    CAnimation() = default;
    CAnimation(const Animation& animation, bool r)
        : m_animation(animation), m_repeat(r) {}
};

class CInvincibility : public Component {
public:
    int m_iFrames{ 0 };

    CInvincibility() = default;
    CInvincibility(int f)
        : m_iFrames(f) {}
};

class CState : public Component {
public:
    PlayerState m_prevState { PlayerState::STAND_DOWN };
    PlayerState m_state     { PlayerState::STAND_DOWN };

    CState() = default;
    CState(const PlayerState& s)
        : m_prevState(s), m_state(s) {}
};

class CHealth : public Component {
public:
    int m_max       { 1 };
    int m_current   { 1 };

    CHealth() = default;
    CHealth(int m)
        : m_max(m), m_current(m) {}
};

class CPatrol : public Component {
public:
    std::vector<Vec2f>  m_positions;
    size_t              m_currentPosition   { 0 };
    std::vector<Vec2f>  m_velocities;
    float               m_speed             { 0.0f };

    CPatrol() = default;
    CPatrol(const std::vector<Vec2f>& pos, const std::vector<Vec2f>& v, float s)
        : m_positions(pos), m_velocities(v), m_speed(s) {}
};

class CFollowPlayer : public Component {
public:
    Vec2f   m_home;
    float   m_speed         { 0.0f };
    float   m_velocityScale { 0.3f };

    CFollowPlayer() = default;
    CFollowPlayer(const Vec2f& p, float s, float vs = 0.3f)
        : m_home(p), m_speed(s), m_velocityScale(vs) {}
};


#endif // !COMPONENTS_H
