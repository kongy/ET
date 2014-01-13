#include "logicset.hpp"
#include "symbol.hpp"

using namespace AST;

bool LogicSet::isEmpty() {
	return getSet()->isEmpty();
}

bool LogicSet::contains(Rule *item) {
	for (Rule *set_elem : *set)
		if (item->equals(set_elem))
			return true;

	return false;
}

bool LogicSet::add(Rule *item) {
	if (!contains(item)) {
		set->push_back(item);
		return true;
	}

	return false;
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

	if (!isEmpty())
		for (Rule *statement : *set)
			if (!statement->equals(item))
				remaining_set->add(statement);

	return remaining_set;
}

LogicSet *LogicSet::diff(LogicSet *other) {

	LogicSet *difference = new LogicSet();
	QVector<LogicStatement *> *currentSet = getSet();

	if (!currentSet->isEmpty()) {
		if (other->isEmpty()) {
			for(Rule *statement : *currentSet)
				difference->add(statement);
		} else {
			for (Rule *statement : *currentSet)
				if (!other->contains(statement))
					difference->add(statement);
		}
	}

	return difference;

}

QString LogicSet::print(bool fullBracket) {
	QString s;
	for(QVector<Rule*>::const_iterator it = set->cbegin(); it != set->cend(); it++) {
		s += (*it)->print(fullBracket);
		if(it < set->end() - 1) {
			s += SYMBOL_EQUIVALENT;
		}
	}
	return s;
}

void LogicSet::deepDeleteContent() {
	if (!isEmpty())
		for (Rule *rule : *set)
			delete rule;
}

bool LogicSet::equals(LogicSet *other) {
	if (set->size() != other->getSet()->size())
		return false;

	for (Rule *statement : *set)
		if (!other->contains(statement))
			return false;

	return true;
}

bool LogicSet::contains(LogicSet *other) {
	QVector<LogicStatement *> *otherSet = other->getSet();

	/* The current set cannot contain other set if it has less element than otherset */
	if (set->size() < otherSet->size())
		return false;

	for (Rule *otherRule : *otherSet)
		if (!this->contains(otherRule))
			return false;

	return true;

}

void LogicSet::add(LogicSet *other) {
	for (Rule *newRule : *other->getSet())
		add(newRule);
}

void LogicSet::setComment(QString ruleComment) {
	comment = ruleComment;
}

QString LogicSet::getComment() {
	return comment;
}
