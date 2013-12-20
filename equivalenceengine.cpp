#include <equivalenceengine.hpp>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
using namespace std;

QString normalise(QString str) {

	QString buffer;

	for (QString part:str.split(" ")) {
		if (part == "and")
			buffer += SYMBOL_AND;
		else if (part == "or")
			buffer += SYMBOL_OR;
		else if (part == "iff")
			buffer += SYMBOL_IFF;
		else if (part == "not")
			buffer += SYMBOL_NOT;
		else if (part == "imply")
			buffer += SYMBOL_IMPLIES;
		else if (part == "F")
			buffer += SYMBOL_FALSITY;
		else if (part == "T")
			buffer += SYMBOL_TRUTH;
		else
			buffer += part;
	}

	return buffer;
}

void EquivalenceEngine::parseRulesTxt() {

	QFile file("equivalences.txt");

	if(!file.open(QIODevice::ReadOnly)) {
		QMessageBox::information(0, "error", file.errorString());
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

QVector<EquivalenceStatement *> *EquivalenceEngine::match(LogicStatement *input) {

	QVector<EquivalenceStatement *> *related_equivalence = new QVector<EquivalenceStatement *>();
	IDTable *id_table = new IDTable();

	for (LogicSet *rule_set : *rules) {
		for (LogicStatement *rule : *rule_set->getSet()) {
			if (rule->match(input, id_table)) {
				related_equivalence->push_back(new EquivalenceStatement(rule, id_table, rule_set->diff(rule)));
				id_table = new IDTable();
			}
			else
				id_table->clear();
		}
	}

	return related_equivalence;
}
