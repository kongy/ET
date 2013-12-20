#ifndef EQUIVALENCEENGINE_H
#define EQUIVALENCEENGINE_H

#include <logicset.hpp>
#include "equivalencestatement.hpp"

class EquivalenceEngine {
private:
	QVector<LogicSet *> *rules;
	void parseRulesTxt();
public:
	EquivalenceEngine();
	~EquivalenceEngine();

	/* Returns a vector of matching rules under construction of
	 * EquivalenceStatement */
	QVector<EquivalenceStatement *> *match(LogicStatement *);
};

#endif // EQUIVALENCEENGINE_H
