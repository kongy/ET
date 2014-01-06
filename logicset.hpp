#ifndef LOGICSET_HPP
#define LOGICSET_HPP

#include <QVector>
#include "AST.hpp"

namespace AST {
class LogicStatement;
}

typedef AST::LogicStatement Rule;
/* Alternative for C++ set */
class LogicSet
{
private:
	QVector<Rule *> *set;
public:
	LogicSet();
	~LogicSet();

	/* Calls Delete on each item in the LogicSet */
	void deepDeleteContent();
	bool isEmpty();

	bool exists(Rule *);

	/* Set Implementation of add() */
	bool add(Rule *);

	/* Returns a vector representing the set */
	QVector<Rule *> *getSet();

	/* Same Operation for Set difference to delete certain element of the set */
	LogicSet *diff(Rule *item);

	QString print(bool fullBracket);
};

#endif // LOGICSET_HPP
