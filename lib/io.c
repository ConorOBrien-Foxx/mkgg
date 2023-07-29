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

// returns 0 for no instruction found, truthy otherwise
int step(MKGG_STATE* state) {
    if(!strcmp("df", *state->input_buffer)) {
        long long ftable_max = *(state->ftable_max);
        puts("==== DEBUGGING FUNCTION TABLE ====");
        printf("Table size : %lli\n", ftable_max);
        for(long long tmp = 0; tmp < ftable_max; tmp++) {
            char* item = (*state->ftable)[tmp];
            printf("Function[%lli] =\n  Name: '%s'\n", tmp, item);
            char* address = (char*) *(long long*) (item + BUFFER_MAX+1);
            printf("  Address = 0x%p\n  Representation = ", address);
            debug_function(address);
            
        }
    }
    else {
        return 0;
    }
    return 1;
}

EXPORT(step)
