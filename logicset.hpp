#ifndef LOGICSET_HPP
#define LOGICSET_HPP

#include <QVector>
#include "AST.hpp"

namespace AST
{
class LogicStatement;
}

typedef AST::LogicStatement Rule;

/* Alternative for C++ set */
class LogicSet
{
  private:
	QVector<Rule *> *set;
	QString comment;

  public:
	LogicSet();
	~LogicSet();

	/* Calls Delete on each item in the LogicSet */
	void deepDeleteContent();
	bool isEmpty();

	bool contains(Rule *);

	/* Set Implementation of add() */
	bool add(Rule *);

	/* Returns a vector representing the set */
	QVector<Rule *> *getSet();

	/* Same Operation for Set difference to remove certain element of the set,
	 * returns newly allocated set */
	LogicSet *diff(Rule *item);

	/* Set difference between two sets, returning a newly allocated set */
	LogicSet *diff(LogicSet *other);

	QString print(bool fullBracket);

	/* Checking whether two sets contains exactly same items */
	bool equals(LogicSet *other);

	/* Returns true iff every rule in other logicset exists in the current
	 * logicset */
	bool contains(LogicSet *other);

	/* Adds the entire set to the current one */
	void add(LogicSet *other);

	/* Sets the comment for rules */
	void setComment(QString ruleComment);

	/* Retreive comment */
	QString getComment();
};

#endif // LOGICSET_HPP
