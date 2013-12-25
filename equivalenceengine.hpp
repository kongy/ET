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
	LogicStatement *processLogicStatement(QXmlStreamReader *);
	LogicStatement *processTruthStatement(QXmlStreamReader *);
	LogicStatement *processFalsityStatement(QXmlStreamReader *);
	LogicStatement *processVariableStatement(QXmlStreamReader *);
	LogicStatement *processNotStatement(QXmlStreamReader *);
	LogicStatement *processOrStatement(QXmlStreamReader *);
	LogicStatement *processAndStatement(QXmlStreamReader *);
	LogicStatement *processIFFStatement(QXmlStreamReader *);
	LogicStatement *processImpliesStatement(QXmlStreamReader *);
	bool ruleApplicable(LogicStatement *, LogicSet *);
public:
	EquivalenceEngine();
	~EquivalenceEngine();

	/* Returns a vector of matching rule sets */
	QVector<LogicSet *> *match(LogicStatement *);

	LogicSet *diff(LogicStatement *, LogicSet *);
	QVector<LogicStatement *> *getMatchedRules(LogicStatement *, LogicSet *);
	LogicStatement *replaceStatement(LogicStatement *input, LogicStatement *baseRule, LogicStatement *transformationRule);
};

#endif // EQUIVALENCEENGINE_H
