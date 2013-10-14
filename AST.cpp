#include "AST.hpp"

using namespace AST;

/* Truth Class */
string Truth::print() {
	return "TRUTH";
}

bool Truth::isFirstOrderLogic() {
	return false;
}

/* Falsity Class */
string Falsity::print() {
	return "FALSITY";
}

bool Falsity::isFirstOrderLogic() {
	return false;
}

/* Variable Class */
Variable::Variable(string *name) {
	setName(name);
}

string Variable::print() {
	return (getName());
}

string Variable::getName() {
	return *name;
}

void Variable::setName(string *name) {
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

string NotStatement::print() {
	return string("NOT ") + string("(") +
			getStatement()->print() + string(")");
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

string BinaryOpStatement::print() {
	return string("(") + getLeftStatement()->print() + string(")")
			+ symbol() +
			string("(") + getRightStatement()->print() + string(")");
}

/* AndStatement Class */
AndStatement::AndStatement(LogicStatement *left, LogicStatement *right) {
	setLeftStatement(left);
	setRightStatement(right);
}

string AndStatement::symbol() {
	return " AND ";
}

/* OrStatement Class */
OrStatement::OrStatement(LogicStatement *left, LogicStatement *right) {
	setLeftStatement(left);
	setRightStatement(right);
}

string OrStatement::symbol() {
	return " OR ";
}

/* IffStatement Class */
IffStatement::IffStatement(LogicStatement *left, LogicStatement *right) {
	setLeftStatement(left);
	setRightStatement(right);
}

string IffStatement::symbol() {
	return " IFF ";
}

/* ImpliesStatement Class */
ImpliesStatement::ImpliesStatement(LogicStatement *left, LogicStatement *right) {
	setLeftStatement(left);
	setRightStatement(right);
}

string ImpliesStatement::symbol() {
	return " IMPLIES ";
}

/* FirstOrderStatement Class */
bool FirstOrderStatement::isFirstOrderLogic() {
	return true;
}

/* ForAllStatement Class */
ForAllStatement::ForAllStatement(LogicStatement *forAllStatement) {
	setStatement(forAllStatement);
}

string ForAllStatement::print() {
	return string("FORALL ") + string("(")
			+ getStatement()->print() +
			string(")");
}

LogicStatement * ForAllStatement::getStatement() {
	return statement;
}

void ForAllStatement::setStatement(LogicStatement *newstatement) {
	statement = newstatement;
}

/* ThereExistsStatement Class */
ThereExistsStatement::ThereExistsStatement(LogicStatement *thereExistsStatement) {
	setStatement(thereExistsStatement);
}

string ThereExistsStatement::print() {
	return string("THEREEXISTS ") + string("(")
			+ getStatement()->print() + string(")");
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

string Parameters::print() {
	string param = getParameter()->print();

	if (rest != NULL) {
		return param + string(", ") + getRemainingParameters()->print();
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

string PredicateSymbolStatement::getPredicateSymbolName() {
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

string PredicateSymbolStatement::print() {
	return getPredicateSymbolName() + string("(") + getParameters()->print() + string(")");
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

string EqualityStatement::print() {
	return getLeftVariable()->print() + " EQUALS " + getRightVariable()->print();
}
