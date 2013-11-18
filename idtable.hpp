#ifndef IDTABLE_HPP
#define IDTABLE_HPP

class IDTable;

#include "AST.hpp"
#include <utility>
using namespace AST;
using namespace std;

class IDTable
{
private:
	vector<pair<Variable *, LogicStatement *> *> *id_table;
public:
	IDTable();
	bool add(Variable *key, LogicStatement *value);
	vector<pair<Variable *, LogicStatement *> *> *getIdTable();
    LogicStatement *valueOf(Variable *key);
	void clear();
};

#endif // IDTABLE_HPP
