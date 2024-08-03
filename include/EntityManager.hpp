#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entity.hpp"
#include "Enums.hpp"

#include <vector>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
    EntityVec                           m_entities;
    EntityVec                           m_entitiesToAdd;
    std::map<EntityTag, EntityVec>      m_entityMap;
    size_t                              m_totalEntities{ 0 };

    void removeDeadEntities(EntityVec& vec);

public:

    EntityManager() = default;

    void update();

    std::shared_ptr<Entity> addEntity(const EntityTag& tag);

    const EntityVec& getEntities();
    const EntityVec& getEntities(const EntityTag& tag);
    EntityVec getEntities(const EntityTagMask& tagMask);
    std::map<EntityTag, EntityVec>& getEntityMap();

    void concatVec(EntityVec& firstVec, const EntityVec& secondVec);
};

#endif // !ENTITY_MANAGER_H
