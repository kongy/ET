#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#define SYMBOL_NOT (QChar(u'¬'))
#define SYMBOL_AND (QChar(u'∧'))
#define SYMBOL_OR (QChar(u'∨'))
#define SYMBOL_IMPLIES (QChar(u'→'))
#define SYMBOL_IFF (QChar(u'↔'))
#define SYMBOL_FALSITY (QChar(u'⊥'))
#define SYMBOL_TRUTH (QChar(u'⊤'))
#define SYMBOL_FORALL (QChar(u'∀'))
#define SYMBOL_THEREEXISTS (QChar(u'∃'))
#define SYMBOL_EQUALS (QChar(u'='))

enum Symbol {
	VARIABLE_SYMBOL,
	TRUTH_SYMBOL,
	FORALL_SYMBOL,
	NOT_SYMBOL,
	AND_SYMBOL,
	OR_SYMBOL,
	IMPLIES_SYMBOL,
	IFF_SYMBOL,
	FALSITY_SYMBOL,
	THEREEXISTS_SYMBOL,
	EQUALS_SYMBOL,
	PARAMETERS_SYMBOL,
	PREDICATE_SYMBOL
};

#endif // SYMBOL_HPP
