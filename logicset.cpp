#include "logicset.hpp"

bool LogicSet::inSet(LogicStatement *item) {
	for (LogicStatement *set_elem : *set)
		if (item->equals(set_elem))
			return true;

	return false;
}

bool LogicSet::add(LogicStatement *item) {
	if (inSet(item))
		return false;
	else {
		set->push_back(item);
		return true;
	}
}

LogicSet::LogicSet() {
	set = new vector<LogicStatement *>();
}

LogicSet::~LogicSet() {
	delete set;
}

vector<LogicStatement *> *LogicSet::getSet() {
	return set;
}

LogicSet *LogicSet::diff(LogicStatement *item) {

	LogicSet *remaining_set = new LogicSet();

	for (LogicStatement *statement : *set)
		if (!statement->equals(item))
			remaining_set->add(statement);

	return remaining_set;
}
