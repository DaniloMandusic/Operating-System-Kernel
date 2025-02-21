//
// Created by os on 10/18/22.
//

#include "../h/CCB.hpp"
#include "../h/scheduler.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

extern "C" void pushRegisters();
extern "C" void popRegisters();

CCB *CCB::running = nullptr;
CCB *CCB::systemThread = nullptr;



CCB *CCB::createCorutine(Body body, void* args) {

    CCB* coroutine = new CCB(body,args);
    Scheduler::put(coroutine);

    return coroutine;
}

CCB *CCB::createCorutineNoStart(Body body, void* args) {

    CCB* coroutine = new CCB(body,args);

    return coroutine;
}

void CCB::start(CCB* ccb){
    Scheduler::put(ccb);
}

void CCB::yield() {
    pushRegisters();

    CCB::dispatch();

    popRegisters();
}

void CCB::dispatch(){
    CCB *old = running;
    if(!old->isFinished()){
        Scheduler::put(old);
    }
    running = Scheduler::get();

    CCB::contextSwitch(&old->context, &running->context);
}

void CCB::threadWrapper(){
    if (running == systemThread) {
        Riscv::ms_sstatus(Riscv::SSTATUS_SPP);
    } else {
        Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    }
    Riscv::popSppSpie();
    running->body(running->args);
    running->setFinished(true);

    thread_dispatch();
}

void CCB::exit() {
    CCB::running->setFinished(true);
    CCB::dispatch();
}
