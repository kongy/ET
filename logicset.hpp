#ifndef LOGICSET_HPP
#define LOGICSET_HPP

#include <vector>
#include <AST.hpp>
using namespace AST;
using namespace std;

class LogicSet
{
private:
	vector<LogicStatement *> *set;
	bool inSet(LogicStatement *);
public:
	LogicSet();
	~LogicSet();
	bool add(LogicStatement *);
	vector<LogicStatement *> *getSet();
	LogicSet *diff(LogicStatement *item);
};

#endif // LOGICSET_HPP
