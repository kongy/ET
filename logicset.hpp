#ifndef LOGICSET_HPP
#define LOGICSET_HPP

#include <vector>
#include <AST.hpp>
using namespace AST;
using namespace std;

/* Alternative for C++ set */
class LogicSet
{
private:
	vector<LogicStatement *> *set;
	bool inSet(LogicStatement *);
public:
	LogicSet();
	~LogicSet();

    /* Set Implementation of add()
     */
	bool add(LogicStatement *);

	/* Returns a vector representing
	 * the set
	 */
	vector<LogicStatement *> *getSet();

    /* Same Operation for Set difference
     * to delete certain element of the set
     */
    LogicSet *diff(LogicStatement *item);
};

#endif // LOGICSET_HPP
