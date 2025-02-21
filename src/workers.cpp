//
// Created by marko on 20.4.22..
//

#include "../lib/hw.h"
#include "../h/CCB.hpp"

//#include "../h/print.hpp"
#include "../test/printing.hpp"

static uint64 fibonacci(uint64 n)
{
    if (n == 0 || n == 1) { return n; }
    if (n % 4 == 0){
        //printString("fibonaccci yield");
        CCB::yield();
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyA(void* args)
{
    printString("A: ");
    printString((char*)args);
    printString("\n");

    uint8 i = 0;
    for (; i < 3; i++)
    {
        printString("A: i=");
        //printInteger(i);
        printInt(i);
        printString("\n");
    }

    printString("A: yield\n");
    __asm__ ("li t1, 7");
    CCB::yield();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printString("A: t1=");
    //printInteger(t1);
    printInt(t1);
    printString("\n");

    printString("A: calling fibonacci\n");
    uint64 result = fibonacci(20);
    printString("A: fibonaci=");
    //printInteger(result);
    printInt(result);
    printString("\n");

    for (; i < 6; i++)
    {
        printString("A: i=");
        //printInteger(i);
        printInt(i);
        printString("\n");
    }

    CCB::running->setFinished(true);
    CCB::yield();
}

//void workerBodyB(void* args)
//{
//    uint8 i = 10;
//    for (; i < 13; i++)
//    {
//        printString("B: i=");
//        printInteger(i);
//        //printInt(i);
//        printString("\n");
//    }
//
//    printString("B: yield\n");
//    __asm__ ("li t1, 5");
//    CCB::yield();
//    //((CCB*)(args))->yield();
//
//    printString("B: calling fibonacci\n");
//    uint64 result = fibonacci(23);
//    printString("B: fibonaci=");
//    //printInteger(result);
//    printInteger(result);
//    printString("\n");
//
//    for (; i < 16; i++)
//    {
//        printString("B: i=");
//        printInteger(i);
//        //printInt(i);
//        printString("\n");
//    }
//
//    CCB::running->setFinished(true);
//    CCB::yield();
//}