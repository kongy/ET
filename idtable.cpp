#include "idtable.hpp"

IDTable::IDTable()
{
	id_table = new QVector<pair<Variable *, LogicStatement *> *>();
}

bool IDTable::add(Variable *key, LogicStatement *value) {
	for (pair<Variable *, LogicStatement *> *keyValuePair : *id_table) {
		if (key->equals(keyValuePair->first))
			return value->equals(keyValuePair->second);
	}

	id_table->push_back(new pair<Variable *, LogicStatement *>(key, value));
	return true;
}

QVector<pair<Variable *, LogicStatement *> *> *IDTable::getIdTable() {
	return id_table;
}

LogicStatement* IDTable::valueOf(Variable *key) {
    for (pair<Variable *, LogicStatement *> *key_value_pair : *getIdTable())
        if (key_value_pair->first->equals(key))
            return key_value_pair->second->clone();

    return nullptr;
}

void IDTable::clear() {
	for (pair<Variable *, LogicStatement *> *pairs : *id_table)
		delete pairs;

	id_table->clear();
}
