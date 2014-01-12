#include "equivalenceengine.hpp"
#include <QDebug>

EquivalenceEngine::EquivalenceEngine() {
	ruleEngine = new RuleEngine();
	rules = ruleEngine->parseRuleXml();
}

EquivalenceEngine::~EquivalenceEngine() {
	if (rules != nullptr && !rules->isEmpty())
		for (LogicSet *logic_set : *rules) {
			logic_set->deepDeleteContent();
			delete logic_set;
		}

	delete rules;
	delete ruleEngine;
}

EquivalenceUtility *EquivalenceEngine::tryMatchRule(LogicStatement *formula, Rule *rule) {
	EquivalenceUtility *matchingUtility = new EquivalenceUtility();

	/* Matched */
	qDebug() << "Matching the rule";
	qDebug() << rule->print(false);

	if (rule->match(formula, matchingUtility)) {
		qDebug() << "Matched";
		return matchingUtility;
	}

	qDebug() << "Mismatched";

	/* Failed to match */
	delete matchingUtility;
	return nullptr;
}

QVector<LogicSet *> *EquivalenceEngine::match(LogicStatement *input) {

	QVector<LogicSet *> *relatedEquivalence = new QVector<LogicSet *>();

	for (LogicSet *ruleSet : *rules)
		if (ruleApplicable(input, ruleSet))
			relatedEquivalence->append(ruleSet);

	return relatedEquivalence;
}

QVector<Rule *> *EquivalenceEngine::getMatchedRules(LogicStatement *input, LogicSet *ruleSet) {
	QVector<Rule *> *matchedRules = new QVector<Rule *>();

	EquivalenceUtility *matchingResult;

	for (Rule *rule : *ruleSet->getSet()) {
		matchingResult = tryMatchRule(input, rule);
		if (matchingResult != nullptr) {
			matchedRules->append(rule);
			delete matchingResult;
		}

		if (rule->isLeibnizRule())
			break;
	}

	return matchedRules;
}

LogicStatement *EquivalenceEngine::replaceStatement(LogicStatement *formula, Rule *baseRule, Rule *transformationRule, SolutionTabWidget *UI) {
	/* A matched leibniz rule does not need further processing */
	if (baseRule->isLeibnizRule()) {
		delete formula;
		return transformationRule->clone();
	}

	EquivalenceUtility *matchingUtility = tryMatchRule(formula, baseRule);
	Variable *boundVariable = matchingUtility->getBoundVariable();
	QVector<Variable *> *freeVariableList = matchingUtility->getFreeVariableList();
	IDTable *idTable = matchingUtility->getIDTable();
	LogicSet *undefinedVariableSet = transformationRule->getExtraVariables(baseRule);
	LogicStatement *result;

	/* Bound Variable Yet to be decided by user */
	if (boundVariable != nullptr) {
		Variable *userdefinedVariable = getQualifiedBoundVariable(formula, boundVariable, UI);
		idTable->add(boundVariable, userdefinedVariable);

		result = transformationRule->clone()->replace(idTable);
		delete userdefinedVariable;
		delete formula;
		delete matchingUtility;
		delete undefinedVariableSet;

		return result;

	/* Variable Rename */
	} else if (freeVariableList != nullptr) {


		Variable *userdefinedVariable = getQualifiedRenameVariable(formula, freeVariableList->front(), UI);
		QString *newName = new QString(userdefinedVariable->getName());

		for (Variable *freeVariable : *freeVariableList)
			freeVariable->setName(newName);

		delete newName;
		delete userdefinedVariable;
		delete matchingUtility;
		delete undefinedVariableSet;

		return formula;
	}

	/* Further input for patterns required */
	if (!undefinedVariableSet->isEmpty()) {
		bool firstOrder = baseRule->isFirstOrderLogic() || transformationRule->isFirstOrderLogic();
		Variable *identifier;

		for (LogicStatement *id : *undefinedVariableSet->getSet()) {
			identifier = dynamic_cast<Variable *>(id);

			if (!firstOrder)
				idTable->add(identifier, getAnyFormula(FORMULA_REQUEST_PREFIX, identifier, UI, NO_MESSAGE));
			else
				idTable->add(identifier, getAnyVariable(VARIABLE_REQUEST_PREFIX, identifier, UI, NO_MESSAGE));
		}
	}

	/* No further input for patterns required */
	result = transformationRule->clone()->replace(idTable);
	delete matchingUtility;
	delete formula;
	delete undefinedVariableSet;

	return result;
}

bool EquivalenceEngine::ruleApplicable(LogicStatement *input, LogicSet *ruleSet) {
	EquivalenceUtility *matchingResult;

	for (Rule *rule : *ruleSet->getSet()) {
		matchingResult = tryMatchRule(input, rule);

		if (matchingResult != nullptr) {
			delete matchingResult;
			return true;
		}

		if (rule->isLeibnizRule())
			break;
	}

	return false;
}

bool EquivalenceEngine::isVariable(LogicStatement *statement) {
	return statement->getSymbol() == VARIABLE_SYMBOL;
}

bool EquivalenceEngine::acceptedRenamedVariable(LogicStatement *originalFormula, Variable *userDefinedVariable) {
	return originalFormula->notOccur(userDefinedVariable);
}

bool EquivalenceEngine::acceptedBoundedVariable(LogicStatement *originalFormula, Variable *userDefinedVariable) {
	return originalFormula->variableBounded(userDefinedVariable);
}

Variable *EquivalenceEngine::getQualifiedRenameVariable(LogicStatement *originalFormula, Variable *toBeReplaced, SolutionTabWidget *UI) {

	Variable *userInputVariable;
	Message errorMessage = NO_MESSAGE;

	while (!acceptedRenamedVariable(originalFormula, userInputVariable = getAnyVariableCasted(RENAME_PREFIX, toBeReplaced, UI, errorMessage))) {
		errorMessage = VARIABLE_MUST_NOT_OCCUR_ERROR;
		delete userInputVariable;
	}

	return userInputVariable;
}

Variable *EquivalenceEngine::getQualifiedBoundVariable(LogicStatement *originalFormula, Variable *boundIdentifier, SolutionTabWidget *UI) {

	Variable *userInputVariable;
	Message errorMessage = NO_MESSAGE;

	while (!acceptedBoundedVariable(originalFormula,
									userInputVariable = getAnyVariableCasted(BOUND_VARIABLE_REQUEST_PREFIX, boundIdentifier, UI, errorMessage))) {
		errorMessage = VARIABLE_OCCURS_FREE_ERROR;
		delete userInputVariable;
	}

	return userInputVariable;
}

LogicStatement *EquivalenceEngine::getAnyVariable(const Message prefixMessage, Variable *suffix, SolutionTabWidget *UI, Message errorMessage) {
	LogicStatement *userInput;

	while (!isVariable(userInput = getAnyFormula(prefixMessage, suffix, UI, errorMessage))) {
		errorMessage = EXPECT_VARIABLE_ERROR;
		delete userInput;
	}

	return userInput;
}

Variable *EquivalenceEngine::getAnyVariableCasted(const Message prefixMessage, Variable *suffix, SolutionTabWidget *UI, const Message errorMessage) {
	return dynamic_cast<Variable *>(getAnyVariable(prefixMessage, suffix, UI, errorMessage));
}

LogicStatement *EquivalenceEngine::getAnyFormula(const Message prefixMessage, Variable *suffix, SolutionTabWidget *UI, const Message errorMessage) {
	return UI->getReplacement(prefixMessage, suffix, errorMessage);
}

bool EquivalenceEngine::addNewPropositionalEquivalence(LogicSet *ruleSet) {
	return ruleEngine->addRule(ruleSet);
}
