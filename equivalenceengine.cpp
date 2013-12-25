#include "equivalenceengine.hpp"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

void EquivalenceEngine::parseRulesXml() {
    QFile file(":/equivalences.xml");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(0, "Error", file.errorString());
        return;
    }

    QXmlStreamReader xml(&file);

    /* Avoid checking start Document in while loop */
    if (xml.readNext() != QXmlStreamReader::StartDocument) {
        QMessageBox::critical(0, "Error", "Start Document not present at start");
        return;
    }

    /* Initialise to get ready for parsing */
    rules = new QVector<LogicSet *>();

    do {
        if (xml.readNextStartElement() && xml.name() == "EquivalentStatements")
            rules->append(processStatements(&xml));
    } while(!xml.atEnd() && !xml.hasError());

    xml.clear();
    file.close();
}

LogicStatement *EquivalenceEngine::processTruthStatement(QXmlStreamReader *xml) {
    xml->skipCurrentElement();
    return new Truth();
}

LogicStatement *EquivalenceEngine::processFalsityStatement(QXmlStreamReader *xml) {
    xml->skipCurrentElement();
    return new Falsity();
}

LogicStatement *EquivalenceEngine::processVariableStatement(QXmlStreamReader *xml) {
    return new Variable(new QString(xml->readElementText()));
}

LogicStatement *EquivalenceEngine::processNotStatement(QXmlStreamReader *xml) {
    /* Evaluate the inner expression inside NOT */
    LogicStatement *nestedStatement = processLogicStatement(xml);

    /* The pointer should now point to just before </NOT>,
     * we will therefore move it over to </NOT> */
    xml->skipCurrentElement();

    return new NotStatement(nestedStatement);
}

LogicStatement *EquivalenceEngine::processOrStatement(QXmlStreamReader *xml) {

    /* Process one tree at a time, stopping at delimiter */
    LogicStatement *leftStatement = processLogicStatement(xml);
    LogicStatement *rightStatement = processLogicStatement(xml);

    /* Pointer moved to </Or> */
    xml->skipCurrentElement();

    return new OrStatement(leftStatement, rightStatement);
}

LogicStatement *EquivalenceEngine::processAndStatement(QXmlStreamReader *xml) {
    /* Process one tree at a time, stopping at delimiter */
    LogicStatement *leftStatement = processLogicStatement(xml);
    LogicStatement *rightStatement = processLogicStatement(xml);

    /* Pointer moved to </And> */
    xml->skipCurrentElement();

    return new AndStatement(leftStatement, rightStatement);
}

LogicStatement *EquivalenceEngine::processIFFStatement(QXmlStreamReader *xml) {
    /* Process one tree at a time, stopping at delimiter */
    LogicStatement *leftStatement = processLogicStatement(xml);
    LogicStatement *rightStatement = processLogicStatement(xml);

    /* Pointer moved to </IFF> */
    xml->skipCurrentElement();

    return new IffStatement(leftStatement, rightStatement);
}

LogicStatement *EquivalenceEngine::processImpliesStatement(QXmlStreamReader *xml) {
    /* Process one tree at a time, stopping at delimiter */
    LogicStatement *leftStatement = processLogicStatement(xml);
    LogicStatement *rightStatement = processLogicStatement(xml);

    /* Pointer moved to </Implies> */
    xml->skipCurrentElement();

    return new ImpliesStatement(leftStatement, rightStatement);
}

LogicStatement *EquivalenceEngine::processLogicStatement(QXmlStreamReader *xml) {
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

LogicSet *EquivalenceEngine::processStatements(QXmlStreamReader *xml) {
    LogicSet *ruleSet = new LogicSet();

    for (xml->readNextStartElement(); xml->name() != "EquivalentStatements"; xml->readNextStartElement())
        if (xml->name() == "LogicStatement" && xml->tokenType() == QXmlStreamReader::StartElement)
            ruleSet->add(processLogicStatement(xml));

    return ruleSet;
}

void EquivalenceEngine::parseRulesTxt() {

	QFile file(":/equivalences.txt");

	if(!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(0, "Error", file.errorString());
		//QMessageBox::information(0, "error", file.errorString());
		return;
	}

	QTextStream in(&file);

	QString line;
	QStringList fields;
	LogicSet *rule_set;
	rules = new QVector<LogicSet *>();

	while(!in.atEnd()) {

		line = in.readLine();
		fields = line.split('\t');

		rule_set = new LogicSet();
		rules->push_back(rule_set);

		for (QString rule : fields)
			rule_set->add(parse(rule));
	}

	file.close();
}

EquivalenceEngine::EquivalenceEngine() {
	parseRulesTxt();
}

EquivalenceEngine::~EquivalenceEngine() {
	for (LogicSet *logic_set : *rules)
		delete logic_set;
	delete rules;
}

QVector<LogicSet *> *EquivalenceEngine::match(LogicStatement *input) {

	QVector<LogicSet *> *relatedEquivalence = new QVector<LogicSet *>();

	for (LogicSet *ruleSet : *rules)
		if (ruleApplicable(input, ruleSet))
			relatedEquivalence->append(ruleSet);

	return relatedEquivalence;
}

LogicSet *EquivalenceEngine::diff(LogicStatement *base, LogicSet *set) {
	return set->diff(base);
}

QVector<LogicStatement *> *EquivalenceEngine::getMatchedRules(LogicStatement *input, LogicSet *ruleSet) {
	QVector<LogicStatement *> *matchedRules = new QVector<LogicStatement *>();

	IDTable idtable;

	for (LogicStatement *rule : *ruleSet->getSet())
		if (rule->match(input, &idtable)) {
			matchedRules->append(rule);
			idtable.clear();
		}

	return matchedRules;
}

LogicStatement *replaceStatement(LogicStatement *input, LogicStatement *baseRule, LogicStatement *transformationRule) {
	IDTable idtable;

	if (baseRule->match(input, &idtable))
		return transformationRule->clone()->replace(&idtable);

	return nullptr;
}

bool EquivalenceEngine::ruleApplicable(LogicStatement *input, LogicSet *ruleSet) {
	IDTable idtable;

	for (LogicStatement *rule : *ruleSet->getSet()) {
		if (rule->match(input, &idtable))
			return true;
	}

	return false;
}
