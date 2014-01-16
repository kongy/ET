#include "equivalenceutility.hpp"

EquivalenceUtility::EquivalenceUtility()
{
	idTable = new IDTable();
	resetAuxiliaryItems();
}

EquivalenceUtility::~EquivalenceUtility()
{
	delete idTable;
	deleteAuxiliaryItems();
}

void EquivalenceUtility::deleteAuxiliaryItems()
{

	if (rejectionBoundVariableSet != nullptr)
		delete rejectionBoundVariableSet;

	if (freeVariableList != nullptr)
		delete freeVariableList;
}

void EquivalenceUtility::resetAuxiliaryItems()
{
	rejectionBoundVariableSet = nullptr;
	freeVariableList = nullptr;
	boundedVariable = nullptr;
}

void
EquivalenceUtility::initRejectionBoundVariableSet(Variable *boundedVariable)
{
	this->boundedVariable = boundedVariable;
	if (rejectionBoundVariableSet == nullptr)
		rejectionBoundVariableSet = new LogicSet();
}

void EquivalenceUtility::initFreeVariableList()
{
	if (freeVariableList == nullptr)
		freeVariableList = new QVector<Variable *>();
}

Variable *EquivalenceUtility::getBoundVariable()
{
	return boundedVariable;
}

IDTable *EquivalenceUtility::getIDTable()
{
	return idTable;
}

LogicSet *EquivalenceUtility::getRejectionBoundVariableSet()
{
	return rejectionBoundVariableSet;
}

QVector<Variable *> *EquivalenceUtility::getFreeVariableList()
{
	return freeVariableList;
}

void EquivalenceUtility::clear()
{
	idTable->clear();

	deleteAuxiliaryItems();
	resetAuxiliaryItems();
}

bool EquivalenceUtility::inRejectionBoundVariableSet(Variable *var)
{
	return rejectionBoundVariableSet->contains(var);
}

bool EquivalenceUtility::inFreeVariableListReferencewise(Variable *var)
{
	for (Variable *variable : *getFreeVariableList())
		if (variable == var)
			return true;

	return false;
}
