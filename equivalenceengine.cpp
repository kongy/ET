#include "equivalenceengine.hpp"
#include "ruleparser.hpp"
#include <QDebug>

EquivalenceEngine::EquivalenceEngine() {
	RuleParser ruleParser;
	rules = ruleParser.parseRuleXml();
}

EquivalenceEngine::~EquivalenceEngine() {
	if (rules != nullptr && !rules->isEmpty())
		for (LogicSet *logic_set : *rules) {
			logic_set->deepDeleteContent();
			delete logic_set;
		}

	delete rules;
}

QVector<LogicSet *> *EquivalenceEngine::match(LogicStatement *input) {

	QVector<LogicSet *> *relatedEquivalence = new QVector<LogicSet *>();

	for (LogicSet *ruleSet : *rules)
		if (ruleApplicable(input, ruleSet))
			relatedEquivalence->append(ruleSet);

	return relatedEquivalence;
}

LogicSet *EquivalenceEngine::diff(Rule *base, LogicSet *set) {
	return set->diff(base);
}

EquivalenceUtility *EquivalenceEngine::getReplacementNecessity(LogicStatement *input, Rule *baseRule, Rule *transformationRule) {
	return nullptr;
}

QVector<Rule *> *EquivalenceEngine::getMatchedRules(LogicStatement *input, LogicSet *ruleSet) {
	QVector<Rule *> *matchedRules = new QVector<Rule *>();

	EquivalenceUtility matchingUtility;

	for (Rule *rule : *ruleSet->getSet()) {
		if (rule->match(input, &matchingUtility)) {
			matchedRules->append(rule);
			matchingUtility.clear();
		}

		if (rule->isLeibnizRule())
			break;
	}

	return matchedRules;
}

LogicStatement *replaceStatement(LogicStatement *input, Rule *baseRule, Rule *transformationRule) {
	/* A matched leibniz rule does not need further processing */
	if (baseRule->isLeibnizRule())
		return transformationRule->clone();

	EquivalenceUtility matchingUtility;

	baseRule->match(input, &matchingUtility);

	Variable *boundVariable = matchingUtility.getBoundVariable();
	LogicSet *boundVariableSet = matchingUtility.getCandidateBoundVariableSet();
	QVector<Variable *> *freeVariableList = matchingUtility.getFreeVariableList();
	IDTable *idTable = matchingUtility.getIDTable();

	LogicSet *undefinedVariableSet = transformationRule->getExtraVariables(baseRule);

	/* Bound Variable Yet to be decided by user */
	if (boundVariable != nullptr) {

	}
	if (undefinedVariableSet->isEmpty())
		return transformationRule->clone()->replace(matchingUtility.getIDTable());
}

bool EquivalenceEngine::ruleApplicable(LogicStatement *input, LogicSet *ruleSet) {
	EquivalenceUtility matchingUtility;
	qDebug() << "Matching the rule";
	for (Rule *rule : *ruleSet->getSet()) {
		qDebug() << rule->print(false);
		if (rule->match(input, &matchingUtility)) {
			qDebug() << "Matched";
			return true;
		}

		qDebug() << "Mismatched";

		if (rule->isLeibnizRule())
			break;

		matchingUtility.clear();
	}

	return false;
}


