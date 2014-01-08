#ifndef IDTABLE_HPP
#define IDTABLE_HPP

#include <QPair>
#include "AST.hpp"

namespace AST {
class Variable;

class LogicStatement;
}

class IDTable
{
private:
	QVector<QPair<AST::Variable *, AST::LogicStatement *> *> *id_table;
public:
	IDTable();

	/* Adding a key value pair that maps identifiers for rules to the filtered
	 * version of the logicstatement which user provided */
	bool add(AST::Variable *key, AST::LogicStatement *value);

	/* Returns the vector of entire mapping from identifier to LogicStatement */
	QVector<QPair<AST::Variable *, AST::LogicStatement *> *> *getMapping();

	/* Provided a key, returns the corresponding Logicstatement or null if
	 * none found */
	AST::LogicStatement *valueOf(AST::Variable *key);

	/* Free up the space allocated by Pairs and empty the mapping table */
	void clear();
};

#endif // IDTABLE_HPP
