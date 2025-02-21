//
// Created by os on 10/18/22.
//

#ifndef PROJECT_BASE_CCB_HPP
#define PROJECT_BASE_CCB_HPP


#include "../lib/hw.h"
#include "scheduler.hpp"

class CCB
{
public:
    ~CCB() {
        delete[] stack;
    }

    bool isFinished() const {
        return finished;
    }

    void setFinished(bool finished) {
        CCB::finished = finished;
    }

    using Body = void (*)(void*);

    static CCB *createCorutine(Body body, void* args);
    static CCB *createCorutineNoStart(Body body, void* args);
    static void start(CCB* ccb);

    static void yield();

    static CCB* running;
    static CCB* systemThread;

    static void exit();

    static void dispatch();

private:
    CCB(Body body, void* args) :
        body(body),
        args(args),
        stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
        context({
                        (uint64) &threadWrapper,
                        stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                }),
        finished(false)
    {

    }

    struct Context{
        uint64 ra; //where should corutine get back
        uint64 sp; //where corutine put its data
    };
    Body body;
    void* args;
    uint64 *stack;
    Context context;
    bool finished;


    static void contextSwitch(Context *oldContext, Context *runningContext);

    static uint64 constexpr STACK_SIZE = DEFAULT_STACK_SIZE;

    static void threadWrapper();

};


#endif //PROJECT_BASE_CCB_HPP
