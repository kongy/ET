#include "idtable.hpp"

IDTable::IDTable()
{
	id_table = new vector<pair<Variable *, LogicStatement *> *>();
}

bool IDTable::add(Variable *key, LogicStatement *value) {
	for (pair<Variable *, LogicStatement *> *keyValuePair : *id_table) {
		if (key->equals(keyValuePair->first))
			return value->equals(keyValuePair->second);
	}

	id_table->push_back(new pair<Variable *, LogicStatement *>(key, value));
	return true;
}

vector<pair<Variable *, LogicStatement *> *> *IDTable::getIdTable() {
	return id_table;
}

void IDTable::clear() {
	for (pair<Variable *, LogicStatement *> *pairs : *id_table)
		delete pairs;

	id_table->clear();
}
