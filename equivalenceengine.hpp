#ifndef EQUIVALENCEENGINE_H
#define EQUIVALENCEENGINE_H

#include <logicset.hpp>
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
public:
	EquivalenceEngine();
	~EquivalenceEngine();

	/* Returns a vector of matching rules under construction of
	 * EquivalenceStatement */
	QVector<EquivalenceStatement *> *match(LogicStatement *);
};

#endif // EQUIVALENCEENGINE_H
