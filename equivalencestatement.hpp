#ifndef EQUIVALENCESTATEMENT_HPP
#define EQUIVALENCESTATEMENT_HPP

#include <utility>
#include "AST.hpp"
#include "logicset.hpp"
#include "idtable.hpp"
using namespace std;
using namespace AST;

class EquivalenceStatement
{
private:
	LogicStatement *base_statement;
	IDTable *id_table;
	LogicSet *rule_set;
public:
	EquivalenceStatement(LogicStatement *base, IDTable *id_table, LogicSet *rule_set);
	~EquivalenceStatement();
	LogicStatement *getBaseStatement();
	IDTable *getIdTable();
	LogicSet *getEquivalentRules();
};

#endif // EQUIVALENCESTATEMENT_HPP
