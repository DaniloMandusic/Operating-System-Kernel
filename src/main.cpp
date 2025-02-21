//
// Created by os on 10/10/24.
//
#include "../lib/console.h"
#include "../h/MemAllocator.hpp"
#include "../lib/hw.h"
#include "../h/CCB.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

#include "../test/printing.hpp"

extern void userMain();

void main(){

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    CCB* mainThread;
    mainThread = CCB::createCorutine(nullptr, nullptr);

    CCB::systemThread = mainThread;
    CCB::running = mainThread;

    CCB* userMainThread;

    userMainThread = CCB::createCorutine(reinterpret_cast<void (*) (void*)> (userMain), nullptr);

    while(!userMainThread->isFinished()){
        thread_dispatch();
    }

}
