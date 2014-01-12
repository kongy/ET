#ifndef SOLUTIONMODEL_HPP
#define SOLUTIONMODEL_HPP

#include <QFile>
#include <QStack>

#include "AST.hpp"

struct Operation {
	bool add : 1;
	bool forward : 1;
	QVector<AST::LogicStatement *> stats;
};

class solutionModel
{
	QStack<Operation*> undoStack;
	QStack<Operation*> redoStack;

public:
	solutionModel(AST::LogicStatement* begin, AST::LogicStatement* end);
	solutionModel(QFile *f);
	~solutionModel();

	QStack<AST::LogicStatement*> forwardStack;
	QStack<AST::LogicStatement*> backwardStack;

	void undo();
	void redo();
	void add(bool forward, AST::LogicStatement *formula);
	void remove(bool forward, int n);

	bool proofFinished();

	void saveToFile(QFile *f);

private:
	void clearRedoStack();
};

#endif // SOLUTIONMODEL_HPP
