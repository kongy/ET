#include "equivalenceutility.hpp"

EquivalenceUtility::EquivalenceUtility() {
    idTable = new IDTable();
    resetAuxiliaryItems();
}

EquivalenceUtility::~EquivalenceUtility() {
    delete idTable;
    deleteAuxiliaryItems();
}

void EquivalenceUtility::deleteAuxiliaryItems() {

    if (candidateBoundVariableSet != nullptr)
        delete candidateBoundVariableSet;

    if (freeVariableList != nullptr)
        delete freeVariableList;
}

void EquivalenceUtility::resetAuxiliaryItems() {
    candidateBoundVariableSet = nullptr;
    freeVariableList = nullptr;
    boundedVariable = nullptr;
}

void EquivalenceUtility::initCandidateBoundVariableSet(Variable *boundedVariable) {
    this->boundedVariable = boundedVariable;
    if (candidateBoundVariableSet == nullptr)
        candidateBoundVariableSet = new LogicSet();
}

void EquivalenceUtility::initFreeVariableList() {
    if (freeVariableList == nullptr)
        freeVariableList = new QVector<Variable *>();
}

Variable *EquivalenceUtility::getBoundVariable() {
    return boundedVariable;
}

IDTable *EquivalenceUtility::getIDTable() {
    return idTable;
}

LogicSet *EquivalenceUtility::getCandidateBoundVariableSet() {
    return candidateBoundVariableSet;
}

QVector<Variable *> *EquivalenceUtility::getFreeVariableList() {
    return freeVariableList;
}

void EquivalenceUtility::clear() {
    idTable->clear();

    deleteAuxiliaryItems();
    resetAuxiliaryItems();
}

bool EquivalenceUtility::inCandidateBoundVariableSet(Variable *var) {
    return candidateBoundVariableSet->exists(var);
}
