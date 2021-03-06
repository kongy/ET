#include "ruleengine.hpp"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

using namespace AST;

#define GENERIC_RULE_PATH QString(":/equivalences.xml")
#define USER_DEFINED_RULE_PATH QString("userDefinedRules.xml")
#define NONE ("")
#define INITIAL_CHARACTER ('A')
#define FINAL_CHARACTER ('Z')

RuleEngine::RuleEngine()
{
	userDefinedRules = new QVector<LogicSet *>();
	allRules = new QVector<LogicSet *>();
}

RuleEngine::~RuleEngine()
{
	delete userDefinedRules;
	delete allRules;
}

bool RuleEngine::addRule(LogicStatement *formulaFrom, LogicStatement *formulaTo)
{
	if (formulaFrom->isFirstOrderLogic() || formulaTo->isFirstOrderLogic())
		return false;

	auto formulaSet = new LogicSet();
	formulaSet->add(formulaFrom);
	formulaSet->add(formulaTo);

	LogicSet *generalisedRule = generateRuleSet(formulaSet);
	delete formulaSet;

	for (LogicSet *existingRuleSet : *allRules)
		if (existingRuleSet->contains(generalisedRule)) {
			generalisedRule->deepDeleteContent();
			delete generalisedRule;
			return false;
		}

	userDefinedRules->append(generalisedRule);
	allRules->append(generalisedRule);
	flushNewRuleToXml();
	return true;
}

void RuleEngine::flushNewRuleToXml()
{
	QFile userFile(USER_DEFINED_RULE_PATH);

	userFile.open(QIODevice::WriteOnly | QIODevice::Text);

	QXmlStreamWriter xml(&userFile);
	xml.setAutoFormatting(true);

	xml.writeStartDocument();
	xml.writeStartElement("PropositionalEquivalences");

	for (LogicSet *ruleSet : *userDefinedRules) {
		xml.writeStartElement("EquivalentStatements");
		for (Rule *rule : *ruleSet->getSet()) {
			xml.writeStartElement("LogicStatement");
			rule->generateRule(&xml);
			xml.writeEndElement();
		}
		xml.writeEndElement();
	}

	xml.writeEndElement();
	xml.writeEndDocument();

	userFile.close();
}

void RuleEngine::parseRule(QString fromFilePath,
                           QVector<LogicSet *> *destinationRuleSet)
{
	QFile file(fromFilePath);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		if (fromFilePath != USER_DEFINED_RULE_PATH)
			QMessageBox::critical(nullptr, "Error", file.errorString());

		return;
	}

	QXmlStreamReader xml(&file);

	/* Avoid checking start Document in while loop */
	if (xml.readNext() != QXmlStreamReader::StartDocument)
		return;

	LogicSet *equivalenceSet;
	QString comments;

	do {
		if (xml.readNextStartElement() &&
		    xml.name() == "EquivalentStatements") {
			comments = xml.attributes().value("COMMENT").toString();
			equivalenceSet = processStatements(&xml);
			equivalenceSet->setComment(comments);
			destinationRuleSet->append(equivalenceSet);
		}
	} while (!xml.atEnd() && !xml.hasError());

	xml.clear();
	file.close();
}

QVector<LogicSet *> *RuleEngine::parseRuleXml()
{
	parseRule(USER_DEFINED_RULE_PATH, userDefinedRules);

	for (LogicSet *ruleSet : *userDefinedRules)
		allRules->append(ruleSet);

	parseRule(GENERIC_RULE_PATH, allRules);

	return allRules;
}

Rule *RuleEngine::processTruthStatement(QXmlStreamReader *xml)
{
	xml->skipCurrentElement();
	return new Truth();
}

Rule *RuleEngine::processFalsityStatement(QXmlStreamReader *xml)
{
	xml->skipCurrentElement();
	return new Falsity();
}

Rule *RuleEngine::processVariableStatement(QXmlStreamReader *xml)
{
	QString boundVariableName =
	    xml->attributes().value("NOT_OCCUR_FREE").toString();
	QString freeVariableName = xml->attributes().value("OCCUR_FREE").toString();
	QString notOccurVariableName =
	    xml->attributes().value("NOT_OCCUR").toString();
	QString mayOccurVariableName =
	    xml->attributes().value("MAY_OCCUR").toString();
	QString varName = xml->readElementText();

	Variable *statement = new Variable(varName);

	if (!freeVariableName.isEmpty())
		statement->setFreeVariable(freeVariableName);

	if (!boundVariableName.isEmpty())
		statement->setBoundedVariable(boundVariableName);

	if (!notOccurVariableName.isEmpty())
		statement->setNotOccurVariable(notOccurVariableName);

	if (!mayOccurVariableName.isEmpty())
		statement->setMayOccurVariable(mayOccurVariableName);

	return statement;
}

Rule *RuleEngine::processNotStatement(QXmlStreamReader *xml)
{
	/* Evaluate the inner expression inside NOT */
	Rule *nestedStatement = processLogicStatement(xml);

	/* The pointer should now point to just before </NOT>,
	 * we will therefore move it over to </NOT> */
	xml->skipCurrentElement();

	return new NotStatement(nestedStatement);
}

Rule *RuleEngine::processOrStatement(QXmlStreamReader *xml)
{

	/* Process one tree at a time, stopping at delimiter */
	Rule *leftStatement = processLogicStatement(xml);
	Rule *rightStatement = processLogicStatement(xml);

	/* Pointer moved to </Or> */
	xml->skipCurrentElement();

	return new OrStatement(leftStatement, rightStatement);
}

Rule *RuleEngine::processAndStatement(QXmlStreamReader *xml)
{
	/* Process one tree at a time, stopping at delimiter */
	Rule *leftStatement = processLogicStatement(xml);
	Rule *rightStatement = processLogicStatement(xml);

	/* Pointer moved to </And> */
	xml->skipCurrentElement();

	return new AndStatement(leftStatement, rightStatement);
}

Rule *RuleEngine::processIFFStatement(QXmlStreamReader *xml)
{
	/* Process one tree at a time, stopping at delimiter */
	Rule *leftStatement = processLogicStatement(xml);
	Rule *rightStatement = processLogicStatement(xml);

	/* Pointer moved to </IFF> */
	xml->skipCurrentElement();

	return new IffStatement(leftStatement, rightStatement);
}

Rule *RuleEngine::processImpliesStatement(QXmlStreamReader *xml)
{
	/* Process one tree at a time, stopping at delimiter */
	Rule *leftStatement = processLogicStatement(xml);
	Rule *rightStatement = processLogicStatement(xml);

	/* Pointer moved to </Implies> */
	xml->skipCurrentElement();

	return new ImpliesStatement(leftStatement, rightStatement);
}

Rule *RuleEngine::processForAllStatement(QXmlStreamReader *xml)
{
	/* Quantifier is processed as logicstatement, because it auto moves pointer
	 * to next start element and process
	 * stops at its delimiter */
	Variable *quantifier = dynamic_cast<Variable *>(processLogicStatement(xml));
	Rule *nestedStatement = processLogicStatement(xml);

	/* Moves Pointer to </ForAll> */
	xml->skipCurrentElement();

	return new ForAllStatement(quantifier, nestedStatement);
}

Rule *RuleEngine::processThereExistsStatement(QXmlStreamReader *xml)
{
	/* Quantifier is processed as logicstatement, because it auto moves pointer
	 * to next start element and process
	 * stops at its delimiter */
	Variable *quantifier = dynamic_cast<Variable *>(processLogicStatement(xml));
	Rule *nestedStatement = processLogicStatement(xml);

	/* Moves Pointer to </ThereExists> */
	xml->skipCurrentElement();

	return new ThereExistsStatement(quantifier, nestedStatement);
}

Rule *RuleEngine::processEqualityStatement(QXmlStreamReader *xml)
{
	/* Both variable of = is processed as logicstatement, because it auto moves
	 * pointer to next start element and
	 * process stops at its delimiter */
	Variable *leftVariable =
	    dynamic_cast<Variable *>(processLogicStatement(xml));
	Variable *rightVariable =
	    dynamic_cast<Variable *>(processLogicStatement(xml));

	/* Moves Pointer to </Equals> */
	xml->skipCurrentElement();

	return new EqualityStatement(leftVariable, rightVariable);
}

Rule *RuleEngine::processLogicStatement(QXmlStreamReader *xml)
{
	/* Move the pointer to the start tag */
	while (!xml->readNextStartElement())
		;

	QString tagName = xml->name().toString();

	if (tagName == "And")
		return processAndStatement(xml);
	else if (tagName == "Or")
		return processOrStatement(xml);
	else if (tagName == "Not")
		return processNotStatement(xml);
	else if (tagName == "IFF")
		return processIFFStatement(xml);
	else if (tagName == "Implies")
		return processImpliesStatement(xml);
	else if (tagName == "ID")
		return processVariableStatement(xml);
	else if (tagName == "Truth")
		return processTruthStatement(xml);
	else if (tagName == "Falsity")
		return processFalsityStatement(xml);
	else if (tagName == "ForAll")
		return processForAllStatement(xml);
	else if (tagName == "ThereExists")
		return processThereExistsStatement(xml);
	else if (tagName == "Equals")
		return processEqualityStatement(xml);
	else
		return nullptr;
}

LogicSet *RuleEngine::processStatements(QXmlStreamReader *xml)
{
	auto ruleSet = new LogicSet();
	Rule *rule;
	bool isLeibnizRule;

	for (xml->readNextStartElement(); xml->name() != "EquivalentStatements";
	     xml->readNextStartElement())
		if (xml->name() == "LogicStatement" &&
		    xml->tokenType() == QXmlStreamReader::StartElement) {
			isLeibnizRule =
			    !xml->attributes().value("FORWARD").toString().isEmpty();
			rule = processLogicStatement(xml);
			rule->setRuleType(isLeibnizRule);
			ruleSet->add(rule);
		}

	return ruleSet;
}

LogicSet *RuleEngine::generateRuleSet(LogicSet *rawEquivalenceFormulas)
{
	auto allVariableSet = new LogicSet();
	LogicSet *rawVariableSet;

	for (LogicStatement *rawFormula : *rawEquivalenceFormulas->getSet()) {
		rawVariableSet = rawFormula->getAllVariables();
		allVariableSet->add(rawVariableSet);
		delete rawVariableSet;
	}

	IDTable *idTable = generateRuleVariables(allVariableSet);
	delete allVariableSet;

	auto newRuleSet = new LogicSet();

	for (LogicStatement *rawFormula : *rawEquivalenceFormulas->getSet())
		newRuleSet->add(rawFormula->replace(idTable));

	idTable->deleteValues();
	delete idTable;

	return newRuleSet;
}

IDTable *RuleEngine::generateRuleVariables(LogicSet *rawVariables)
{
	auto idTable = new IDTable();
	short asciiCode = INITIAL_CHARACTER;

	for (LogicStatement *statement : *rawVariables->getSet()) {
		idTable->add(dynamic_cast<Variable *>(statement->clone()),
		             asciiToVariable(asciiCode));
		++asciiCode;
	}

	return idTable;
}

Variable *RuleEngine::asciiToVariable(const short asciiCode)
{
	QString stringName;
	const short finalAsciiCode = ((short)FINAL_CHARACTER) + 1;
	short times = asciiCode / finalAsciiCode;
	const QChar character = asciiCode % finalAsciiCode;

	do {
		stringName += character;
	} while (--times >= 0);

	auto variable = new Variable(stringName);
	return variable;
}
