#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#define SYMBOL_NOT ("¬")
#define SYMBOL_AND ("∧")
#define SYMBOL_OR ("∨")
#define SYMBOL_IMPLIES ("→")
#define SYMBOL_IFF ("↔")
#define SYMBOL_FALSITY ("⊥")
#define SYMBOL_TRUTH ("⊤")
#define SYMBOL_FORALL ("∀")
#define SYMBOL_THEREEXISTS ("∃")
#define SYMBOL_EQUALS ("=")

enum Symbol {
    NOT_SYMBOL,
    AND_SYMBOL,
    OR_SYMBOL,
    IMPLIES_SYMBOL,
    IFF_SYMBOL,
    FALSITY_SYMBOL,
    TRUTH_SYMBOL,
    FORALL_SYMBOL,
    THEREEXISTS_SYMBOL,
    EQUALS_SYMBOL,
    VARIABLE_SYMBOL,
    PARAMETERS_SYMBOL,
    PREDICATE_SYMBOL
};

#endif // SYMBOL_HPP
