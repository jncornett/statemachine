# StateMachine

A state machine for C++11

# Basic Usage

## define a grammar

    enum class WebSocketState : uint8_t;

    SMGrammar<Packet, WebSocketState> grammar;
    grammar[START] = CLI_REQUEST_WS >> WAIT_FOR_SRV_HANDSHAKE;
    grammar[WAIT_FOR_SRV_HANDSHAKE] = SRV_ACCEPT_WS >> READY;
    grammar[READY] = HANDLE_FRAME >> READY;
    grammar[READY] = HANDLE_FRAG_BEGIN >> WAIT_FOR_FRAG;
    grammar[WAIT_FOR_FRAG] = HANDLE_FRAG_MIDDLE >> WAIT_FOR_FRAG;
    grammar[WAIT_FOR_FRAG] = HANDLE_FRAG_END >> READY;
    grammar[READY] = HANDLE_PING >> WAIT_FOR_PONG;
    grammar[WAIT_FOR_PONG] = HANDLE_PONG >> READY;

## setup an event loop

    SMMachine<Packet, WebSocketState> machine(grammar, START);

    while ( !done )
    {
        Packet pkt = get_next_wire_packet();
        if ( !machine.input(pkt) )
        {
            // unexpected State
            cout << "Uh-oh!";
            machine.reset(START);
        }
    }
