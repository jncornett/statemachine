#include <iostream>
#include <pookie.hpp>

using namespace pookie;

namespace
{

enum class Type
{
    NONE = 0,
    OPEN,
    PING,
    PONG,
    CLOSE
};

struct Event
{
    Type type;
    const char* message;
};

const Event events[] =
{
    { Type::OPEN, "foo" },
    { Type::PING, "bar" },
    { Type::PONG, "buzz" },
    { Type::CLOSE, "bat" },
    { Type::NONE, nullptr }
};

} // namespace

namespace
{

std::ostream& operator<<(std::ostream& os, const Type& t)
{
    switch ( t )
    {
        case Type::OPEN: os << "OPEN"; break;
        case Type::PING: os << "PING"; break;
        case Type::PONG: os << "PONG"; break;
        case Type::CLOSE: os << "CLOSE"; break;
        default: os << "NONE"; break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const Event& e)
{
    os << "Event { " << e.type << ", '" << e.message << "' }";
    return os;
}

} // namespace

namespace
{

enum class SMState
{ START, OPEN, CLOSE, PING, PONG, INVALID, END };

struct Context { };

#define MATCHER() \
    [](Context* ctx, Event& e, const SMState& k)

#define ACTION() \
    [](Context* ctx, Event& e, const SMState& k)

const auto NULL_MATCHER = MATCHER() { return true; };
const auto NULL_ACTION = ACTION() { };

}

void process(Machine<Event, SMState>& machine, const Event& e)
{
    std::cout << "Processing " << e << std::endl;
    std::cout << "Result: " << machine.input(e) << std::endl;
}

void main_loop(Machine<Event, SMState>& machine)
{
    static int index = 0;

    machine.reset(SMState::START);

    while ( events[index].message )
        process(machine, events[index++]);
}

int main()
{
    Machine<Event, SMState> machine;
    machine[SMState::START] = {
        [](const Event& e, const SMState&) { return e.type == Type::OPEN; },
        [](const Event& e, const SMState&) { std::cout << "doing action for start -> open\n"; },
        { SMState::OPEN }
    };

    machine[SMState::OPEN] = {
        [](const Event& e, const SMState&) { return e.type == Type::CLOSE; },
        [](const Event& e, const SMState&) { std::cout << "doing action for open -> close\n"; },
        { SMState::CLOSE }
    };

    machine[SMState::OPEN] = {
        [](const Event& e, const SMState&) { return e.type == Type::PING; },
        [](const Event& e, const SMState&) { std::cout << "doing action for open -> ping\n"; },
        { SMState::PING }
    };

    machine[SMState::PING] = {
        [](const Event& e, const SMState&) { return e.type == Type::PONG; },
        [](const Event& e, const SMState&) { std::cout << "doing action for ping -> pong\n"; },
        { SMState::OPEN }
    };


    main_loop(machine);
    return 0;
}
