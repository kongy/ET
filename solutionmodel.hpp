#ifndef SOLUTIONMODEL_HPP
#define SOLUTIONMODEL_HPP

#include <QStack>

#include "AST.hpp"

class solutionModel
{
public:
	solutionModel();
	solutionModel(AST::LogicStatement* begin, AST::LogicStatement* end);

	QStack<AST::LogicStatement*> forwardStack;
	QStack<AST::LogicStatement*> backwardStack;
};

#endif // SOLUTIONMODEL_HPP
