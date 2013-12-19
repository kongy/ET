#ifndef EQUIVALENCEENGINE_H
#define EQUIVALENCEENGINE_H

#include <logicset.hpp>
#include "equivalencestatement.hpp"

class EquivalenceEngine {
private:
	vector<LogicSet *> *rules;
public:
	EquivalenceEngine();
	~EquivalenceEngine();

	/* Returns a vector of matching rules under construction of
	 * EquivalenceStatement */
	vector<EquivalenceStatement *> *match(LogicStatement *);
};

#endif // EQUIVALENCEENGINE_H
