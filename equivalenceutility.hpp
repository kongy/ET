#ifndef EQUIVALENCEUTILITY_HPP
#define EQUIVALENCEUTILITY_HPP

#include "idtable.hpp"
#include "logicset.hpp"

class LogicSet;
class IDTable;

namespace AST
{
class Variable;
}

using namespace AST;

class EquivalenceUtility
{
	/* Existing bindings from Variable in rules to LogicStatement in input */
	IDTable *idTable;

	/* A List containing same variable but at different places, used for rename
	 */
	QVector<Variable *> *freeVariableList;

	/* Collecting different possible x's in A in a Rule like
	 * A=FORALL(x)A=THEREEXISTS(x)A where x does not occur free in A and x is
	 * not matched */
	LogicSet *candidateBoundVariableSet;

	/* An identifier for potential candidate for candidateBoundVariableSet,
	 * usually collected and matched later on */
	Variable *boundedVariable;

  public:
	EquivalenceUtility();
	~EquivalenceUtility();
	void initFreeVariableList();
	void initCandidateBoundVariableSet(Variable *boundedVariable);
	QVector<Variable *> *getFreeVariableList();
	Variable *getBoundVariable();
	LogicSet *getCandidateBoundVariableSet();
	IDTable *getIDTable();
	void resetAuxiliaryItems();
	void deleteAuxiliaryItems();
	void clear();
	bool inCandidateBoundVariableSet(Variable *var);
	bool inFreeVariableListReferencewise(Variable *var);
};

#endif // EQUIVALENCEUTILITY_HPP
