#ifndef EQUIVALENCEENGINE_H
#define EQUIVALENCEENGINE_H

#include "equivalenceutility.hpp"
using namespace AST;

class EquivalenceEngine {
private:
	QVector<LogicSet *> *rules;
	bool ruleApplicable(LogicStatement *, LogicSet *);
public:
	EquivalenceEngine();
	~EquivalenceEngine();

	/* Returns a vector of matching rule sets */
	QVector<LogicSet *> *match(LogicStatement *);

	LogicSet *diff(Rule *, LogicSet *);

	/* Returns a vector of rule that has been matched onto */
	QVector<Rule *> *getMatchedRules(LogicStatement *, LogicSet *);
	EquivalenceUtility *getReplacementNecessity(LogicStatement *input, Rule *baseRule, Rule *transformationRule);
	LogicStatement *replaceStatement(LogicStatement *input, Rule *baseRule, Rule *transformationRule);
};

#endif // EQUIVALENCEENGINE_H
