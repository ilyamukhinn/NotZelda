#ifndef ACTION_H
#define ACTION_H

#include "Enums.hpp"
#include <string>

class Action {
	uint32_t	m_name{ 0 };
	ActionType	m_type{ ActionType::UNDEFINED };
public:
	Action() = default;
	Action(const uint32_t& name, const ActionType& type)
		: m_name(name), m_type(type) {};

	const uint32_t& name() const;
	const ActionType& type() const;
};

#endif // !ACTION_H
