#ifndef LOGICSET_HPP
#define LOGICSET_HPP

#include <QVector>
#include <AST.hpp>
using namespace AST;

/* Alternative for C++ set */
class LogicSet
{
private:
	QVector<LogicStatement *> *set;
	bool inSet(LogicStatement *);
public:
	LogicSet();
	~LogicSet();

	/* Set Implementation of add() */
	bool add(LogicStatement *);

	/* Returns a vector representing the set */
	QVector<LogicStatement *> *getSet();

	/* Same Operation for Set difference to delete certain element of the set */
	LogicSet *diff(LogicStatement *item);
};

#endif // LOGICSET_HPP
