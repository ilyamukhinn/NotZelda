#include "EntityManager.hpp"

void EntityManager::update() {
    for (auto e : this->m_entitiesToAdd) {
        this->m_entities.push_back(e);
        this->m_entityMap[e->tag()].push_back(e);
    }

    this->m_entitiesToAdd.clear();

    this->removeDeadEntities(this->m_entities);
    for (auto& kv : this->m_entityMap) {
        this->removeDeadEntities(kv.second);
    }
}

void EntityManager::removeDeadEntities(EntityVec& vec) {
    std::erase_if(vec, [](const std::shared_ptr<Entity>& entity) {
        return !entity->isActive();
        }
    );
}

std::shared_ptr<Entity> EntityManager::addEntity(const EntityTag& tag) {
    auto entity = std::shared_ptr<Entity>(new Entity(this->m_totalEntities++, tag));
    this->m_entitiesToAdd.push_back(entity);
    return entity;
}

const EntityVec& EntityManager::getEntities() {
    return m_entities;
}

const EntityVec& EntityManager::getEntities(const EntityTag& tag) {
    return this->m_entityMap[tag];
}

EntityVec EntityManager::getEntities(const EntityTagMask& tagMask) {
	EntityVec res;

    for (const auto& tag : entityTagStrMap) {
        if (tagMask.has(tag.first)) {
            if (this->m_entityMap.find(tag.first) != this->m_entityMap.end()) {
                const auto& bulletVec = this->getEntities(tag.first);
                concatVec(res, bulletVec);
            }
        }
    }

	return res;
}

void EntityManager::concatVec(EntityVec& firstVec, const EntityVec& secondVec) {
	firstVec.insert(firstVec.end(),
		secondVec.begin(),
		secondVec.end());
}