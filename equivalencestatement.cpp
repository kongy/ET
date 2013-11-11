#include "equivalencestatement.hpp"

EquivalenceStatement::EquivalenceStatement(LogicStatement *base, IDTable *id_table, LogicSet *rule_set) {
	this->base_statement = base;
	this->id_table = id_table;
	this->rule_set = rule_set;
}

EquivalenceStatement::~EquivalenceStatement() {
	delete id_table;
	delete rule_set;
}

LogicStatement *EquivalenceStatement::getBaseStatement() {
	return base_statement;
}

IDTable *EquivalenceStatement::getIdTable() {
	return id_table;
}

LogicSet *EquivalenceStatement::getEquivalentRules() {
	return rule_set;
}
