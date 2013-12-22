#ifndef EQUIVALENCESTATEMENT_HPP
#define EQUIVALENCESTATEMENT_HPP

#include <utility>
#include "AST.hpp"
#include "logicset.hpp"
#include "idtable.hpp"
using namespace AST;

class EquivalenceStatement
{
private:
	LogicStatement *base_statement;
	IDTable *id_table;
	LogicSet *rule_set;
public:
	EquivalenceStatement(LogicStatement *base, IDTable *id_table,
						 LogicSet *rule_set);
	~EquivalenceStatement();

	/* Returns the matched pattern for instance A->B ¬AVB returns A->B
	 * for display purposes */
	LogicStatement *getBaseStatement();

	/* Returns the matching symbol(Identifier in this case) with the
	 * relevant AST tree to swap */
	IDTable *getIdTable();

	/* A Set of equivalences that matched the base rule, call getSet()
	 * to return the vector, for each individual rule, call
	 * rule.clone().replace(getIdTable()) to return the replaced
	 * statement */
	LogicSet *getEquivalentRules();
};

#endif // EQUIVALENCESTATEMENT_HPP
