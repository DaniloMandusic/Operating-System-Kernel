//
// Created by os on 4/8/24.
//

#ifndef PROJECT_BASE_MEMALLOCATOR_HPP
#define PROJECT_BASE_MEMALLOCATOR_HPP

#include "../lib/hw.h"

class MemAllocator {
public:
    static void* mem_alloc(size_t size);

    static int mem_free(void*);

private:


};


#endif //PROJECT_BASE_MEMALLOCATOR_HPP
