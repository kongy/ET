#include "AST.hpp"
#include "symbol.hpp"
#include "parser.hpp"
#include "lexer.hpp"

using namespace AST;

extern LogicStatement *entireStatement;

/* LogicStatement Class */
void LogicStatement::list_destroy(QVector<QVector<Variable *> *> *var_list) {
	for (QVector<Variable *> *identicalVar_list : *var_list)
		delete identicalVar_list;

	delete var_list;
}

bool LogicStatement::isEquivalent(LogicStatement *statement) {

	if (isFirstOrderLogic() || statement->isFirstOrderLogic())
		return true;

	QVector<QVector<Variable *>*> *var_list = new QVector<QVector<Variable *> *>();

	collectVariables(var_list);
	statement->collectVariables(var_list);

	if (var_list->empty()) {
		list_destroy(var_list);
		return evaluate() == statement->evaluate();
	}

	int list_size = var_list->size();
	unsigned int total_combinations = list_size * list_size;
	unsigned int current_combination;
	unsigned int shifted_combination;
	bool value;

	for (current_combination = 0; current_combination < total_combinations; ++current_combination) {
		shifted_combination = current_combination;

		for (QVector<Variable *> *identicalVar_list : *var_list) {
			value = shifted_combination % 2;

			for (Variable *variable : *identicalVar_list)
				variable->setBooleanValue(value);

			shifted_combination /= 2;
		}

		if (evaluate() != statement->evaluate()) {
			list_destroy(var_list);
			return false;
		}
	}

	list_destroy(var_list);
	return true;
}

/* Truth Class */
QString Truth::print(bool) {
	return SYMBOL_TRUTH;
}

bool Truth::isFirstOrderLogic() {
	return false;
}

Symbol Truth::getSymbol() {
	return Symbol::TRUTH_SYMBOL;
}

bool Truth::evaluate() {
	return true;
}

void Truth::collectVariables(QVector<QVector<Variable *> *> *) {
	//DO NOTHING
}

bool Truth::equals(LogicStatement *statement) {
	return statement->getSymbol() == getSymbol();
}

bool Truth::match(LogicStatement *matching_statement, IDTable *) {
	return matching_statement->getSymbol() == getSymbol();
}

LogicStatement* Truth::replace(IDTable *) {
	return this;
}

LogicStatement* Truth::clone() {
	return new Truth();
}

inline bool Truth::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol();
}

/* Falsity Class */
QString Falsity::print(bool) {
	return SYMBOL_FALSITY;
}

bool Falsity::isFirstOrderLogic() {
	return false;
}

Symbol Falsity::getSymbol() {
	return Symbol::FALSITY_SYMBOL;
}

bool Falsity::evaluate() {
	return false;
}

void Falsity::collectVariables(QVector<QVector<Variable *> *> *) {
	//DO NOTHING
}

bool Falsity::equals(LogicStatement *statement) {
	return statement->getSymbol() == getSymbol();
}

bool Falsity::match(LogicStatement *matching_statement, IDTable *) {
	return matching_statement->getSymbol() == getSymbol();
}

LogicStatement* Falsity::replace(IDTable *) {
	return this;
}

LogicStatement* Falsity::clone() {
	return new Falsity();
}

inline bool Falsity::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol();
}

/* Variable Class */
Variable::Variable(QString *name) {
	setName(name);
}

QString Variable::print(bool) {
	return getName();
}

QString Variable::getName() {
	return name;
}

void Variable::setName(QString *name) {
	this->name = *name;
}

bool Variable::isFirstOrderLogic() {
	return false;
}

Symbol Variable::getSymbol() {
	return Symbol::VARIABLE_SYMBOL;
}

void Variable::setBooleanValue(bool value) {
	this->value = value;
}

bool Variable::evaluate() {
	return value;
}

bool Variable::equals(LogicStatement *statement) {
	return statement->getSymbol() == getSymbol() &&
			(dynamic_cast<Variable *>(statement))->getName() == getName();
}

void Variable::collectVariables(QVector<QVector<Variable *> *> *var_list) {

	if (!var_list->empty())
		for (QVector<Variable *> *identicalVar_list : *var_list)
			if (identicalVar_list->front()->equals(this)) {
				identicalVar_list->push_back(this);
				return;
			}

	QVector<Variable *> *new_list = new QVector<Variable *>();
	new_list->push_back(this);
	var_list->push_back(new_list);
}

bool Variable::match(LogicStatement *matching_statement, IDTable *table) {
	return table->add(this, matching_statement);
}

LogicStatement* Variable::replace(IDTable *table) {
	LogicStatement *replacedStatement = table->valueOf(this);
	delete this;
	return replacedStatement;
}

LogicStatement* Variable::clone() {
	return new Variable(new QString(getName()));
}

inline bool Variable::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			getName() == dynamic_cast<Variable&>(other).getName();
}

/* UnaryOpStatement Class (Virtual) */
void UnaryOpStatement::setStatement(LogicStatement *statement) {
	nestedStatement = statement;
}

LogicStatement * UnaryOpStatement::getStatement() {
	return nestedStatement;
}

void UnaryOpStatement::collectVariables(QVector<QVector<Variable *> *> *var_list) {
	getStatement()->collectVariables(var_list);
}

bool UnaryOpStatement::equals(LogicStatement *statement) {
	return statement->getSymbol() == getSymbol() &&
			getStatement()->equals((dynamic_cast<UnaryOpStatement *>(statement))->getStatement());
}

bool UnaryOpStatement::match(LogicStatement *matching_statement, IDTable *table) {
	return matching_statement->getSymbol() == getSymbol() &&
			getStatement()->match((dynamic_cast<UnaryOpStatement *>(matching_statement))->getStatement(), table);
}

LogicStatement* UnaryOpStatement::replace(IDTable *table) {
	setStatement(getStatement()->replace(table));
	return this;
}

inline bool UnaryOpStatement::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			*getStatement() == *dynamic_cast<UnaryOpStatement&>(other).getStatement();
}

/* NotStatement Class */
NotStatement::NotStatement(LogicStatement *statement) {
	setStatement(statement);
}

bool NotStatement::isFirstOrderLogic() {
	return getStatement()->isFirstOrderLogic();
}

QString NotStatement::print(bool fullBracket) {

	if (!fullBracket && getSymbol() >= getStatement()->getSymbol())
		return QString(SYMBOL_NOT) + getStatement()->print(fullBracket);
	else
		return QString(SYMBOL_NOT) + QString("(") +
				getStatement()->print(fullBracket) + QString(")");

}


Symbol NotStatement::getSymbol() {
	return Symbol::NOT_SYMBOL;
}

bool NotStatement::evaluate() {
	return !getStatement()->evaluate();
}

LogicStatement* NotStatement::clone() {
	return new NotStatement(getStatement()->clone());
}

/* BinaryOpStatement Class (Virtual) */
void BinaryOpStatement::setLeftStatement(LogicStatement *newLeft) {
	leftStatement = newLeft;
}

void BinaryOpStatement::setRightStatement(LogicStatement *newRight) {
	rightStatement = newRight;
}

LogicStatement * BinaryOpStatement::getLeftStatement() {
	return leftStatement;
}

LogicStatement * BinaryOpStatement::getRightStatement() {
	return rightStatement;
}

bool BinaryOpStatement::isFirstOrderLogic() {
	return getLeftStatement()->isFirstOrderLogic()
			|| getRightStatement()->isFirstOrderLogic();
}

QString BinaryOpStatement::print(bool fullBracket) {
	QString left;
	QString right;

	if (!fullBracket) {
		if ( this->getSymbol() > getLeftStatement()->getSymbol() ) {
			left = getLeftStatement()->print(fullBracket) + QString(" ");
		} else {
			left = QString("(") + getLeftStatement()->print(fullBracket) + QString(")") +
					QString(" ");
		}

		if ( this->getSymbol() > getRightStatement()->getSymbol() ) {
			right = QString(" ") + getRightStatement()->print(fullBracket);
		} else {
			right = QString(" ") +
					QString("(") + getRightStatement()->print(fullBracket) + QString(")");
		}
	} else {
		left = QString("(") + getLeftStatement()->print(fullBracket) + QString(")") + QString(" ");
		right = QString(" ") + QString("(") + getRightStatement()->print(fullBracket) + QString(")");
	}

	return left + symbol() + right;

}

void BinaryOpStatement::collectVariables(QVector<QVector<Variable *> *> *var_list) {
	getLeftStatement()->collectVariables(var_list);
	getRightStatement()->collectVariables(var_list);
}

bool BinaryOpStatement::equals(LogicStatement *statement) {
	return statement->getSymbol() == getSymbol() &&
			getLeftStatement()->equals((dynamic_cast<BinaryOpStatement *>(statement))->getLeftStatement()) &&
			getRightStatement()->equals((dynamic_cast<BinaryOpStatement *>(statement))->getRightStatement());
}

bool BinaryOpStatement::match(LogicStatement *matching_statement, IDTable *table) {
	return matching_statement->getSymbol() == getSymbol() &&
			getLeftStatement()->match((dynamic_cast<BinaryOpStatement *>(matching_statement))->getLeftStatement(), table) &&
			getRightStatement()->match((dynamic_cast<BinaryOpStatement *>(matching_statement))->getRightStatement(), table);
}

LogicStatement* BinaryOpStatement::replace(IDTable *table) {
	setLeftStatement(getLeftStatement()->replace(table));
	setRightStatement(getRightStatement()->replace(table));
	return this;
}

inline bool BinaryOpStatement::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			*getLeftStatement() == *dynamic_cast<BinaryOpStatement&>(other).getLeftStatement() &&
			*getRightStatement() == *dynamic_cast<BinaryOpStatement&>(other).getRightStatement();
}

/* AndStatement Class */
AndStatement::AndStatement(LogicStatement *left, LogicStatement *right) {
	setLeftStatement(left);
	setRightStatement(right);
}

QString AndStatement::symbol() {
	return SYMBOL_AND;
}

Symbol AndStatement::getSymbol() {
	return Symbol::AND_SYMBOL;
}

bool AndStatement::evaluate() {
	return getLeftStatement()->evaluate() && getRightStatement()->evaluate();
}

LogicStatement* AndStatement::clone() {
	return new AndStatement(getLeftStatement()->clone(), getRightStatement()->clone());
}

/* OrStatement Class */
OrStatement::OrStatement(LogicStatement *left, LogicStatement *right) {
	setLeftStatement(left);
	setRightStatement(right);
}

QString OrStatement::symbol() {
	return SYMBOL_OR;
}

Symbol OrStatement::getSymbol() {
	return Symbol::OR_SYMBOL;
}

bool OrStatement::evaluate() {
	return getLeftStatement()->evaluate() || getRightStatement()->evaluate();
}

LogicStatement* OrStatement::clone() {
	return new OrStatement(getLeftStatement()->clone(), getRightStatement()->clone());
}

/* IffStatement Class */
IffStatement::IffStatement(LogicStatement *left, LogicStatement *right) {
	setLeftStatement(left);
	setRightStatement(right);
}

QString IffStatement::symbol() {
	return SYMBOL_IFF;
}

Symbol IffStatement::getSymbol() {
	return Symbol::IFF_SYMBOL;
}

bool IffStatement::evaluate() {
	return getLeftStatement()->evaluate() == getRightStatement()->evaluate();
}

LogicStatement* IffStatement::clone() {
	return new IffStatement(getLeftStatement()->clone(), getRightStatement()->clone());
}

/* ImpliesStatement Class */
ImpliesStatement::ImpliesStatement(LogicStatement *left, LogicStatement *right) {
	setLeftStatement(left);
	setRightStatement(right);
}

QString ImpliesStatement::symbol() {
	return SYMBOL_IMPLIES;
}

Symbol ImpliesStatement::getSymbol() {
	return Symbol::IMPLIES_SYMBOL;
}

bool ImpliesStatement::evaluate() {
	return (!getLeftStatement()->evaluate() || getRightStatement()->evaluate());
}

LogicStatement* ImpliesStatement::clone() {
	return new ImpliesStatement(getLeftStatement()->clone(), getRightStatement()->clone());
}

/* FirstOrderStatement Class */
bool FirstOrderStatement::isFirstOrderLogic() {
	return true;
}

bool FirstOrderStatement::evaluate() {
	return false;
}

void FirstOrderStatement::collectVariables(QVector<QVector<Variable *> *> *) {
	//UNUSED
}

bool FirstOrderStatement::equals(LogicStatement *) {
	return true;
}

bool FirstOrderStatement::match(LogicStatement *, IDTable *) {
	return false;
}

LogicStatement* FirstOrderStatement::clone() {
	return this;
}

LogicStatement* FirstOrderStatement::replace(IDTable *) {
	return this;
}

inline bool FirstOrderStatement::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol();
}

/* ForAllStatement Class */
ForAllStatement::ForAllStatement(Variable *identifier, LogicStatement *forAllStatement) {
	setStatement(forAllStatement);
	setIdentifier(identifier);
}

void ForAllStatement::setIdentifier(Variable *identifier) {
	this->identifier = identifier;
}

QString ForAllStatement::print(bool fullBracket) {

	if (!fullBracket && getSymbol() >= getStatement()->getSymbol())
		return QString(SYMBOL_FORALL) + getStatement()->print(fullBracket);
	else
		return QString(SYMBOL_FORALL) + QString("(") +
				getStatement()->print(fullBracket) + QString(")");

}

LogicStatement * ForAllStatement::getStatement() {
	return statement;
}

void ForAllStatement::setStatement(LogicStatement *newstatement) {
	statement = newstatement;
}

Symbol ForAllStatement::getSymbol() {
	return Symbol::FORALL_SYMBOL;
}

/* ThereExistsStatement Class */
ThereExistsStatement::ThereExistsStatement(
		Variable *identifier, LogicStatement *thereExistsStatement) {
	setStatement(thereExistsStatement);
	setIdentifier(identifier);
}

void ThereExistsStatement::setIdentifier(Variable *identifier) {
	this->identifier = identifier;
}

QString ThereExistsStatement::print(bool fullBracket) {

	if (!fullBracket && getSymbol() >= getStatement()->getSymbol())
		return QString(SYMBOL_THEREEXISTS) + getStatement()->print(fullBracket);
	else
		return QString(SYMBOL_THEREEXISTS) + QString("(") +
				getStatement()->print(fullBracket) + QString(")");
}

LogicStatement * ThereExistsStatement::getStatement() {
	return statement;
}

void ThereExistsStatement::setStatement(LogicStatement *newStatement) {
	statement = newStatement;
}

Symbol ThereExistsStatement::getSymbol() {
	return Symbol::THEREEXISTS_SYMBOL;
}

/* Parameters Class */
Parameters::Parameters(Variable *variable, Parameters *remainingParameters) {
	setParameter(variable);
	setRemainingParameters(remainingParameters);
}

Variable * Parameters::getParameter() {
	return parameter;
}

bool Parameters::evaluate() {
	return false;
}

void Parameters::setParameter(Variable *newParameter) {
	parameter = newParameter;
}

Parameters * Parameters::getRemainingParameters() {
	return rest;
}

void Parameters::setRemainingParameters(Parameters *remainingParameters) {
	rest = remainingParameters;
}

bool Parameters::isFirstOrderLogic() {
	return true;
}

QString Parameters::print(bool fullBracket) {
	QString param = getParameter()->print(fullBracket);

	if (rest != NULL)
		return param + QString(", ") + getRemainingParameters()->print(fullBracket);
	else
		return param;
}

Symbol Parameters::getSymbol() {
	return Symbol::PARAMETERS_SYMBOL;
}

void Parameters::collectVariables(QVector<QVector<Variable *> *> *) {
	//NOT USED
}

bool Parameters::equals(LogicStatement *statement) {

	if (getSymbol() == statement->getSymbol()) {

		Parameters *casted_statement = dynamic_cast<Parameters *>(statement);

		if (getParameter()->equals(casted_statement->getParameter())) {
			Parameters *casted_statement_params = casted_statement->getRemainingParameters();
			Parameters *current_statement_params = getRemainingParameters();

			if (casted_statement_params == NULL && current_statement_params == NULL)
				return true;
			else if (casted_statement_params == NULL || current_statement_params == NULL)
				return false;
			else
				return current_statement_params->equals(casted_statement_params);
		}
	}

	return false;
}

bool Parameters::match(LogicStatement *, IDTable *) {
	return false;
}

LogicStatement* Parameters::clone() {
	Parameters *newRemainingParameters = nullptr;
	Parameters *remainingParameters = getRemainingParameters();

	if (remainingParameters != nullptr)
		newRemainingParameters = dynamic_cast<Parameters *>(remainingParameters->clone());

	return new Parameters(dynamic_cast<Variable *>(getParameter()->clone()), newRemainingParameters);
}

LogicStatement* Parameters::replace(IDTable *table) {
	setParameter(dynamic_cast<Variable *>(getParameter()->replace(table)));

	Parameters *remainingParameter = getRemainingParameters();
	if (remainingParameter != nullptr)
		setRemainingParameters(dynamic_cast<Parameters *>(getRemainingParameters()->replace(table)));

	return this;
}

inline bool Parameters::operator==(LogicStatement &other) {
	/* Must have same symbol and parameter match to proceed */
	if (!(getSymbol() == other.getSymbol() &&
		  *getParameter() == *dynamic_cast<Parameters&>(other).getParameter()))
		return false;

	/* Symbol and type matched */
	Parameters *otherCasted = &dynamic_cast<Parameters&>(other);
	Parameters *currentRemainingParams = getRemainingParameters();
	Parameters *otherRemainingParams = otherCasted->getRemainingParameters();

	/* No more parameters */
	if (currentRemainingParams == nullptr &&
			otherRemainingParams == nullptr)
		return true;

	/* One has more parameter whilst other one doesn't */
	if (currentRemainingParams == nullptr || otherRemainingParams == nullptr)
		return false;

	/* Both has more parameters needs to be matched */
	return *currentRemainingParams == *otherRemainingParams;
}

/* PredicateSymbolStatement Class */
PredicateSymbolStatement::PredicateSymbolStatement(Variable *predicateName, Parameters *params) {
	setPredicateSymbol(predicateName);
	setParameters(params);
}

QString PredicateSymbolStatement::getPredicateSymbolName() {
	return getPredicateSymbol()->getName();
}

Variable * PredicateSymbolStatement::getPredicateSymbol() {
	return predicateSymbol;
}

void PredicateSymbolStatement::setPredicateSymbol(Variable *predicateName) {
	predicateSymbol = predicateName;
}

Parameters * PredicateSymbolStatement::getParameters() {
	return parameters;
}

void PredicateSymbolStatement::setParameters(Parameters *params) {
	parameters = params;
}

QString PredicateSymbolStatement::print(bool fullBracket) {
	return getPredicateSymbolName() + QString("(") +
			getParameters()->print(fullBracket) + QString(")");
}

Symbol PredicateSymbolStatement::getSymbol() {
	return Symbol::PREDICATE_SYMBOL;
}

/* EqualityStatement Class */
EqualityStatement::EqualityStatement(Variable *left, Variable *right) {
	setLeftVariable(left);
	setRightVariable(right);
}

Variable * EqualityStatement::getLeftVariable() {
	return leftVariable;
}

Variable * EqualityStatement::getRightVariable() {
	return rightVariable;
}

void EqualityStatement::setLeftVariable(Variable *newLeft) {
	leftVariable = newLeft;
}

void EqualityStatement::setRightVariable(Variable *newRight) {
	rightVariable = newRight;
}

QString EqualityStatement::print(bool fullBracket) {
	return getLeftVariable()->print(fullBracket) +
			QString(" ") + QString(SYMBOL_EQUALS) + QString(" ") +
			getRightVariable()->print(fullBracket);
}

Symbol EqualityStatement::getSymbol() {
	return Symbol::EQUALS_SYMBOL;
}

extern int yyparse();

/**
 * @brief Parse the string to generate AST
 * @param expression String to parse
 * @return Well-formed AST if expression is well-formed
 *         otherwise return NULL
 */
LogicStatement *AST::parse(QString expression) {
	yy_scan_string(expression.toUtf8().constData());
	int result = yyparse();
	return result == 0 ? entireStatement : nullptr;
}
