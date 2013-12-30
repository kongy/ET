#ifndef EQUIVALENCEENGINE_H
#define EQUIVALENCEENGINE_H

#include "logicset.hpp"
#include "equivalencestatement.hpp"
#include <QXmlStreamReader>

class EquivalenceEngine {
private:
	QVector<LogicSet *> *rules;
	void parseRulesTxt();
	void parseRulesXml();
	LogicSet *processStatements(QXmlStreamReader *);
	Rule *processLogicStatement(QXmlStreamReader *);
	Rule *processTruthStatement(QXmlStreamReader *);
	Rule *processFalsityStatement(QXmlStreamReader *);
	Rule *processVariableStatement(QXmlStreamReader *);
	Rule *processNotStatement(QXmlStreamReader *);
	Rule *processOrStatement(QXmlStreamReader *);
	Rule *processAndStatement(QXmlStreamReader *);
	Rule *processIFFStatement(QXmlStreamReader *);
	Rule *processImpliesStatement(QXmlStreamReader *);
	bool ruleApplicable(LogicStatement *, LogicSet *);
public:
	EquivalenceEngine();
	~EquivalenceEngine();

	/* Returns a vector of matching rule sets */
	QVector<LogicSet *> *match(LogicStatement *);

	LogicSet *diff(Rule *, LogicSet *);
	QVector<Rule *> *getMatchedRules(LogicStatement *, LogicSet *);
	LogicStatement *replaceStatement(LogicStatement *input, Rule *baseRule, Rule *transformationRule);
};

#endif // EQUIVALENCEENGINE_H
