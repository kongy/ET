#include "AST.hpp"
#include "symbol.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include "utility.hpp"

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

void LogicStatement::setRuleType(bool isLeibnizRule) {
	this->isLeibniz = isLeibnizRule;
}

bool LogicStatement::isLeibnizRule() {
	return isLeibniz;
}

LogicSet *LogicStatement::getAllVariables() {
	QVector<QVector<Variable *>*> *var_list = new QVector<QVector<Variable *> *>();

	collectVariables(var_list);

	LogicSet *variableSet = new LogicSet();

	if (!var_list->isEmpty())
		for (QVector<Variable *> *identicalVarList : *var_list)
			variableSet->add(identicalVarList->front());

	list_destroy(var_list);

	return variableSet;
}

LogicSet *LogicStatement::getExtraVariables(LogicStatement *other) {
	LogicSet *currentVariableSet = getAllVariables();
	LogicSet *otherVariableSet = other->getAllVariables();

	LogicSet *difference = currentVariableSet->diff(otherVariableSet);
	delete currentVariableSet;
	delete otherVariableSet;

	return difference;
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

void Truth::collectVariables(QVector<QVector<Variable *> *> *) {}

bool Truth::equals(LogicStatement *statement) {
	return statement->getSymbol() == getSymbol();
}

bool Truth::match(LogicStatement *matchingStatement, EquivalenceUtility *) {
	return getSymbol() == matchingStatement->getSymbol();
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

void Truth::candidateBoundVariables(LogicStatement *, LogicSet *) {}

bool Truth::notOccur(Variable *) {
	return true;
}

int Truth::numberOfLeibnizReplacedVariable(LogicStatement *other, EquivalenceUtility *) {
	if (getSymbol() != other->getSymbol())
		return -1;
	else
		return 0;
}

void Truth::replaceChildStatement(LogicStatement *, LogicStatement *) {}

QString Truth::XmlSymbol() {
	return XML_TRUTH_TAG;
}

void Truth::generateRule(QXmlStreamWriter *out) {
	out->writeEmptyElement(XmlSymbol());
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

void Falsity::collectVariables(QVector<QVector<Variable *> *> *) {}

bool Falsity::equals(LogicStatement *statement) {
	return statement->getSymbol() == getSymbol();
}

bool Falsity::match(LogicStatement *matchingStatement, EquivalenceUtility *) {
	return getSymbol() == matchingStatement->getSymbol();
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

void Falsity::candidateBoundVariables(LogicStatement *, LogicSet *) {}

bool Falsity::notOccur(Variable *) {
	return true;
}

int Falsity::numberOfLeibnizReplacedVariable(LogicStatement *other, EquivalenceUtility *) {
	if (getSymbol() != other->getSymbol())
		return -1;
	else
		return 0;
}

void Falsity::replaceChildStatement(LogicStatement *, LogicStatement *) {}

QString Falsity::XmlSymbol() {
	return XML_FALSITY_TAG;
}

void Falsity::generateRule(QXmlStreamWriter *out) {
	out->writeEmptyElement(XmlSymbol());
}

/* Variable Class */
Variable::Variable(QString *name) {
	setName(name);
	freeVariable = nullptr;
	boundedVariable = nullptr;
	notOccurVariable = nullptr;
	mayOccurVariable = nullptr;
}

Variable::~Variable() {
	if (freeVariable != nullptr)
		delete freeVariable;

	if (boundedVariable != nullptr)
		delete boundedVariable;

	if (mayOccurVariable != nullptr)
		delete mayOccurVariable;

	if (notOccurVariable != nullptr)
		delete notOccurVariable;
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
			dynamic_cast<Variable *>(statement)->getName() == getName();
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

bool Variable::match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility) {
	IDTable *idTable = matchingUtility->getIDTable();

	/* For Leibneiz rule */
	if (notOccurVariable != nullptr) {
		/* If did occur, because the rule is Leibneiz so we know the value of y in idTable */
		if (!matchingStatement->notOccur(dynamic_cast<Variable *>(idTable->valueOf(notOccurVariable))))
			return false;
	}

	/* For Leibneiz rule, we log the may occur variable in free Variable list */
	if (mayOccurVariable != nullptr) {
		matchingUtility->initFreeVariableList();
		matchingStatement->collectFreeVariable(dynamic_cast<Variable *>(idTable->valueOf(mayOccurVariable)), matchingUtility->getFreeVariableList());
	}

	/* NOT_OCCUR_FREE */
	if (boundedVariable != nullptr) {

		/* Obtain Mapping from nested bound identifier to its mapped variable */
		Variable *mappedVariable = dynamic_cast<Variable *>(idTable->valueOf(boundedVariable));

		/* Mapping known, need to confirm the variable is actually bounded */
		if (mappedVariable != nullptr) {
			/* Variable not bounded, matching failure */
			if (!matchingStatement->variableBounded(mappedVariable))
				return false;

			/* Variable bounded confirmed */

			/* Mapping not known, need to find possible candidate for the mapping */
		} else {
			/* Initialise candidate set with its representitive identifier */
			matchingUtility->initCandidateBoundVariableSet(boundedVariable);

			/* Log all possible bound candidates */
			matchingStatement->candidateBoundVariables(matchingStatement, matchingUtility->getCandidateBoundVariableSet());
		}
		/* OCCUR_FREE */
	} else if (freeVariable != nullptr) {

		/* Get the mapping from identifier of freeVariable to its corresponding Variable, the mapping is assumed to be known */
		Variable *mappedVariable = dynamic_cast<Variable *>(idTable->valueOf(freeVariable));

		/* Initialise free variable list, if not already initialised(checked by function) */
		matchingUtility->initFreeVariableList();

		/* Log all variables that are free in matchingStatement and equals to mappedVariable */
		matchingStatement->collectFreeVariable(mappedVariable, matchingUtility->getFreeVariableList());

	}

	return idTable->add(this, matchingStatement);
}

LogicStatement* Variable::replace(IDTable *idTable) {
	LogicStatement *replacedStatement = idTable->valueOf(this)->clone();
	delete this;
	return replacedStatement;
}

LogicStatement* Variable::clone() {
	return new Variable(new QString(getName()));
}

bool Variable::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			getName() == dynamic_cast<Variable &>(other).getName();
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

void Variable::setBoundedVariable(QString name) {
	boundedVariable = new Variable(&name);
}

void Variable::setFreeVariable(QString name) {
	freeVariable = new Variable(&name);
}

Variable *Variable::getBoundedVariable() {
	return boundedVariable;
}

Variable *Variable::getFreeVariable() {
	return freeVariable;
}

void Variable::candidateBoundVariables(LogicStatement *, LogicSet *) {}

bool Variable::notOccur(Variable *var) {
	return !this->equals(var);
}

void Variable::setMayOccurVariable(QString name) {
	mayOccurVariable = new Variable(&name);
}

void Variable::setNotOccurVariable(QString name) {
	notOccurVariable = new Variable(&name);
}

Variable *Variable::getMayOccurVariable() {
	return mayOccurVariable;
}

Variable *Variable::getNotOccurVariable() {
	return notOccurVariable;
}

int Variable::numberOfLeibnizReplacedVariable(LogicStatement *other, EquivalenceUtility *matchingUtility) {
	if (!getSymbol() == other->getSymbol())
		return -1;

	bool currentVariableInFreeList = matchingUtility->inFreeVariableListReferencewise(this);
	bool otherVariableInFreeList = matchingUtility->inFreeVariableListReferencewise(dynamic_cast<Variable *>(other));
	bool sameVariableName = this->equals(other);

	/* Both are either the free x or free y */
	if (currentVariableInFreeList && otherVariableInFreeList) {
		/* Not replaced */
		if  (sameVariableName)
			return 0;
		/* Replaced */
		else
			return 1;

	/* Both not part of free variables */
	} else if (!currentVariableInFreeList && !otherVariableInFreeList) {
		/* Matched */
		if (sameVariableName)
			return 0;
		/* Mismatched, different AST */
		else
			return -1;

	/* One of them is free whilst other isn't */
	} else
		return -1;
}

void Variable::replaceChildStatement(LogicStatement *, LogicStatement *) {}

QString Variable::XmlSymbol() {
	return XML_IDENTIFER_TAG;
}

void Variable::generateRule(QXmlStreamWriter *out) {
	out->writeTextElement(XmlSymbol(), getName());
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
			getStatement()->equals(dynamic_cast<UnaryOpStatement *>(statement)->getStatement());
}

bool UnaryOpStatement::match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility) {
	return getSymbol() == matchingStatement->getSymbol() &&
			getStatement()->match(dynamic_cast<UnaryOpStatement *>(matchingStatement)->getStatement(), matchingUtility);
}

LogicStatement* UnaryOpStatement::replace(IDTable *idTable) {
	setStatement(getStatement()->replace(idTable));
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
			*getStatement() == *dynamic_cast<UnaryOpStatement &>(other).getStatement();
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

void UnaryOpStatement::candidateBoundVariables(LogicStatement *rootStatement, LogicSet *boundSet) {
	getStatement()->candidateBoundVariables(rootStatement, boundSet);
}

bool UnaryOpStatement::notOccur(Variable *var) {
	return getStatement()->notOccur(var);
}

int UnaryOpStatement::numberOfLeibnizReplacedVariable(LogicStatement *other, EquivalenceUtility *matchingUtility) {
	if (getSymbol() != other->getSymbol())
		return -1;

	return getStatement()->numberOfLeibnizReplacedVariable(dynamic_cast<UnaryOpStatement *>(other)->getStatement(), matchingUtility);
}

void UnaryOpStatement::replaceChildStatement(LogicStatement *oldChildFormula, LogicStatement *newChildFormula) {
	LogicStatement *childStatement = getStatement();

	if (childStatement == oldChildFormula)
		setStatement(newChildFormula);
	else
		childStatement->replaceChildStatement(oldChildFormula, newChildFormula);
}

void UnaryOpStatement::generateRule(QXmlStreamWriter *out) {
	out->writeStartElement(XmlSymbol());
	getStatement()->generateRule(out);
	out->writeEndElement();
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

QString NotStatement::XmlSymbol() {
	return XML_NOT_TAG;
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
			getLeftStatement()->equals(dynamic_cast<BinaryOpStatement *>(statement)->getLeftStatement()) &&
			getRightStatement()->equals(dynamic_cast<BinaryOpStatement *>(statement)->getRightStatement());
}

bool BinaryOpStatement::match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility) {
	return getSymbol() == matchingStatement->getSymbol() &&
			getLeftStatement()->match((dynamic_cast<BinaryOpStatement *>(matchingStatement))->getLeftStatement(), matchingUtility) &&
			getRightStatement()->match((dynamic_cast<BinaryOpStatement *>(matchingStatement))->getRightStatement(), matchingUtility);
}

LogicStatement* BinaryOpStatement::replace(IDTable *idTable) {
	setLeftStatement(getLeftStatement()->replace(idTable));
	setRightStatement(getRightStatement()->replace(idTable));
	return this;
}

bool BinaryOpStatement::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			*getLeftStatement() == *dynamic_cast<BinaryOpStatement &>(other).getLeftStatement() &&
			*getRightStatement() == *dynamic_cast<BinaryOpStatement &>(other).getRightStatement();
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

void BinaryOpStatement::candidateBoundVariables(LogicStatement *rootStatement, LogicSet *boundSet) {
	getLeftStatement()->candidateBoundVariables(rootStatement, boundSet);
	getRightStatement()->candidateBoundVariables(rootStatement, boundSet);
}

bool BinaryOpStatement::notOccur(Variable *var) {
	return getLeftStatement()->notOccur(var) &&
			getRightStatement()->notOccur(var);
}

int BinaryOpStatement::numberOfLeibnizReplacedVariable(LogicStatement *other, EquivalenceUtility *matchingUtility) {
	if (getSymbol() != other->getSymbol())
		return -1;

	BinaryOpStatement *castedOther = dynamic_cast<BinaryOpStatement *>(other);

	int leftReplaced = getLeftStatement()->numberOfLeibnizReplacedVariable(castedOther->getLeftStatement(), matchingUtility);

	if (leftReplaced == -1)
		return -1;

	int rightReplaced = getRightStatement()->numberOfLeibnizReplacedVariable(castedOther->getRightStatement(), matchingUtility);

	if (rightReplaced == -1)
		return -1;

	return leftReplaced + rightReplaced;
}

void BinaryOpStatement::replaceChildStatement(LogicStatement *oldChildFormula, LogicStatement *newChildFormula) {
	LogicStatement *leftChild = getLeftStatement();
	LogicStatement *rightChild = getRightStatement();

	if (leftChild == oldChildFormula)
		setLeftStatement(newChildFormula);
	else if (rightChild == oldChildFormula)
		setRightStatement(newChildFormula);
	else {
		leftChild->replaceChildStatement(oldChildFormula, newChildFormula);
		rightChild->replaceChildStatement(oldChildFormula, newChildFormula);
	}
}

void BinaryOpStatement::generateRule(QXmlStreamWriter *out) {
	out->writeStartElement(XmlSymbol());
	getLeftStatement()->generateRule(out);
	getRightStatement()->generateRule(out);
	out->writeEndElement();;
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

QString AndStatement::XmlSymbol() {
	return XML_AND_TAG;
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

QString OrStatement::XmlSymbol() {
	return XML_OR_TAG;
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

QString IffStatement::XmlSymbol() {
	return XML_IFF_TAG;
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

bool ImpliesStatement::match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility) {
	bool matched = BinaryOpStatement::match(matchingStatement, matchingUtility);

	/* has the shape of (x=y) IMPLIES (A IFF B) where x is free  */
	if (isLeibnizRule()) {
		/* Confirmed x is free in A and y is free in B and y does not occur in A, so the idTable has mapping
		 * for x,y,A and B */
		if (matched) {
			IDTable *idTable = matchingUtility->getIDTable();
			QString *A = new QString("A");
			QString *B = new QString("B");
			Variable *VAR_A = new Variable(A);
			Variable *VAR_B = new Variable(B);
			LogicStatement *matching_A = idTable->valueOf(VAR_A);
			LogicStatement *matching_B = idTable->valueOf(VAR_B);
			delete A;
			delete B;
			delete VAR_A;
			delete VAR_B;

			/* When one or more free x gets replaced by y, Leibniz rule matched, if none gets replaced or structural
			 * mismatch, then failed to match leibniz rule */
			return matching_A->numberOfLeibnizReplacedVariable(matching_B, matchingUtility) > 0;
		}
	}

	return matched;
}

QString ImpliesStatement::XmlSymbol() {
	return XML_IMPLIES_TAG;
}

/* FirstOrderStatement Class */
bool FirstOrderStatement::isFirstOrderLogic() {
	return true;
}

bool FirstOrderStatement::evaluate() {
	return false;
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

void ForAllStatement::collectVariables(QVector<QVector<Variable *> *> *var_list) {
	getQuantifier()->collectVariables(var_list);
	getStatement()->collectVariables(var_list);
}

bool ForAllStatement::equals(LogicStatement *other) {
	return getSymbol() == other->getSymbol() &&
			getQuantifier()->equals(dynamic_cast<ForAllStatement *>(other)->getQuantifier()) &&
			getStatement()->equals(dynamic_cast<ForAllStatement *>(other)->getStatement());
}

LogicStatement *ForAllStatement::clone() {
	return new ForAllStatement(dynamic_cast<Variable *>(getQuantifier()->clone()), getStatement()->clone());
}

bool ForAllStatement::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			*getQuantifier() == *dynamic_cast<ForAllStatement &>(other).getQuantifier() &&
			*getStatement() == *dynamic_cast<ForAllStatement &>(other).getStatement();
}

void ForAllStatement::candidateBoundVariables(LogicStatement *rootStatement, LogicSet *boundSet) {
	Variable *quantifier = getQuantifier();

	if (rootStatement->variableBounded(quantifier))
		boundSet->add(quantifier);

	getStatement()->candidateBoundVariables(rootStatement, boundSet);
}

bool ForAllStatement::match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility) {
	if (getSymbol() != matchingStatement->getSymbol())
		return false;

	ForAllStatement *castedMatchingStatement = dynamic_cast<ForAllStatement *>(matchingStatement);
	Variable *loggedBoundVariable = matchingUtility->getBoundVariable();

	/* In case where the bound variable has been logged for candidate search an Example would be
	 * FORALL(x)(A V B)=A V FORALL(x)(B) where x occurs free in A, in the latter rule, the A will be matched and
	 * candidates for x would be logged, and the second half must confirm one of the logged candidates for x is
	 * the current one it is matching onto */
	if (loggedBoundVariable != nullptr && loggedBoundVariable->equals(getQuantifier())) {
		/* Current quantifier in actual statement is not matched to any candidate */
		if (!matchingUtility->inCandidateBoundVariableSet(castedMatchingStatement->getQuantifier()))
			return false;

		/* When control reaches this block, the binding has been confirmed and candidate set is no longer needed,
		 * in fact keeping the information would confuse the matching system */
		matchingUtility->deleteAuxiliaryItems();
		matchingUtility->resetAuxiliaryItems();
		/* Used for renaming bound variables, inserting the x in FORALL(x)A to the free list to be replaced later on */
	} else if (getStatement()->getSymbol() == VARIABLE_SYMBOL &&
			   dynamic_cast<Variable *>(getStatement())->getFreeVariable() != nullptr) {
		matchingUtility->initFreeVariableList();
		matchingUtility->getFreeVariableList()->append(getQuantifier());
	}

	/* We proceed adding the quantifier to IDTable as usual and keep matching */
	return getQuantifier()->match(castedMatchingStatement->getQuantifier(), matchingUtility) &&
			getStatement()->match(castedMatchingStatement->getStatement(), matchingUtility);
}

LogicStatement *ForAllStatement::replace(IDTable *idTable) {
	setIdentifier(dynamic_cast<Variable *>(getQuantifier()->replace(idTable)));
	setStatement(getStatement()->replace(idTable));
	return this;
}

bool ForAllStatement::notOccur(Variable *var) {
	return getQuantifier()->notOccur(var) &&
			getStatement()->notOccur(var);
}

int ForAllStatement::numberOfLeibnizReplacedVariable(LogicStatement *other, EquivalenceUtility *matchingUtility) {
	if (getSymbol() != other->getSymbol())
		return -1;

	ForAllStatement *castedOther = dynamic_cast<ForAllStatement *>(other);

	int quantifierReplaced = getQuantifier()->numberOfLeibnizReplacedVariable(castedOther->getQuantifier(), matchingUtility);

	if (quantifierReplaced == -1)
		return -1;

	int statementReplaced = getStatement()->numberOfLeibnizReplacedVariable(castedOther->getStatement(), matchingUtility);

	if (statementReplaced == -1)
		return -1;

	return quantifierReplaced + statementReplaced;
}

void ForAllStatement::replaceChildStatement(LogicStatement *oldChildFormula, LogicStatement *newChildFormula) {
	LogicStatement *child = getStatement();

	if (child == oldChildFormula)
		setStatement(newChildFormula);
	else
		child->replaceChildStatement(oldChildFormula, newChildFormula);
}

QString ForAllStatement::XmlSymbol() {
	return XML_FORALL_TAG;
}

void ForAllStatement::generateRule(QXmlStreamWriter *out) {
	out->writeStartElement(XmlSymbol());
	getQuantifier()->generateRule(out);
	getStatement()->generateRule(out);
	out->writeEndElement();
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

void ThereExistsStatement::collectVariables(QVector<QVector<Variable *> *> *var_list) {
	getQuantifier()->collectVariables(var_list);
	getStatement()->collectVariables(var_list);
}

bool ThereExistsStatement::equals(LogicStatement *other) {
	return getSymbol() == other->getSymbol() &&
			getQuantifier()->equals(dynamic_cast<ThereExistsStatement *>(other)->getQuantifier()) &&
			getStatement()->equals(dynamic_cast<ThereExistsStatement *>(other)->getStatement());
}

LogicStatement *ThereExistsStatement::clone() {
	return new ThereExistsStatement(dynamic_cast<Variable *>(getQuantifier()->clone()), getStatement()->clone());
}

bool ThereExistsStatement::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			*getQuantifier() == *dynamic_cast<ThereExistsStatement &>(other).getQuantifier() &&
			*getStatement() == *dynamic_cast<ThereExistsStatement &>(other).getStatement();
}

void ThereExistsStatement::candidateBoundVariables(LogicStatement *rootStatement, LogicSet *boundSet) {
	Variable *quantifier = getQuantifier();

	if (rootStatement->variableBounded(quantifier))
		boundSet->add(quantifier);

	getStatement()->candidateBoundVariables(rootStatement, boundSet);
}

bool ThereExistsStatement::match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility) {
	if (getSymbol() != matchingStatement->getSymbol())
		return false;

	ThereExistsStatement *castedMatchingStatement = dynamic_cast<ThereExistsStatement *>(matchingStatement);
	Variable *loggedBoundVariable = matchingUtility->getBoundVariable();

	/* In case where the bound variable has been logged for candidate search an Example would be
	 * THEREEXIST(x)(A ^ B)=A ^ THEREEXIST(x)(B) where x occurs free in A, in the latter rule, the A will be matched and
	 * candidates for x would be logged, and the second half must confirm one of the logged candidates for x is
	 * the current one it is matching onto */
	if (loggedBoundVariable != nullptr && loggedBoundVariable->equals(getQuantifier())) {
		/* Current quantifier in actual statement is not matched to any candidate */
		if (!matchingUtility->inCandidateBoundVariableSet(castedMatchingStatement->getQuantifier()))
			return false;

		/* When control reaches this block, the binding has been confirmed and candidate set is no longer needed,
		 * in fact keeping the information would confuse the matching system */
		matchingUtility->resetAuxiliaryItems();

		/* Used for renaming bound variables, inserting the x in FORALL(x)A to the free list to be replaced later on */
	} else if (getStatement()->getSymbol() == VARIABLE_SYMBOL &&
			   dynamic_cast<Variable *>(getStatement())->getFreeVariable() != nullptr) {
		matchingUtility->initFreeVariableList();
		matchingUtility->getFreeVariableList()->append(getQuantifier());
	}


	/* We proceed adding the quantifier to IDTable as usual and keep matching */
	return getQuantifier()->match(castedMatchingStatement->getQuantifier(), matchingUtility) &&
			getStatement()->match(castedMatchingStatement->getStatement(), matchingUtility);
}

LogicStatement *ThereExistsStatement::replace(IDTable *idTable) {
	setIdentifier(dynamic_cast<Variable *>(getQuantifier()->replace(idTable)));
	setStatement(getStatement()->replace(idTable));
	return this;
}

bool ThereExistsStatement::notOccur(Variable *var) {
	return getQuantifier()->notOccur(var) &&
			getStatement()->notOccur(var);
}

int ThereExistsStatement::numberOfLeibnizReplacedVariable(LogicStatement *other, EquivalenceUtility *matchingUtility) {
	if (getSymbol() != other->getSymbol())
		return -1;

	ThereExistsStatement *castedOther = dynamic_cast<ThereExistsStatement *>(other);

	int quantifierReplaced = getQuantifier()->numberOfLeibnizReplacedVariable(castedOther->getQuantifier(), matchingUtility);

	if (quantifierReplaced == -1)
		return -1;

	int statementReplaced = getStatement()->numberOfLeibnizReplacedVariable(castedOther->getStatement(), matchingUtility);

	if (statementReplaced == -1)
		return -1;

	return quantifierReplaced + statementReplaced;
}

void ThereExistsStatement::replaceChildStatement(LogicStatement *oldChildFormula, LogicStatement *newChildFormula) {
	LogicStatement *child = getStatement();

	if (child == oldChildFormula)
		setStatement(newChildFormula);
	else
		child->replaceChildStatement(oldChildFormula, newChildFormula);
}

QString ThereExistsStatement::XmlSymbol() {
	return XML_THEREEXISTS_TAG;
}

void ThereExistsStatement::generateRule(QXmlStreamWriter *out) {
	out->writeStartElement(XmlSymbol());
	getQuantifier()->generateRule(out);
	getStatement()->generateRule(out);
	out->writeEndElement();
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

void Parameters::collectVariables(QVector<QVector<Variable *> *> *var_list) {
	getParameter()->collectVariables(var_list);

	Parameters *remainingParams = getRemainingParameters();

	if (remainingParams != nullptr)
		remainingParams->collectVariables(var_list);
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

bool Parameters::match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility) {
	if (getSymbol() != matchingStatement->getSymbol())
		return false;

	Parameters *castedMatchingStatement = dynamic_cast<Parameters *>(matchingStatement);

	if (!getParameter()->match(castedMatchingStatement->getParameter(), matchingUtility))
		return false;

	Parameters *castedRemainingParams = castedMatchingStatement->getRemainingParameters();
	Parameters *remainingParams = getRemainingParameters();

	if (remainingParams == nullptr && castedRemainingParams == nullptr)
		return true;
	else if (remainingParams == nullptr || castedRemainingParams == nullptr)
		return false;
	else
		return remainingParams->match(castedRemainingParams, matchingUtility);
}

LogicStatement* Parameters::clone() {
	Parameters *newRemainingParameters = nullptr;
	Parameters *remainingParameters = getRemainingParameters();

	if (remainingParameters != nullptr)
		newRemainingParameters = dynamic_cast<Parameters *>(remainingParameters->clone());

	return new Parameters(dynamic_cast<Variable *>(getParameter()->clone()), newRemainingParameters);
}

LogicStatement *Parameters::replace(IDTable *idTable) {
	setParameter(dynamic_cast<Variable *>(getParameter()->replace(idTable)));

	Parameters *remainingParameter = getRemainingParameters();
	if (remainingParameter != nullptr)
		setRemainingParameters(dynamic_cast<Parameters *>(getRemainingParameters()->replace(idTable)));

	return this;
}

bool Parameters::operator==(LogicStatement &other) {
	/* Must have same symbol and parameter match to proceed */
	if (!(getSymbol() == other.getSymbol() &&
		  *getParameter() == *dynamic_cast<Parameters &>(other).getParameter()))
		return false;

	/* Symbol and type matched */
	Parameters *otherCasted = &dynamic_cast<Parameters &>(other);
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

void Parameters::candidateBoundVariables(LogicStatement *, LogicSet *) {}

bool Parameters::notOccur(Variable *var) {
	if (!getParameter()->notOccur(var))
		return false;

	Parameters *remainingParams = getRemainingParameters();

	if (remainingParams != nullptr)
		return remainingParams->notOccur(var);

	return true;
}

int Parameters::numberOfLeibnizReplacedVariable(LogicStatement *other, EquivalenceUtility *matchingUtility) {
	if (getSymbol() != other->getSymbol())
		return -1;

	Parameters *castedOther = dynamic_cast<Parameters *>(other);

	int parameterReplaced = getParameter()->numberOfLeibnizReplacedVariable(castedOther->getParameter(), matchingUtility);

	if (parameterReplaced == -1)
		return -1;

	Parameters *remainingParams = getRemainingParameters();
	Parameters *otherRemainingParams = castedOther->getRemainingParameters();

	/* Both reached end */
	if (remainingParams == nullptr && otherRemainingParams == nullptr)
		return parameterReplaced;

	/* Both has remaining */
	else if (remainingParams != nullptr && otherRemainingParams != nullptr) {
		int paramsReplaced = remainingParams->numberOfLeibnizReplacedVariable(otherRemainingParams, matchingUtility);

		if (paramsReplaced == -1)
			return -1;

		return parameterReplaced + paramsReplaced;

	/* One has reached the end whilst other has remaining, structure mismatch */
	} else
		return -1;
}

void Parameters::replaceChildStatement(LogicStatement *, LogicStatement *) {}

QString Parameters::XmlSymbol() {
	/* Unused */
	return "";
}

void Parameters::generateRule(QXmlStreamWriter *) {}

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
	mapping.append(QPair<QString, LogicStatement *>("(", nullptr));
	mapping += getParameters()->getStringMapping(fullBracket);
	mapping.append(QPair<QString, LogicStatement *>(")", nullptr));

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

void PredicateSymbolStatement::collectVariables(QVector<QVector<Variable *> *> *var_list) {
	getPredicateSymbol()->collectVariables(var_list);
	getParameters()->collectVariables(var_list);
}

bool PredicateSymbolStatement::equals(LogicStatement *other) {
	return getSymbol() == other->getSymbol() &&
			getPredicateSymbol()->equals(dynamic_cast<PredicateSymbolStatement *>(other)->getPredicateSymbol()) &&
			getParameters()->equals(dynamic_cast<PredicateSymbolStatement *>(other)->getParameters());
}

LogicStatement *PredicateSymbolStatement::clone() {
	return new PredicateSymbolStatement(dynamic_cast<Variable *>(getPredicateSymbol()->clone()),
										dynamic_cast<Parameters *>(getParameters()->clone()));
}

bool PredicateSymbolStatement::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			*getPredicateSymbol() == *dynamic_cast<PredicateSymbolStatement &>(other).getPredicateSymbol() &&
			*getParameters() == *dynamic_cast<PredicateSymbolStatement &>(other).getParameters();
}

void PredicateSymbolStatement::candidateBoundVariables(LogicStatement *, LogicSet *) {}

bool PredicateSymbolStatement::match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility) {
	return getSymbol() == matchingStatement->getSymbol() &&
			getPredicateSymbol()->match(dynamic_cast<PredicateSymbolStatement *>(matchingStatement)->getPredicateSymbol(), matchingUtility) &&
			getParameters()->match(dynamic_cast<PredicateSymbolStatement *>(matchingStatement)->getParameters(), matchingUtility);
}

LogicStatement *PredicateSymbolStatement::replace(IDTable *idTable) {
	/* Predicate Symbol (function symbol) should not be replaced in any instance */
	this->setParameters(dynamic_cast<Parameters *>(getParameters()->replace(idTable)));
	return this;
}

bool PredicateSymbolStatement::notOccur(Variable *var) {
	return getPredicateSymbol()->notOccur(var) &&
			getParameters()->notOccur(var);
}

int PredicateSymbolStatement::numberOfLeibnizReplacedVariable(LogicStatement *other, EquivalenceUtility *matchingUtility) {
	if (getSymbol() != other->getSymbol())
		return -1;

	PredicateSymbolStatement *otherCasted = dynamic_cast<PredicateSymbolStatement *>(other);

	int predicateSymbolReplaced = getPredicateSymbol()->numberOfLeibnizReplacedVariable(otherCasted->getPredicateSymbol(), matchingUtility);

	if (predicateSymbolReplaced == -1)
		return -1;

	int parametersReplaced = getParameters()->numberOfLeibnizReplacedVariable(otherCasted->getParameters(), matchingUtility);

	if (parametersReplaced == -1)
		return -1;

	return predicateSymbolReplaced + parametersReplaced;
}

void PredicateSymbolStatement::replaceChildStatement(LogicStatement *, LogicStatement *) {}

QString PredicateSymbolStatement::XmlSymbol() {
	/* Unused */
	return "";
}

void PredicateSymbolStatement::generateRule(QXmlStreamWriter *) {}

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

void EqualityStatement::collectVariables(QVector<QVector<Variable *> *> *var_list) {
	getLeftVariable()->collectVariables(var_list);
	getRightVariable()->collectVariables(var_list);
}

bool EqualityStatement::equals(LogicStatement *other) {
	return getSymbol() == other->getSymbol() &&
			getLeftVariable()->equals(dynamic_cast<EqualityStatement *>(other)->getLeftVariable()) &&
			getRightVariable()->equals(dynamic_cast<EqualityStatement *>(other)->getRightVariable());
}

LogicStatement *EqualityStatement::clone() {
	return new EqualityStatement(dynamic_cast<Variable *>(getLeftVariable()->clone()),
								 dynamic_cast<Variable *>(getRightVariable()->clone()));
}

bool EqualityStatement::operator==(LogicStatement &other) {
	return getSymbol() == other.getSymbol() &&
			*getLeftVariable() == *dynamic_cast<EqualityStatement &>(other).getLeftVariable() &&
			*getRightVariable() == *dynamic_cast<EqualityStatement &>(other).getRightVariable();
}

void EqualityStatement::candidateBoundVariables(LogicStatement *, LogicSet *) {}

bool EqualityStatement::match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility) {
	return getSymbol() == matchingStatement->getSymbol() &&
			getLeftVariable()->match(dynamic_cast<EqualityStatement *>(matchingStatement)->getLeftVariable(), matchingUtility) &&
			getRightVariable()->match(dynamic_cast<EqualityStatement *>(matchingStatement)->getRightVariable(), matchingUtility);
}

LogicStatement *EqualityStatement::replace(IDTable *idTable) {
	setLeftVariable(dynamic_cast<Variable *>(getLeftVariable()->replace(idTable)));
	setRightVariable(dynamic_cast<Variable *>(getLeftVariable()->replace(idTable)));
	return this;
}

bool EqualityStatement::notOccur(Variable *var) {
	return getLeftVariable()->notOccur(var) &&
			getRightVariable()->notOccur(var);
}

int EqualityStatement::numberOfLeibnizReplacedVariable(LogicStatement *other, EquivalenceUtility *matchingUtility) {
	if (getSymbol() != other->getSymbol())
		return -1;

	EqualityStatement *castedOther = dynamic_cast<EqualityStatement *>(other);

	int leftVariableReplaced = getLeftVariable()->numberOfLeibnizReplacedVariable(castedOther->getLeftVariable(), matchingUtility);

	if (leftVariableReplaced == -1)
		return -1;

	int rightVariableReplaced = getRightVariable()->numberOfLeibnizReplacedVariable(castedOther->getRightVariable(), matchingUtility);

	if (rightVariableReplaced == -1)
		return -1;

	return leftVariableReplaced + rightVariableReplaced;
}

void EqualityStatement::replaceChildStatement(LogicStatement *, LogicStatement *) {}

QString EqualityStatement::XmlSymbol() {
	return XML_EQUALS_TAG;
}

void EqualityStatement::generateRule(QXmlStreamWriter *out) {
	out->writeStartElement(XmlSymbol());
	getLeftVariable()->generateRule(out);
	getRightVariable()->generateRule(out);
	out->writeEndElement();
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
