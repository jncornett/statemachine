#pragma once

#include <map>
#include <vector>

enum SMModifier
{
    SM_MOD_NONE = 0,
    SM_MOD_MAYBE,
    SM_MOD_ANY
};

template<typename T, typename K>
struct SMTraits
{
    using Token = T;
    using Key = K;
    using MatchFn = std::function<bool(const Token&, const Key&)>;
    using ActionFn = std::function<void()>;

};

template<typename Traits>
struct SMConstants
{
    static const typename Traits::MatchFn NULL_MATCHER;
    static const typename Traits::ActionFn NULL_ACTION;
};

template<typename Traits>
const typename Traits::MatchFn SMConstants<Traits>::NULL_MATCHER =
    [](const typename Traits::Token&, const typename Traits::Key&)
    { return false; };

template<typename Traits>
const typename Traits::ActionFn SMConstants<Traits>::NULL_ACTION =
    []() { };

template<typename Traits>
struct SMMatcher
{
    using Constants = SMConstants<Traits>;
    using MatchFn = typename Traits::MatchFn;

    MatchFn match;
    SMModifier modifier;

    SMMatcher(
        MatchFn m = Constants::NULL_MATCHER, SMModifier mod = SM_MOD_NONE) :
        match(m), modifier(mod)
    { }
};

template<typename Traits>
struct SMRule
{
    using Constants = SMConstants<Traits>;
    using Key = typename Traits::Key;
    using ActionFn = typename Traits::ActionFn;
    using Matcher = SMMatcher<Traits>;

    Matcher matcher;
    ActionFn action;
    Key next;

    SMRule(
        Matcher m = Matcher(),
        ActionFn a = Constants::NULL_ACTION) :
        matcher(m), action(a)
    { }

    SMRule(Matcher m, ActionFn a, Key n) :
        matcher(m), action(a), next(n)
    { }

    SMRule(Matcher m, Key n) :
        matcher(m), action(Constants::NULL_ACTION), next(n)
    { }
};

template<typename Traits>
inline constexpr SMRule<Traits> operator>>(SMMatcher<Traits> matcher, typename Traits::Key next)
{ return { matcher, next }; }

template<typename Traits>
inline constexpr SMRule<Traits> operator>>(SMRule<Traits> rule, typename Traits::Key next)
{ return { rule.matcher, rule.action, next }; }

template<typename T, typename K>
class SMGrammar
{
public:
    using Traits = SMTraits<T, K>;
    using Key = typename Traits::Key;
    using Rule = SMRule<Traits>;
    using Row = std::vector<Rule>;
    using Map = std::map<Key, Row>;

    class RowProxy
    {
    public:
        RowProxy(Row& row) : row(row) { }
        operator Row&() { return row; }
        RowProxy& operator=(Rule rule)
        {
            row.push_back(rule);
            return *this;
        }

    private:
        Row& row;
    };

    SMGrammar(Map map) : map(map) { }
    SMGrammar() { }

    RowProxy operator[](const Key& key)
    { return { map[key] }; }

private:
    Map map;
};

template<typename T, typename K>
class Machine
{
public:
    using Traits = SMTraits<T, K>;
    using MatchFn = typename Traits::MatchFn;
    using ActionFn = typename Traits::ActionFn;
    using Grammar = SMGrammar<T, K>;
    using Row = typename Grammar::Row;
    using Key = typename Traits::Key;
    using Token = typename Traits::Token;
    using States = std::vector<Key>;
    using Matcher = SMMatcher<Traits>;
    using Rule = SMRule<Traits>;

    Machine(Grammar g, const Key& start) :
        grammar(g), start(start), states { start } { }

    Machine(Grammar g) :
        grammar(g) { }

    void reset(const Key& key)
    { states = { key }; }

    void reset()
    { reset(start); }

    bool input(const Token& tok)
    {
        if ( states.empty() )
            return false;

        States tmp;

        for ( auto state : states )
        {
            Row& row = grammar[state];
            for ( auto rule : row )
            {
                if ( process(rule.matcher, tok, state, rule.next, tmp) )
                    rule.action();
            }
        }

        states = tmp;
        return true;
    }

private:
    bool process(
        Matcher& matcher, const Token& tok, const Key& key, const Key& next, States& out)
    {
        bool matched = matcher.match(tok, key);

        if ( matched )
        {
            if ( matcher.modifier == SM_MOD_ANY )
                out.push_back(key);

            out.push_back(next);
        }

        else if ( matcher.modifier == SM_MOD_MAYBE )
            out.push_back(next);

        return matched;
    }

    Grammar grammar;
    Key start;
    States states;
};

void foo()
{
    using Traits = SMTraits<int, int>;
    using Rule = SMRule<Traits>;
    using Matcher = SMMatcher<Traits>;
    SMTraits<int, int> smt;
    SMConstants<Traits>::NULL_MATCHER(1, 1);
    auto x = Rule() >> 1;

    Matcher m;
    auto y = m >> 1;

    SMGrammar<int, int> grammar;
    const Matcher A;
    grammar[3] = A >> 1;

    Machine<int, int> machine(grammar);
    machine.input(12);
}
