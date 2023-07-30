#include "../src/ext.h"
#include <stdio.h>
#include <string.h>

// gcc ext.c -o ext.dll -shared -Wall -g

void debug_function(char* addr) {
    printf("[ ");
    for(int i = 0; *addr; i++, addr += W) {
        printf("%s ", addr);
    }
    puts("]");
}

int stack_debug_counter = 0;

// returns 0 for no instruction found, truthy otherwise
int step(MKGG_STATE* state) {
    if(!strcmp("df", *state->input_buffer)) {
        long long ftable_max = *(state->ftable_max);
        puts("==== DEBUGGING FUNCTION TABLE ====");
        printf("Table size : %lli\n", ftable_max);
        for(long long tmp = 0; tmp < ftable_max; tmp++) {
            char* item = (*state->ftable)[tmp];
            printf("Function[%lli] =\n  Name: '%s'\n", tmp, item);
            // equiv. (char*) *(long long*)
            char* address = *(char**) (item + BUFFER_MAX+1);
            printf("  Address = 0x%p\n  Representation = ", address);
            debug_function(address);
            
        }
    }
    // debug functions; todo, remove?
    else if(!strcmp("debug", *state->input_buffer)) {
        printf("\n== Stack Debug #%i ==\n", stack_debug_counter++);
        for(long long (*J)[2] = *state->stack; J < *state->stack_pointer; J++) {
            if((*J)[1] == 1) {
                printf("%4lli: ", J - *state->stack);
                char* address = (char*) (*J)[0];
                debug_function(address);
            }
            else {
                printf("%4lli: %lli (type: %lli)\n", J - *state->stack, (*J)[0], (*J)[1]);
            }
        }
        puts("== /Stack Debug ==");
    }
    else if(!strcmp("putc", *state->input_buffer)) {
        putchar(**--*state->stack_pointer);
    }
    else if(!strcmp("putn", *state->input_buffer)) {
        printf("%lli", **--*state->stack_pointer);
    }
    else if(!strcmp("getc", *state->input_buffer)) {
        ***state->stack_pointer = getchar();
        1[**state->stack_pointer] = 0;
        ++*state->stack_pointer;
    }
    else if(!strcmp("getn", *state->input_buffer)) {
        // scanf(" %lld", &***state->stack_pointer);
        scanf(" %lld", **state->stack_pointer);
        1[**state->stack_pointer] = 0;
        ++*state->stack_pointer;
    }
    else {
        return 0;
    }
    return 1;
}

EXPORT(step)
