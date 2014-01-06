#include "equivalenceengine.hpp"
#include "ruleparser.hpp"

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

		if (rule->isForwardRule())
			break;
	}

	return matchedRules;
}

LogicStatement *replaceStatement(LogicStatement *input, Rule *baseRule, Rule *transformationRule) {
	EquivalenceUtility matchingUtility;

	if (baseRule->match(input, &matchingUtility))
		return transformationRule->clone()->replace();

	return nullptr;
}

bool EquivalenceEngine::ruleApplicable(LogicStatement *input, LogicSet *ruleSet) {
	EquivalenceUtility matchingUtility;

	for (Rule *rule : *ruleSet->getSet()) {
		if (rule->match(input, &matchingUtility))
			return true;

		if (rule->isForwardRule())
			break;

		matchingUtility.clear();
	}

	return false;
}
