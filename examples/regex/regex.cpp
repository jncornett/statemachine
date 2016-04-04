#include <iostream>
#include <functional>
#include <string>

#include <state_machine.hpp>

// imitate the regular expression [ab][ab123]+
// expands to (a | b) (a | b | 1 | 2 | 3) (a | b | 1 | 2 | 3) *

enum State
{
    START,
    ALPHANUM,
    END
};

struct SetMatcher
{
    const char* set;

    SetMatcher(const char* s) : set(s) { }

    bool operator()(const char& tok, const State&)
    { return std::string(set).find(tok) != std::string::npos; }
};

using RegexMachine = Machine<char, State>;

// matches [ab]
const auto MATCHFN_ALPHA = SetMatcher("ab");
const auto MATCHFN_NUM = SetMatcher("123");

const RegexMachine::ActionFn ALPHA_ACTION = []() { std::cout << "Matched 'ab'\n"; };
const RegexMachine::ActionFn NUM_ACTION = []() { std::cout << "Matched '123'\n"; };

const RegexMachine::Rule RULE_ALPHA { {MATCHFN_ALPHA}, ALPHA_ACTION };
const RegexMachine::Rule RULE_NUM { {MATCHFN_NUM}, NUM_ACTION };

const std::string good = "ab12";
const std::string bad1 = "gb12";
const std::string bad2 = "ab99";

void test(const RegexMachine::Grammar& g, const std::string& string)
{
    RegexMachine machine(g, START);
    std::cout << "testing '" << string << "'\n";
    for ( auto c : string )
        if ( !machine.input(c) )
            break;
}

int main()
{
    RegexMachine::Grammar grammar;
    grammar[START] = RULE_ALPHA >> ALPHANUM;
    grammar[ALPHANUM] = RULE_ALPHA >> ALPHANUM;
    grammar[ALPHANUM] = RULE_NUM >> ALPHANUM;

    test(grammar, good);
    test(grammar, bad1);
    test(grammar, bad2);
}
