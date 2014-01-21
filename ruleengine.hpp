#ifndef RULEENGINE_HPP
#define RULEENGINE_HPP

#include "logicset.hpp"
#include <QXmlStreamReader>

class RuleEngine
{
  private:
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
	Rule *processForAllStatement(QXmlStreamReader *);
	Rule *processThereExistsStatement(QXmlStreamReader *);
	Rule *processEqualityStatement(QXmlStreamReader *);
	void parseRule(QString fromFilePath,
	               QVector<LogicSet *> *destinationRuleSet);
	QVector<LogicSet *> *userDefinedRules;
	QVector<LogicSet *> *allRules;
	void flushNewRuleToXml();
	LogicSet *generateRuleSet(LogicSet *rawEquivalenceFormulas);
	IDTable *generateRuleVariables(LogicSet *rawVariables);
	Rule *generateNewRule(LogicStatement *rawFormula, IDTable *replaceTable);
	Variable *asciiToVariable(const short asciiCode);

  public:
	QVector<LogicSet *> *parseRuleXml();
	bool addRule(LogicStatement *formulaFrom, LogicStatement *formulaTo);
	RuleEngine();
	~RuleEngine();
};

#endif // RULEENGINE_HPP
