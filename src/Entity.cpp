#include "Entity.hpp"

bool Entity::isActive() const {
	return this->m_active;
}

void Entity::destroy() {
	this->m_active = false;
}

size_t Entity::id() const {
	return this->m_id;
}

const EntityTag& Entity::tag() const {
	return this->m_tag;
}