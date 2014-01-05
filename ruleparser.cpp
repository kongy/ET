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
    return new Variable(new QString(xml->readElementText()));
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
