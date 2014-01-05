#ifndef EQUIVALENCEENGINE_H
#define EQUIVALENCEENGINE_H

#include "logicset.hpp"
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
	QVector<Rule *> *getMatchedRules(LogicStatement *, LogicSet *);
	LogicStatement *replaceStatement(LogicStatement *input, Rule *baseRule, Rule *transformationRule);
};

#endif // EQUIVALENCEENGINE_H
