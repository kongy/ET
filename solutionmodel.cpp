#include "solutionmodel.hpp"

solutionModel::solutionModel()
{

}

solutionModel::solutionModel(AST::LogicStatement *begin, AST::LogicStatement *end) {
	forwardStack<<begin;
	backwardStack<<end;
}
