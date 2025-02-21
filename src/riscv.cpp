//
// Created by os on 10/11/24.
//

#include "../h/riscv.hpp"
#include "../h/MemAllocator.hpp"
#include "../h/CCB.hpp"
#include "../lib/console.h"
//#include "../h/print.hpp"
//#include "../test1/printing.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap(){
    uint64  scause = r_scause();

    if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        uint64 code;
        __asm__ volatile ("mv %0, a0": "=r" (code));

        switch (code) {
            case 0x01: {
                size_t size;
                __asm__ volatile("mv %0, a1" : "=r" (size));

                void *ret = MemAllocator::mem_alloc(size);

                __asm__ volatile("mv a0, %0" : : "r"(ret));
                __asm__ volatile("sw a0, 10*8(fp)");

                break;
            }

            case 0x02: {
                void *memory;
                __asm__ volatile("mv %0, a1" : "=r" (memory));

                int ret = MemAllocator::mem_free(memory);

                __asm__ volatile("mv a0, %0" : : "r"(ret));
                __asm__ volatile("sw a0, 10*8(fp)");

                break;
            }

            case 0x11: {
                CCB **handle;
                CCB::Body body;
                void *args;

                __asm__ volatile ("mv %0, a1": "=r" (handle));
                __asm__ volatile ("mv %0, a2": "=r" (body));
                __asm__ volatile ("mv %0, a6": "=r" (args));

                *handle = CCB::createCorutine(body, args);

                if (!*handle) {
                    __asm__ volatile("mv a0, %0" : : "r"(-1));
                } else {
                    __asm__ volatile("mv a0, %0" : : "r"(0));
                }
                __asm__ volatile("sw a0, 10*8(fp)");

                break;
            }

            //thread exit
            case 0x12: {
                CCB::exit();

                __asm__ volatile("mv a0, %0" : : "r"(0));
                __asm__ volatile("sw a0, 10*8(fp)");

                break;
            }

            //thread dispatch
            case 0x13: {
                CCB::dispatch();

                break;
            }

            // create thread no start
            case 0x14: {
                CCB **handle;
                CCB::Body body;
                void *args;

                __asm__ volatile ("mv %0, a1": "=r" (handle));
                __asm__ volatile ("mv %0, a2": "=r" (body));
                __asm__ volatile ("mv %0, a6": "=r" (args));

                *handle = CCB::createCorutineNoStart(body, args);

                if (!*handle) {
                    __asm__ volatile("mv a0, %0" : : "r"(-1));
                } else {
                    __asm__ volatile("mv a0, %0" : : "r"(0));
                }
                __asm__ volatile("sw a0, 10*8(fp)");

                break;
            }

            case 0x15: {
                CCB *ccb;
                __asm__ volatile ("mv %0, a1": "=r" (ccb));

                CCB::start(ccb);

                break;
            }

            //getc
            case 0x41: {
                char c;
                c = __getc();
                __asm__ volatile ("mv a0, %0" : : "r"(c));
                __asm__ volatile ("sw a0, 10*8(fp)");
                break;
            }

            //putc
            case 0x42: {
                char c;
                __asm__ volatile ("mv %0, a1" : "=r" (c));
                __putc(c);
                break;
            }

            default:
                break;
        }

        w_sstatus(sstatus);
        w_sepc(sepc);

    } else if(scause == 0x8000000000000009UL) {
        //printString("console handler");
        console_handler();
    } else if(scause == 0x8000000000000001UL){
        //printString("supervisor software interrupt 8001");
        mc_sip(SIP_SSIE);

    } else {
        //printString("error, scause:");
        //printInteger(scause);
        //printString("\n");

    }

}
