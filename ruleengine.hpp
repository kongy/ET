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
    void parseRule(QString fromFilePath, QVector<LogicSet *> *destinationRuleSet);
    QVector<LogicSet *> *userDefinedRules;
    QVector<LogicSet *> *allRules;
    void flushNewRuleToXml();
public:
    QVector<LogicSet *> *parseRuleXml();
    QVector<LogicSet *> *parseRuleTxt();
    bool addRule(LogicSet *ruleSet);
    RuleEngine();
    ~RuleEngine();
};

#endif // RULEENGINE_HPP
