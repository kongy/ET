#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QString>
#include <QVector>

#define XML_TRUTH_TAG QString("Truth")
#define XML_FALSITY_TAG QString("Falsity")
#define XML_IDENTIFER_TAG QString("ID")
#define XML_AND_TAG QString("And")
#define XML_OR_TAG QString("Or")
#define XML_IMPLIES_TAG QString("Implies")
#define XML_IFF_TAG QString("IFF")
#define XML_NOT_TAG QString("Not")
#define XML_FORALL_TAG QString("ForAll")
#define XML_THEREEXISTS_TAG QString("ThereExists")
#define XML_NOT_OCCUR_FREE QString("NOT_OCCUR_FREE")
#define XML_OCCUR_FREE QString("OCCUR_FREE")
#define XML_NOT_OCCUR QString("NOT_OCCUR")
#define XML_MAY_OCCUR QString("MAY_OCCUR")
#define XML_EQUALS_TAG QString("Equals")

static const QString EXPECT_VARIABLE_ERROR_MESSAGE = "Error: Expecting input of type Variable";
static const QString VARIABLE_MUST_NOT_OCCUR_ERROR_MESSAGE = "Error: Variable must not occur in the statement to be replaced";
static const QString RENAME_PREFIX_MESSAGE = "Please enter a variable to rename ";
static const QString VARIABLE_REQUEST_PREFIX_MESSAGE = "Please enter a variable for ";
static const QString FORMULA_REQUEST_PREFIX_MESSAGE = "Please enter a formula for ";
static const QString BOUND_VARIABLE_REQUEST_PREFIX_MESSAGE = "Please enter a bounded variable for ";
static const QString VARIABLE_OCCURS_FREE_ERROR_MESSAGE = "Error: User defined variable occured free in the statement to be replaced";
static const QString NONE = "";

enum Message {
    EXPECT_VARIABLE_ERROR,
    VARIABLE_MUST_NOT_OCCUR_ERROR,
    RENAME_PREFIX,
    VARIABLE_REQUEST_PREFIX,
    FORMULA_REQUEST_PREFIX,
    BOUND_VARIABLE_REQUEST_PREFIX,
    VARIABLE_OCCURS_FREE_ERROR,
    NO_MESSAGE
};


static const QVector<QString> messages = {EXPECT_VARIABLE_ERROR_MESSAGE, VARIABLE_MUST_NOT_OCCUR_ERROR_MESSAGE, RENAME_PREFIX_MESSAGE,
                                  VARIABLE_REQUEST_PREFIX_MESSAGE, FORMULA_REQUEST_PREFIX_MESSAGE, BOUND_VARIABLE_REQUEST_PREFIX_MESSAGE,
                                  VARIABLE_OCCURS_FREE_ERROR_MESSAGE, NONE};

static const QString getMessage(Message messageID) {
    return messages.at(messageID);
}

#endif // UTILITY_HPP
