#include "ruleparser.hpp"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

using namespace AST;

QVector<LogicSet *> *RuleParser::parseRuleTxt() {
	QFile file(":/equivalences.txt");

	if(!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(0, "Error", file.errorString());
		return nullptr;
	}

	QTextStream in(&file);

	QString line;
	QStringList fields;
	LogicSet *rule_set;
	QVector<LogicSet *> *rules = new QVector<LogicSet *>();

	while(!in.atEnd()) {

		line = in.readLine();
		fields = line.split('\t');

		rule_set = new LogicSet();
		rules->push_back(rule_set);

		for (QString rule : fields)
			rule_set->add(parse(rule));
	}

	file.close();
	return rules;
}

QVector<LogicSet *> *RuleParser::parseRuleXml() {
    QFile file(":/equivalences.xml");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0, "Error", file.errorString());
        return nullptr;
    }

    QXmlStreamReader xml(&file);

    /* Avoid checking start Document in while loop */
    if (xml.readNext() != QXmlStreamReader::StartDocument) {
        QMessageBox::critical(0, "Error", "Start Document not present at start");
        return nullptr;
    }

    /* Initialise to get ready for parsing */
    QVector<LogicSet *> *rules = new QVector<LogicSet *>();

    do {
        if (xml.readNextStartElement() && xml.name() == "EquivalentStatements")
            rules->append(processStatements(&xml));
    } while(!xml.atEnd() && !xml.hasError());

    xml.clear();
    file.close();
    return rules;
}

Rule *RuleParser::processTruthStatement(QXmlStreamReader *xml) {
    xml->skipCurrentElement();
    return new Truth();
}

Rule *RuleParser::processFalsityStatement(QXmlStreamReader *xml) {
    xml->skipCurrentElement();
    return new Falsity();
}

Rule *RuleParser::processVariableStatement(QXmlStreamReader *xml) {
    QString boundVariableName = xml->attributes().value("NOT_OCCUR_FREE").toString();
    QString freeVariableName = xml->attributes().value("OCCUR_FREE").toString();

    Variable *statement = new Variable(new QString(xml->readElementText()));

    if (freeVariableName != "")
        statement->setFreeVariable(freeVariableName);

    if (boundVariableName != "")
        statement->setBoundedVariable(boundVariableName);

    return statement;
}

Rule *RuleParser::processNotStatement(QXmlStreamReader *xml) {
    /* Evaluate the inner expression inside NOT */
    Rule *nestedStatement = processLogicStatement(xml);

    /* The pointer should now point to just before </NOT>,
     * we will therefore move it over to </NOT> */
    xml->skipCurrentElement();

    return new NotStatement(nestedStatement);
}

Rule *RuleParser::processOrStatement(QXmlStreamReader *xml) {

    /* Process one tree at a time, stopping at delimiter */
    Rule *leftStatement = processLogicStatement(xml);
    Rule *rightStatement = processLogicStatement(xml);

    /* Pointer moved to </Or> */
    xml->skipCurrentElement();

    return new OrStatement(leftStatement, rightStatement);
}

Rule *RuleParser::processAndStatement(QXmlStreamReader *xml) {
    /* Process one tree at a time, stopping at delimiter */
    Rule *leftStatement = processLogicStatement(xml);
    Rule *rightStatement = processLogicStatement(xml);

    /* Pointer moved to </And> */
    xml->skipCurrentElement();

    return new AndStatement(leftStatement, rightStatement);
}

Rule *RuleParser::processIFFStatement(QXmlStreamReader *xml) {
    /* Process one tree at a time, stopping at delimiter */
    Rule *leftStatement = processLogicStatement(xml);
    Rule *rightStatement = processLogicStatement(xml);

    /* Pointer moved to </IFF> */
    xml->skipCurrentElement();

    return new IffStatement(leftStatement, rightStatement);
}

Rule *RuleParser::processImpliesStatement(QXmlStreamReader *xml) {
    /* Process one tree at a time, stopping at delimiter */
    Rule *leftStatement = processLogicStatement(xml);
    Rule *rightStatement = processLogicStatement(xml);

    /* Pointer moved to </Implies> */
    xml->skipCurrentElement();

    return new ImpliesStatement(leftStatement, rightStatement);
}

Rule *RuleParser::processForAllStatement(QXmlStreamReader *xml) {
    /* Quantifier is processed as logicstatement, because it auto moves pointer to next start element and process
     * stops at its delimiter */
    Variable *quantifier = dynamic_cast<Variable *>(processLogicStatement(xml));
    Rule *nestedStatement = processLogicStatement(xml);

    /* Moves Pointer to </ForAll> */
    xml->skipCurrentElement();

    return new ForAllStatement(quantifier, nestedStatement);
}

Rule *RuleParser::processThereExistsStatement(QXmlStreamReader *xml) {
    /* Quantifier is processed as logicstatement, because it auto moves pointer to next start element and process
     * stops at its delimiter */
    Variable *quantifier = dynamic_cast<Variable *>(processLogicStatement(xml));
    Rule *nestedStatement = processLogicStatement(xml);

    /* Moves Pointer to </ThereExists> */
    xml->skipCurrentElement();

    return new ThereExistsStatement(quantifier, nestedStatement);
}

Rule *RuleParser::processEqualityStatement(QXmlStreamReader *xml) {
    /* Both variable of = is processed as logicstatement, because it auto moves pointer to next start element and
     * process stops at its delimiter */
    Variable *leftVariable = dynamic_cast<Variable *>(processLogicStatement(xml));
    Variable *rightVariable = dynamic_cast<Variable *>(processLogicStatement(xml));

    /* Moves Pointer to </Equals> */
    xml->skipCurrentElement();

    return new EqualityStatement(leftVariable, rightVariable);
}

Rule *RuleParser::processLogicStatement(QXmlStreamReader *xml) {
    /* Move the pointer to the start tag */
    while (!xml->readNextStartElement());

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

LogicSet *RuleParser::processStatements(QXmlStreamReader *xml) {
    LogicSet *ruleSet = new LogicSet();

    for (xml->readNextStartElement(); xml->name() != "EquivalentStatements"; xml->readNextStartElement())
        if (xml->name() == "LogicStatement" && xml->tokenType() == QXmlStreamReader::StartElement)
            ruleSet->add(processLogicStatement(xml));

    return ruleSet;
}