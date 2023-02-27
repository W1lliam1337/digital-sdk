#ifndef VALVE_MOVE_HELPER
#define VALVE_MOVE_HELPER

class i_move_helper {
    virtual void unknown_virtual( ) = 0;
public:
    virtual void set_host( void* host ) = 0;
private:
    virtual void pad00( ) = 0;
    virtual void pad01( ) = 0;
public:
    virtual void process_impacts( ) = 0;
};

#endif