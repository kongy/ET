#ifndef RULEPARSER_HPP
#define RULEPARSER_HPP

#include "logicset.hpp"
#include <QXmlStreamReader>

class RuleParser
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
public:
    QVector<LogicSet *> *parseRuleXml();
    QVector<LogicSet *> *parseRuleTxt();
};

#endif // RULEPARSER_HPP
