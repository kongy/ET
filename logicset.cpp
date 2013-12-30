#include "logicset.hpp"

bool LogicSet::inSet(Rule *item) {
	for (Rule *set_elem : *set)
		if (item->equals(set_elem))
			return true;

	return false;
}

bool LogicSet::add(Rule *item) {
	if (inSet(item))
		return false;
	else {
		set->push_back(item);
		return true;
	}
}

LogicSet::LogicSet() {
	set = new QVector<Rule *>();
}

LogicSet::~LogicSet() {
	delete set;
}

QVector<Rule *> *LogicSet::getSet() {
	return set;
}

LogicSet *LogicSet::diff(Rule *item) {

	LogicSet *remaining_set = new LogicSet();

	for (Rule *statement : *set)
		if (!statement->equals(item))
			remaining_set->add(statement);

	return remaining_set;
}
