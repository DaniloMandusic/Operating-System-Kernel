//
// Created by os on 4/8/24.
//

#include "../h/MemAllocator.hpp"

struct FreeMem {
    struct FreeMem* next;
    size_t size;
    //uint64 startingAddress;
};

static struct FreeMem* freeMemHead;

void* MemAllocator::mem_alloc(size_t size){
//    void* memory;
//    memory = (void*) HEAP_START_ADDR;

    //return memory;
    if(size <= 0){
        return 0;
    }

    if (size%MEM_BLOCK_SIZE != 0){
        size = (size/MEM_BLOCK_SIZE + 1) * MEM_BLOCK_SIZE;
    }

    //Try to find existing free mem block in list
    struct FreeMem *block = freeMemHead, *prev = 0;
    for(; block != 0; prev = block, block = block->next)
        if(block->size >= size)
            break;

    //size of freeMem block rounded on memblock size
    size_t freeMemBlockSize = sizeof(struct FreeMem);
    if (freeMemBlockSize%MEM_BLOCK_SIZE != 0){
        freeMemBlockSize = (freeMemBlockSize/MEM_BLOCK_SIZE + 1) * MEM_BLOCK_SIZE;
    }

    if(block == 0){
        //creating new block
        block = (struct FreeMem*) HEAP_START_ADDR;
        block->size = (size_t)((uint64)HEAP_END_ADDR-(uint64)HEAP_START_ADDR) - freeMemBlockSize; // prepravljeni cast uint64
        block->next = 0;

        freeMemHead = block;
    }

    //Allocate the requested block
    size_t remainingSize = block->size - size;

    struct FreeMem* newBlock;
    if(remainingSize > 0){
        block->size = size;
        size_t offset = freeMemBlockSize + size;

        newBlock = (struct FreeMem*) ((void*)(block + offset));
        if(prev){
            prev->next = newBlock;
        }else{
            freeMemHead = newBlock;
        }
        newBlock->size = remainingSize-freeMemBlockSize;
        newBlock->next = block->next;
    }else {
        if(prev) prev->next = block->next;
        else freeMemHead = block->next;
    }
    block->next = 0;

    return (void*) ((char*)block + freeMemBlockSize);
}

int MemAllocator::mem_free(void* memory){
    uint64 memoryAddress = (uint64) memory;
    if(memoryAddress < (uint64) HEAP_START_ADDR || memoryAddress > (uint64) HEAP_END_ADDR){
        //memory outside of the heap
        return -2;
    }

    size_t freeMemBlockSize = sizeof(struct FreeMem);
    if (freeMemBlockSize%MEM_BLOCK_SIZE != 0){
        freeMemBlockSize = (freeMemBlockSize/MEM_BLOCK_SIZE + 1) * MEM_BLOCK_SIZE;
    }

    uint64 headerAddress = memoryAddress - freeMemBlockSize;

    struct FreeMem* header = (struct FreeMem*) headerAddress; // sa (void*) prabacen na (struct FreeMem*)

    struct FreeMem* tmp = freeMemHead;
    if (tmp == 0){
        //no memory prevously allocated
        return -1;
    }

    if (memoryAddress < (uint64) freeMemHead){
        //memory is before header
        header->next = freeMemHead;
        freeMemHead = header;

        return 0;
    }

    struct FreeMem* prev = 0;

    for( ; tmp->next != 0 ; prev = tmp, tmp = tmp->next){
        if(headerAddress > (uint64) prev && headerAddress < (uint64) tmp){
            prev->next = header;
            header->next = tmp;

            //inserted in middle of list
            return 0;
        }
    }

    tmp->next = header;
    //inserted on the end of list (most likely not going to happen)
    return 0;
}
