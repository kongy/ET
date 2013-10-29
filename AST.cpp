#include "AST.hpp"
#include "symbol.hpp"
#include "parser.hpp"
#include "lexer.hpp"

using namespace AST;

extern LogicStatement *entireStatement;

/* Truth Class */
QString Truth::print() {
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

/* Falsity Class */
QString Falsity::print() {
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

/* Variable Class */
Variable::Variable(QString *name) {
	setName(name);
}

QString Variable::print() {
	return getName();
}

QString Variable::getName() {
	return *name;
}

void Variable::setName(QString *name) {
	this->name = name;
}

bool Variable::isFirstOrderLogic() {
	return false;
}

Symbol Variable::getSymbol() {
	return Symbol::VARIABLE_SYMBOL;
}

bool Variable::evaluate() {
	return value;
}

/* UnaryOpStatement Class (Virtual) */
void UnaryOpStatement::setStatement(LogicStatement *statement) {
	nestedStatement = statement;
}

LogicStatement * UnaryOpStatement::getStatement() {
	return nestedStatement;
}

/* NotStatement Class */
NotStatement::NotStatement(LogicStatement *statement) {
	setStatement(statement);
}

bool NotStatement::isFirstOrderLogic() {
	return getStatement()->isFirstOrderLogic();
}

QString NotStatement::print() {
	return QString(SYMBOL_NOT) + QString("(") +
			getStatement()->print() + QString(")");
}

Symbol NotStatement::getSymbol() {
	return Symbol::NOT_SYMBOL;
}

bool NotStatement::evaluate() {
	return !getStatement()->evaluate();
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

QString BinaryOpStatement::print() {
	return QString("(") + getLeftStatement()->print() + QString(")") +
			QString(" ") + symbol() + QString(" ") +
			QString("(") + getRightStatement()->print() + QString(")");
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

/* FirstOrderStatement Class */
bool FirstOrderStatement::isFirstOrderLogic() {
	return true;
}

bool FirstOrderStatement::evaluate() {
	return false;
}

/* ForAllStatement Class */
ForAllStatement::ForAllStatement(Variable *identifier, LogicStatement *forAllStatement) {
	setStatement(forAllStatement);
	setIdentifier(identifier);
}

void ForAllStatement::setIdentifier(Variable *identifier) {
	this->identifier = identifier;
}

QString ForAllStatement::print() {
	return QString(SYMBOL_FORALL) + QString("(")
			+ getStatement()->print() + QString(")");
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
ThereExistsStatement::ThereExistsStatement(Variable *identifier, LogicStatement *thereExistsStatement) {
	setStatement(thereExistsStatement);
	setIdentifier(identifier);
}

void ThereExistsStatement::setIdentifier(Variable *identifier) {
	this->identifier = identifier;
}

QString ThereExistsStatement::print() {
	return QString(SYMBOL_THEREEXISTS) + QString("(")
			+ getStatement()->print() + QString(")");
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

QString Parameters::print() {
	QString param = getParameter()->print();

	if (rest != NULL)
		return param + QString(", ") + getRemainingParameters()->print();
	else
		return param;
}

Symbol Parameters::getSymbol() {
	return Symbol::PARAMETERS_SYMBOL;
}

/* PredicateSymbolStatement Class */
PredicateSymbolStatement::PredicateSymbolStatement(Variable *predicateName, Parameters *params) {
	setPredicateSymbol(predicateName);
	setParameters(params);
}

bool Parameters::evaluate() {
	return false;
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

QString PredicateSymbolStatement::print() {
	return getPredicateSymbolName() + QString("(") + getParameters()->print() + QString(")");
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

QString EqualityStatement::print() {
	return getLeftVariable()->print() + QString(" ") + QString(SYMBOL_EQUALS) + QString(" ") + getRightVariable()->print();
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
	yy_scan_string(expression.toLocal8Bit().data());
	int result = yyparse();
	return result == 0 ? entireStatement : nullptr;
}
