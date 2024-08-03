#include "Action.hpp"

#include <sstream>

const uint32_t& Action::name() const {
	return this->m_name;
}

const ActionType& Action::type() const {
	return this->m_type;
}