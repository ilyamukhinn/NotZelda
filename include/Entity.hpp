#ifndef ENTITY_H
#define ENTITY_H

#include "Components.hpp"
#include "Enums.hpp"

#include <tuple>
#include <string>

class EntityManager;

typedef std::tuple<
    CTransform,
    CLifeSpan,
    CInput,
    CBoundingBox,
    CAnimation,
    CInvincibility,
    CState,
    CHealth,
    CDamage,
    CPatrol,
    CFollowPlayer
> ComponentTuple;

class Entity {
    friend class EntityManager;

    bool                m_active{ true };
    EntityTag           m_tag{ EntityTag::UNDEFINED };
    size_t              m_id{ 0 };
    ComponentTuple      m_components;

    Entity(size_t id, const EntityTag& tag)
        : m_id(id), m_tag(tag) {};

public:

    void                    destroy();
    size_t                  id()                const;
    bool                    isActive()          const;
    const EntityTag&        tag()               const;


    template <typename T>
    bool has() const {
        return this->get<T>().has;
    }

    template <typename T, typename... TArgs>
    T& add(TArgs&&... mArgs) {
        auto& component = this->get<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template<typename T>
    T& get() {
        return std::get<T>(this->m_components);
    }

    template<typename T>
    const T& get() const {
        return std::get<T>(this->m_components);
    }

    template<typename T>
    void remove() {
        this->get<T>() = T();
    }
};



#endif // !ENTITY_H
