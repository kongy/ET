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

int LogicStatement::comparePrecedence(LogicStatement *outer, LogicStatement *inner) {
	return outer->getSymbol() - inner->getSymbol();
}

LogicStatement::~LogicStatement() {}

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

void Truth::collectVariables(QVector<QVector<Variable *> *> *) {}

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

bool Truth::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol();
}

QVector<QPair<QString, LogicStatement *> > Truth::getStringMapping(bool) {
	QVector<QPair<QString, LogicStatement *> > mapping;
	mapping.append(QPair<QString, LogicStatement *>(QString(SYMBOL_TRUTH), this));
	return mapping;
}

bool Truth::variableBounded(Variable *) {
	/* No constraint on constant, so variable is bounded. */
	return true;
}

void Truth::collectFreeVariable(Variable *, QVector<Variable *> *) {}

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

void Falsity::collectVariables(QVector<QVector<Variable *> *> *) {}

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

bool Falsity::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol();
}

QVector<QPair<QString, LogicStatement *> > Falsity::getStringMapping(bool) {
	QVector<QPair<QString, LogicStatement *> > mapping;
	mapping.append(QPair<QString, LogicStatement *>(QString(SYMBOL_FALSITY), this));
	return mapping;
}

bool Falsity::variableBounded(Variable *) {
	/* No constraint on constant, so variable is bounded. */
	return true;
}

void Falsity::collectFreeVariable(Variable *, QVector<Variable *> *) {}

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

bool Variable::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			getName() == dynamic_cast<Variable&>(other).getName();
}

QVector<QPair<QString, LogicStatement *> > Variable::getStringMapping(bool) {
	QVector<QPair<QString, LogicStatement *> > mapping;
	mapping.append(QPair<QString, LogicStatement *>(getName(), this));
	return mapping;
}

bool Variable::variableBounded(Variable *boundedVariable) {
	/* A variable is bounded if either it is caght in ThereExists or ForAll statement. Otherwise it doesn't match
	 * any Variables that exists. */
	return !this->equals(boundedVariable);
}

void Variable::collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection) {
	/* We reach this block of code because it is not caught by quantifiers */
	if (this->equals(freeVariable))
		collection->append(this);
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

QString UnaryOpStatement::print(bool fullBracket) {
	QString SYMBOL = symbol();

	if (!fullBracket && (comparePrecedence(this, getStatement()) >= 0))
		return SYMBOL + getStatement()->print(fullBracket);
	else
		return QString("%1(%2)").arg(SYMBOL, getStatement()->print(fullBracket));
}

bool UnaryOpStatement::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			*getStatement() == *dynamic_cast<UnaryOpStatement&>(other).getStatement();
}

UnaryOpStatement::~UnaryOpStatement() {
	delete getStatement();
}

QVector<QPair<QString, LogicStatement *> > UnaryOpStatement::getStringMapping(bool fullBracket) {
	QVector<QPair<QString, LogicStatement *> > mapping;

	/* Symbols are needed anyway */
	mapping.append(QPair<QString, LogicStatement *>(symbol(), this));

	/* Bracket Not Needed */
	if (!fullBracket && (comparePrecedence(this, getStatement()) >= 0))
		mapping += getStatement()->getStringMapping(fullBracket);
	/* Brackets Required */
	else {
		mapping.append(QPair<QString, LogicStatement *>("(", nullptr));
		mapping += getStatement()->getStringMapping(fullBracket);
		mapping.append(QPair<QString, LogicStatement *>(")", nullptr));
	}

	return mapping;
}

bool UnaryOpStatement::variableBounded(Variable *boundedVariable) {
	/* Propositional logic operator does nothing, need to check nested statement */
	return getStatement()->variableBounded(boundedVariable);
}

void UnaryOpStatement::collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection) {
	/* Symbols has no effect on free variables */
	getStatement()->collectFreeVariable(freeVariable, collection);
}

/* NotStatement Class */
NotStatement::NotStatement(LogicStatement *statement) {
	setStatement(statement);
}

bool NotStatement::isFirstOrderLogic() {
	return getStatement()->isFirstOrderLogic();
}

Symbol NotStatement::getSymbol() {
	return Symbol::NOT_SYMBOL;
}

QString NotStatement::symbol() {
	return SYMBOL_NOT;
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

	if (!fullBracket && (comparePrecedence(this, getLeftStatement()) >= 0)) {
		left = getLeftStatement()->print(fullBracket) + QString(" ");
	} else {
		left = QString("(%1) ").arg(getLeftStatement()->print(fullBracket));
	}

	if (!fullBracket && (comparePrecedence(this, getRightStatement()) > 0)) {
		right = QString(" ") + getRightStatement()->print(fullBracket);
	} else {
		right = QString(" (%1)").arg(getRightStatement()->print(fullBracket));
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

bool BinaryOpStatement::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			*getLeftStatement() == *dynamic_cast<BinaryOpStatement&>(other).getLeftStatement() &&
			*getRightStatement() == *dynamic_cast<BinaryOpStatement&>(other).getRightStatement();
}

BinaryOpStatement::~BinaryOpStatement() {
	delete getLeftStatement();
	delete getRightStatement();
}

QVector<QPair<QString, LogicStatement *> > BinaryOpStatement::getStringMapping(bool fullBracket) {

	QVector<QPair<QString, LogicStatement *> > mapping;

	/* Bracket not needed on left */
	if (!fullBracket && (comparePrecedence(this, getLeftStatement()) >= 0))
		mapping = getLeftStatement()->getStringMapping(fullBracket);
	/* Brackets needed on left */
	else {
		mapping.append(QPair<QString, LogicStatement *>("(", nullptr));
		mapping += getLeftStatement()->getStringMapping(fullBracket);
		mapping.append(QPair<QString, LogicStatement *>(")", nullptr));
	}

	mapping.append(QPair<QString, LogicStatement *>(symbol(), this));

	/* Brackets not needed on right */
	if (!fullBracket && (comparePrecedence(this, getRightStatement()) > 0))
		mapping += getRightStatement()->getStringMapping(fullBracket);
	else {
		mapping.append(QPair<QString, LogicStatement *>("(", nullptr));
		mapping += getRightStatement()->getStringMapping(fullBracket);
		mapping.append(QPair<QString, LogicStatement *>(")", nullptr));
	}

	return mapping;
}

bool BinaryOpStatement::variableBounded(Variable *boundedVariable) {
	/* Propositional logic operator has no impact on whether a variable is bounded or not. Both sides have to be
	 * evaluated. */
	return getLeftStatement()->variableBounded(boundedVariable) &&
			getRightStatement()->variableBounded(boundedVariable);
}

void BinaryOpStatement::collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection) {
	/* Symbols has no effect on free variables, need to collect both side of BinaryStatement */
	getLeftStatement()->collectFreeVariable(freeVariable, collection);
	getRightStatement()->collectFreeVariable(freeVariable, collection);
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

void FirstOrderStatement::collectVariables(QVector<QVector<Variable *> *> *) {}

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

bool FirstOrderStatement::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol();
}

FirstOrderStatement::~FirstOrderStatement() {}

/* ForAllStatement Class */
ForAllStatement::ForAllStatement(Variable *identifier, LogicStatement *forAllStatement) {
	setStatement(forAllStatement);
	setIdentifier(identifier);
}

void ForAllStatement::setIdentifier(Variable *identifier) {
	this->identifier = identifier;
}

QString ForAllStatement::print(bool fullBracket) {
    return QString("%1%2(%3)").arg(SYMBOL_FORALL, getQuantifier()->print(fullBracket),
                                   getStatement()->print(fullBracket));
}

LogicStatement * ForAllStatement::getStatement() {
    return statement;
}

Variable *ForAllStatement::getQuantifier() {
    return identifier;
}

void ForAllStatement::setStatement(LogicStatement *newstatement) {
    statement = newstatement;
}

Symbol ForAllStatement::getSymbol() {
    return Symbol::FORALL_SYMBOL;
}

ForAllStatement::~ForAllStatement() {
	delete getQuantifier();
	delete getStatement();
}

QVector<QPair<QString, LogicStatement *> > ForAllStatement::getStringMapping(bool fullBracket) {
	QVector<QPair<QString, LogicStatement *> > mapping;

	/* Quantifier forall binds the entirestatement */
	mapping.append(QPair<QString, LogicStatement *>(QString(SYMBOL_FORALL), this));
	/* Nullptr because Quantifier alone is meaningless for equivalence */
	mapping.append(QPair<QString, LogicStatement *>(getQuantifier()->getName(), nullptr));

	/* Bracket not needed */
	if (!fullBracket && (comparePrecedence(this, getStatement()) > 0))
		mapping += getStatement()->getStringMapping(fullBracket);
	/* Brackets needed */
	else {
		mapping.append(QPair<QString, LogicStatement *>("(", nullptr));
		mapping += getStatement()->getStringMapping(fullBracket);
		mapping.append(QPair<QString, LogicStatement *>(")", nullptr));
	}

	return mapping;
}

bool ForAllStatement::variableBounded(Variable *boundedVariable) {
	/* If the variable appears in the quantifier, we stop and confirm that the variable is bounded. */
	if (getQuantifier()->equals(boundedVariable))
		return true;

	/* Variable didn't appear in quantifier, check the nested statement */
	return getStatement()->variableBounded(boundedVariable);
}

void ForAllStatement::collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection) {
	/* If not bounded by quantifier, then item might be free in nested statement, else stop */
	if (!getQuantifier()->equals(freeVariable))
		getStatement()->collectFreeVariable(freeVariable, collection);
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
    return QString("%1%2(%3)").arg(SYMBOL_THEREEXISTS, getQuantifier()->print(fullBracket),
                                   getStatement()->print(fullBracket));
}

LogicStatement * ThereExistsStatement::getStatement() {
    return statement;
}

Variable *ThereExistsStatement::getQuantifier() {
    return identifier;
}

void ThereExistsStatement::setStatement(LogicStatement *newStatement) {
    statement = newStatement;
}

Symbol ThereExistsStatement::getSymbol() {
    return Symbol::THEREEXISTS_SYMBOL;
}

ThereExistsStatement::~ThereExistsStatement() {
	delete getQuantifier();
	delete getStatement();
}

QVector<QPair<QString, LogicStatement *> > ThereExistsStatement::getStringMapping(bool fullBracket) {
	QVector<QPair<QString, LogicStatement *> > mapping;

	/* Maps symbol thereexists to the entire statement */
	mapping.append(QPair<QString, LogicStatement *>(QString(SYMBOL_THEREEXISTS), this));
	/* Nullptr because quantifier alone is meaningless */
	mapping.append(QPair<QString, LogicStatement *>(getQuantifier()->getName(), nullptr));

	/* Bracket Not Needed */
	if (!fullBracket && (comparePrecedence(this, getStatement()) > 0))
		mapping += getStatement()->getStringMapping(fullBracket);
	/* Brackets needed */
	else {
		mapping.append(QPair<QString, LogicStatement *>("(", nullptr));
		mapping += getStatement()->getStringMapping(fullBracket);
		mapping.append(QPair<QString, LogicStatement *>(")", nullptr));
	}

	return mapping;
}

bool ThereExistsStatement::variableBounded(Variable *boundedVariable) {
	/* If the variable appears in the quantifier, we stop and confirm that the variable is bounded. */
	if (getQuantifier()->equals(boundedVariable))
		return true;

	/* Variable didn't appear in quantifier, check the nested statement */
	return getStatement()->variableBounded(boundedVariable);
}

void ThereExistsStatement::collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection) {
	/* If not bounded by quantifier, then item might be free in nested statement, else stop */
	if (!getQuantifier()->equals(freeVariable))
		getStatement()->collectFreeVariable(freeVariable, collection);
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

void Parameters::collectVariables(QVector<QVector<Variable *> *> *) {}

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

bool Parameters::operator==(LogicStatement &other) {
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

Parameters::~Parameters() {
	delete getParameter();
	Parameters *remainingParameters = getRemainingParameters();
	if (remainingParameters != nullptr)
		delete remainingParameters;
}

QVector<QPair<QString, LogicStatement *> > Parameters::getStringMapping(bool fullBracket) {
	QVector<QPair<QString, LogicStatement *> > mapping;

	/* Null Pointer here because one argument is meaningless alone */
	mapping.append(QPair<QString, LogicStatement *>(getParameter()->getName(), nullptr));
	Parameters *remainingParameters = getRemainingParameters();

	if (remainingParameters != nullptr) {
		mapping.append(QPair<QString, LogicStatement *>(",", nullptr));
		mapping += remainingParameters->getStringMapping(fullBracket);
	}

	return mapping;
}

bool Parameters::variableBounded(Variable *boundedVariable) {
	/* We reached this block of code because variable is not caught in ThereExists or ForAll, so if boundedVariable
	 * appears, we stop and conclude the variable is not bounded */
	if (getParameter()->equals(boundedVariable))
		return false;

	Parameters *remainingParams = getRemainingParameters();

	/* The current variable is not bounded, if more variables check. */
	if (remainingParams != nullptr)
		return remainingParams->variableBounded(boundedVariable);

	/* No more variables */
	return true;
}

void Parameters::collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection) {

	Variable *parameter = getParameter();

	/* Variable not bounded by quantifiers, if they are the same, add to collection */
	if (parameter->equals(freeVariable))
		collection->append(parameter);

	Parameters *remainingParameters = getRemainingParameters();

	/* Collect from the remaining Parameters */
	if (remainingParameters != nullptr)
		remainingParameters->collectFreeVariable(freeVariable, collection);
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
	return QString("%1(%2)").arg(getPredicateSymbolName(), getParameters()->print(fullBracket));
}

Symbol PredicateSymbolStatement::getSymbol() {
	return Symbol::PREDICATE_SYMBOL;
}

PredicateSymbolStatement::~PredicateSymbolStatement() {
	delete getPredicateSymbol();
	delete getParameters();
}

QVector<QPair<QString, LogicStatement *> > PredicateSymbolStatement::getStringMapping(bool fullBracket) {
	QVector<QPair<QString, LogicStatement *> > mapping;

	mapping.append(QPair<QString, LogicStatement *>(getPredicateSymbolName(), this));
	mapping += getParameters()->getStringMapping(fullBracket);

	return mapping;
}

bool PredicateSymbolStatement::variableBounded(Variable *boundedVariable) {
	/* In the context of  */
	return getParameters()->variableBounded(boundedVariable);
}

void PredicateSymbolStatement::collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection) {
	/* No need to check function symbol name */
	getParameters()->collectFreeVariable(freeVariable, collection);
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

EqualityStatement::~EqualityStatement() {
	delete getLeftVariable();
	delete getRightVariable();
}

QVector<QPair<QString, LogicStatement *> > EqualityStatement::getStringMapping(bool) {
	QVector<QPair<QString, LogicStatement *> > mapping;

	/* Null Mapping because one Variable argument is meaningless for equivalence */
	mapping.append(QPair<QString, LogicStatement *>(getLeftVariable()->getName(), nullptr));
	mapping.append(QPair<QString, LogicStatement *>(QString(SYMBOL_EQUALS), this));
	mapping.append(QPair<QString, LogicStatement *>(getRightVariable()->getName(), nullptr));

	return mapping;
}

bool EqualityStatement::variableBounded(Variable *boundedVariable) {
	/* Need to check both left and right of "=" for bounded variable */
	return getLeftVariable()->variableBounded(boundedVariable) &&
			getRightVariable()->variableBounded(boundedVariable);
}

void EqualityStatement::collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection) {
	getLeftVariable()->collectFreeVariable(freeVariable, collection);
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
