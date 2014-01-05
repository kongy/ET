#include "equivalenceengine.hpp"
#include "ruleparser.hpp"

EquivalenceEngine::EquivalenceEngine() {
	RuleParser ruleParser;
	this->rules = ruleParser.parseRuleXml();
}

EquivalenceEngine::~EquivalenceEngine() {
	for (LogicSet *logic_set : *rules)
		delete logic_set;
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

QVector<Rule *> *EquivalenceEngine::getMatchedRules(LogicStatement *input, LogicSet *ruleSet) {
	QVector<Rule *> *matchedRules = new QVector<Rule *>();

	IDTable idtable;

	for (Rule *rule : *ruleSet->getSet())
		if (rule->match(input, &idtable)) {
			matchedRules->append(rule);
			idtable.clear();
		}

	return matchedRules;
}

LogicStatement *replaceStatement(LogicStatement *input, Rule *baseRule, Rule *transformationRule) {
	IDTable idtable;

	if (baseRule->match(input, &idtable))
		return transformationRule->clone()->replace(&idtable);

	return nullptr;
}

bool EquivalenceEngine::ruleApplicable(LogicStatement *input, LogicSet *ruleSet) {
	IDTable idtable;

	for (Rule *rule : *ruleSet->getSet()) {
		if (rule->match(input, &idtable))
			return true;
	}

	return false;
}
