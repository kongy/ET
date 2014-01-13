#include "solutionmodel.hpp"

#include <QTextStream>

using AST::LogicStatement;

solutionModel::solutionModel(LogicStatement *begin, LogicStatement *end)
{
	forwardStack << begin;
	backwardStack << end;
}

solutionModel::solutionModel(QFile *f)
{
	QTextStream in(f);

	for (QString line; !(line = in.readLine()).isEmpty();) {
		forwardStack << AST::parse(line);
	}
	while (!in.atEnd()) {
		QString line = in.readLine();
		backwardStack << AST::parse(line);
	}
}

solutionModel::~solutionModel()
{
	for (LogicStatement *s : forwardStack) {
		delete s;
	}
	for (LogicStatement *s : backwardStack) {
		delete s;
	}
	clearRedoStack();
}

void solutionModel::add(bool forward, LogicStatement *formula)
{
	clearRedoStack();
	if (forward) {
		forwardStack.push(formula);
	} else {
		backwardStack.push(formula);
	}
	undoStack.push(
	    new Operation{ true, forward, QVector<LogicStatement *>({ formula }) });
}

void solutionModel::remove(bool forward, int n)
{
	QStack<LogicStatement *> *stack = forward ? &forwardStack : &backwardStack;
	QVector<LogicStatement *> removedlist;
	for (int i = stack->size() - 1; i >= n; i--) {
		removedlist.append(stack->pop());
	}
	undoStack.push(new Operation{ false, forward, removedlist });
}

void solutionModel::undo()
{
	if (undoStack.isEmpty())
		return;

	Operation *o = undoStack.pop();
	QStack<LogicStatement *> *tostack =
	    o->forward ? &forwardStack : &backwardStack;
	if (o->add) {
		tostack->pop();
	} else {
		for (LogicStatement *i : o->stats) {
			tostack->push(i);
		}
	}
	redoStack.push(o);
}

void solutionModel::redo()
{
	if (redoStack.isEmpty())
		return;

	Operation *o = redoStack.pop();
	QStack<LogicStatement *> *tostack =
	    o->forward ? &forwardStack : &backwardStack;
	if (o->add) {
		tostack->push(o->stats.at(0));
	} else {
		for (int i = 0; i < o->stats.size(); i++) {
			tostack->pop();
		}
	}
	undoStack.push(o);
}

bool solutionModel::proofFinished()
{
	return forwardStack.top()->equals(backwardStack.top());
}

void solutionModel::saveToFile(QFile *f)
{
	QTextStream out(f);
	for (LogicStatement *l : forwardStack) {
		out << l->print(true) << "\n";
	}
	out << "\n";
	for (LogicStatement *l : backwardStack) {
		out << l->print(true) << "\n";
	}
}

void solutionModel::clearRedoStack()
{
	// TODO: Fix memory corruption

	for (Operation *o : redoStack) {
		for (LogicStatement *s : o->stats) {
			delete s;
		}
		delete o;
	}
	redoStack.clear();
}
