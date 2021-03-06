#include "idtable.hpp"

using namespace AST;

IDTable::IDTable()
{
	id_table = new QVector<QPair<Variable *, LogicStatement *> *>();
}

IDTable::~IDTable() {
	for (QPair<Variable *, LogicStatement *> *keyValuePair : *id_table)
		delete keyValuePair;
}

bool IDTable::add(Variable *key, LogicStatement *value)
{
	for (QPair<Variable *, LogicStatement *> *keyValuePair : *id_table) {
		if (key->equals(keyValuePair->first))
			return value->equals(keyValuePair->second);
	}

	id_table->push_back(new QPair<Variable *, LogicStatement *>(key, value));
	return true;
}

QVector<QPair<Variable *, LogicStatement *> *> *IDTable::getMapping()
{
	return id_table;
}

LogicStatement *IDTable::valueOf(Variable *key)
{
	for (QPair<Variable *, LogicStatement *> *key_value_pair : *getMapping())
		if (key_value_pair->first->equals(key))
			return key_value_pair->second;

	return nullptr;
}

void IDTable::clear()
{
	if (!id_table->isEmpty())
		for (QPair<Variable *, LogicStatement *> *pairs : *id_table)
			delete pairs;

	id_table->clear();
}

void IDTable::deleteValues()
{
	if (!id_table->isEmpty())
		for (QPair<Variable *, LogicStatement *> *pairs : *id_table)
			delete pairs->second;
}
