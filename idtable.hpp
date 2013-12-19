#ifndef IDTABLE_HPP
#define IDTABLE_HPP

class IDTable;

#include "AST.hpp"
#include <utility>
using namespace AST;
using namespace std;

class IDTable
{
private:
	vector<pair<Variable *, LogicStatement *> *> *id_table;
public:
	IDTable();

	/* Adding a key value pair that maps identifiers for rules
	 * to the filtered version of the logicstatement which
	 * user provided
	 */
	bool add(Variable *key, LogicStatement *value);

    /* Returns the vector of entire mapping from identifier to
     * LogicStatement
     */
	vector<pair<Variable *, LogicStatement *> *> *getIdTable();

    /* Provided a key, returns the corresponding Logicstatement
     * or null if none found
     */
    LogicStatement *valueOf(Variable *key);

    /* Free up the space allocated by Pairs and empty the mapping
     * table
     */
    void clear();
};

#endif // IDTABLE_HPP
