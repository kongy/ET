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

/* Falsity Class */
QString Falsity::print() {
	return SYMBOL_FALSITY;
}

bool Falsity::isFirstOrderLogic() {
	return false;
}

/* Variable Class */
Variable::Variable(QString *name) {
	setName(name);
}

QString Variable::print() {
	return (getName());
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

/* OrStatement Class */
OrStatement::OrStatement(LogicStatement *left, LogicStatement *right) {
	setLeftStatement(left);
	setRightStatement(right);
}

QString OrStatement::symbol() {
	return SYMBOL_OR;
}

/* IffStatement Class */
IffStatement::IffStatement(LogicStatement *left, LogicStatement *right) {
	setLeftStatement(left);
	setRightStatement(right);
}

QString IffStatement::symbol() {
	return SYMBOL_IFF;
}

/* ImpliesStatement Class */
ImpliesStatement::ImpliesStatement(LogicStatement *left, LogicStatement *right) {
	setLeftStatement(left);
	setRightStatement(right);
}

QString ImpliesStatement::symbol() {
	return SYMBOL_IMPLIES;
}

/* FirstOrderStatement Class */
bool FirstOrderStatement::isFirstOrderLogic() {
	return true;
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

	if (rest != NULL) {
		return param + QString(", ") + getRemainingParameters()->print();
	}
	else {
		return param;
	}
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

QString PredicateSymbolStatement::print() {
	return getPredicateSymbolName() + QString("(") + getParameters()->print() + QString(")");
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

LogicStatement *AST::parse(QString expression) {
	yy_scan_string(expression.toLocal8Bit().data());
	yyparse();
	return entireStatement;
}
