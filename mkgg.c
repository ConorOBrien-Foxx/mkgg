#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define M 1024
char B[17]={0}, // input buffer
*b, // buffer/string iterator
E; // eof type
long long S[M][2], // stack
(*P)[2]=S, // stack head pointer
(*J)[2], // stack iterator
A[M][2], // auxiliary stack
(*R)[2]=A, // auxiliary stack pointer
X; // function table max

// each item on the stack is an int pair [data,type]
char F[M][26]={0}; // function table (format: 16 character buffer, 0, 8 character "address"
// TODO: changing 25 to 26 in the above line fixed everything; am i off-by-one elsewhere, or is this the correct length?

char *Q[M]={0}, // call stack of read source (0 means read from stdin)
**H=Q; // head of the call stack

void debug_function(char* addr) {
    puts("Debugging function...");
    for(int i = 0; *addr; i++, addr += 17) {
        printf(" %i = [%s]\n", i, addr);
    }
    puts("Done debugging function!");
}

void debug_function_inline(char* addr) {
    printf("[ ");
    for(int i = 0; *addr; i++, addr += 17) {
        printf("%s ", addr);
    }
    printf("]\n");
}

int stack_debug_counter = 0;
void debug_stack(void) {
    
    printf("\n== Stack Debug #%i ==\n", stack_debug_counter++);
    for(J = S; J < P; J++) {
        if((*J)[1] == 1) {
            printf("%4lli: ", J - S);
            char* address = (char*) (*J)[0];
            debug_function_inline(address);
        }
        else {
            printf("%4lli: %lli (type: %lli)\n", J - S, (*J)[0], (*J)[1]);
        }
    }
    puts("== /Stack Debug ==");
}

// get token, stores in B
FILE*Y; // source to read from
void g(){
*B=0;
// read from queue rather than stdin
if(H>Q){
    H--;
    if(*H && **H){
        strcpy(B,*H);
        *H+=17;
        // re-increment only if we have not reached eof of *H
        if(**H)H++;
    }
}
else{b=B;do*b=fgetc(Y);while(*b>32&&*b^';'&&++b-B<16);
if(*b==';')for(;*b!='\n'&&!feof(Y);*b=fgetc(Y));
E=*b;*b=0;}
}

#define C(s);}else if(!strcmp(#s,B)){

// uses the call stack to build the function
char* store_function(int count, int depth) {
    char temp_buff[17];
    // skip empty tokens
    // TODO: fix (potential) crash when mismatched
    do g(); while(!*B);
    if(*B == '[') {
        depth++;
    }
    if(*B == ']') {
        depth--;
    }
    if(depth) {
        strcpy(temp_buff, B);
        char* address = store_function(count + 1, depth);
        address -= 17;
        strcpy(address, temp_buff);
        return address;
    }
    // let's just do it lazily atm
    // printf("NEED TO ALLOCATE %i !!\n", count);
    // if(!count) count = 1;
    int amount = 17 * count;
    // printf("----> data with %i bytes\n", amount + 1);
    // char* memory = calloc(amount + 1, 1);
    char* memory = malloc(amount + 1);
    memory[amount] = 0;
    return memory + amount;
}

void debug_pair(long long pair[2]) {
    printf("(%lli, %lli)", pair[0], pair[1]);
}

int interp(int subroutine) {

// printf("Interpreting with depth = %i...\n", depth);
// while(H >= Q + depth && (depth || !feof(Y))) {
// printf("LOOP VARIABLES\n");
// printf("  H - (Q + depth) = %lli\n", H - (Q + depth));
// printf("  depth           = %i\n", depth);
// printf("  !feof(Y)        = %i\n", !feof(Y));
// printf(" B O P \n");
char** start_h = H;
while(subroutine ? start_h <= H : !feof(Y)) {
    // debug_stack();
    // printf("LOOP VARIABLES\n");
    // printf("  H - (Q + depth) = %lli\n", H - (Q + depth));
    // printf("  depth           = %i\n", depth);
    // printf("  !feof(Y)        = %i\n", !feof(Y));
    // printf(" B O P \n");
    // read a token
    g();
    // printf("%*sToken = '%s'\n", 4, "", B);
    if(*B == '[') {
        char* result = store_function(0, 1);
        // puts("About to store function:");
        // debug_function(result);
        **P = (long long) result; // <=> (*P)[0] = result;
        1[*P] = 1; // function type; <=> (*P)[1]
        // puts("-- pushed function to stack --");
        P++;
    }
    else if(*B == '"') {
        printf("%s",B+1);
    }
    else if(*B == '!') {
        // TODO: remove (long long) everywhere
        *H++ = (char*) (long long) **--P;
    }
    else if(*B == ':') {
        // move to corresponding entry
        P--;
        if(1[*P]) { // if it's a function
            // check if name already exists
            int idx = 0;
            // ...by looping forward through each existing name
            for(; idx < X && strcmp(F[idx], B+1); idx++);
            // printf("Storing function %s @ X=%lli...\n", B, X);
            // add name to compare for
            strcpy(F[idx], B + 1);
            // store address (**P) into last 8 bytes of F[idx]
            memcpy(F[idx] + 18, (char*)&**P, sizeof(long long));
            // increment table counter if we made a new entry
            if(idx == X) X++;
        }
        else {
            // TODO: remove; golfing doesn't need error checking
            puts("ERROR: CANNOT STORE NON-FUNCTION AS FUNCTION");
            return 1;
        }
    }
    else if(*B == '#') {
        // load from file
        // use last entry in function table as a temporary
        // this works since it is 8 characters longer than a buffer,
        // and .mkgg\0 is only 6 characters
        sprintf(F[X],"%s.mkgg",B+1);
        // TODO: multiple files?
        // TODO: only include once?
        FILE* my_file = fopen(F[X],"r");
        Y = my_file;
        interp(0);
        fclose(my_file);
        //TODO: this will need to change if multiple files
        Y = stdin;
    }
    else if(('0' <= *B && *B <= '9') || *B == '-') {
        **P = atoll(B);
        1[*P] = 0; // 0 = integral type
        P++;
    }
    else if(!strcmp("give", B)) {
        memcpy(*R++, *--P, sizeof(*R));
    }
    else if(!strcmp("take", B)) {
        memcpy(*P++, *--R, sizeof(*P));
    }
    else if(!strcmp("add", B)) {
        P--;
        P[-1][0] += **P;
        P[-1][1] |= 1[*P];
    }
    else if(!strcmp("and", B)) {
        P--;
        P[-1][0] &= **P;
        P[-1][1] |= 1[*P];
    }
    else if(!strcmp("or", B)) {
        P--;
        P[-1][0] |= **P;
        P[-1][1] |= 1[*P];
    }
    else if(!strcmp("mul", B)) {
        P--;
        P[-1][0] *= **P;
        P[-1][1] |= 1[*P];
    }
    else if(!strcmp("div", B)) {
        P--;
        P[-1][0] /= **P;
        P[-1][1] |= 1[*P];
    }
    else if(!strcmp("mod", B)) {
        P--;
        P[-1][0] %= **P;
        P[-1][1] |= 1[*P];
    }
    else if(!strcmp("cmp", B)) {
        P--;
        P[-1][0] = (P[-1][0]>**P)-(P[-1][0]<**P);
        P[-1][1] |= 1[*P];
    }
    else if(!strcmp("roll", B)) {
        int amt = **--P;
        // printf("NUMBER OF INDICES TO ROLL = %i\n", amt);
        for(int i = 0; i < amt; i++) {
            long long idx = **(P - amt + i);
            // printf("The %i-th index is %i\n", i, idx);
            // TODO: golf placement formula?
            memcpy(P + i, P - 2 * amt + idx, sizeof(*P));
            // memcpy(P + amt - i - 1, P - amt - idx - 1, sizeof(*P));
        }
        // copy the generated swath to the right place
        memcpy(P - 2 * amt, P, amt * sizeof(*P));
        // "pop" the indices
        P -= amt;
    }
    else if(!strcmp("putc", B)) {
        putchar(**--P);
    }
    else if(!strcmp("putn", B)) {
        printf("%lli", **--P);
    }
    else if(!strcmp("while", B) || !strcmp("if", B)) {
         // --> [ body ] [ condition ] while/if
        // puts("LET'S START WHILE!");
        // debug_stack();
        int is_if = *B == 'i';
        P--;
        char* cond_addr = **P;
        // puts("COND READ!");
        // debug_stack();
        P--;
        char* body_addr = **P;
        // puts("BODY READ!");
        // debug_stack();
        
        for(;;) {
            // puts("Read+execute condition");
        // debug_stack();
            *H++ = cond_addr;
            interp(1);
            // pop and observe
            // puts("Check+pop condition result");
        // debug_stack();
            if(!**--P) break;
            
            // puts("Read+execute body");
        // debug_stack();
            *H++ = body_addr;
            interp(1);
            
            if(is_if) break;
        }
    }
    // debug functions; todo, remove?
    else if(!strcmp("debug", B)) {
        debug_stack();
    }
    else if(!strcmp("df", B)) {
        puts("==== DEBUGGING FUNCTION TABLE ====");
        for(long long tmp = 0; tmp < X; tmp++) {
            printf(" %lli = '%s'\n", tmp, F[tmp]);
            char* address = (char*) *(long long*) (F[tmp] + 18);
            printf(" Address = %p\n", address);
            debug_function(address);
        }
    }
    else {
        // check if in function table
        // printf("Checking against unknown token '%s'\n", B);
        // puts("Examining function table");
        for(long long tmp = 0; tmp < X; tmp++) {
            // printf(" %lli = '%s'\n", tmp, F[tmp]);
            if(!strcmp(F[tmp], B)) {
                // puts("INNER HIT!");
                // printf("%*s| Expanding definition...\n", depth*4, "");
                char* address = (char*) *(long long*) (F[tmp] + 18);
                // printf("~> Address = %p\n", address);
                *H++ = address;
                break;
            }
        }
        // printf("Examined all %lli entries.\n", X);
    }
    // TODO: load context from file
    // TODO: comments
    // TODO: if statements
    //   --> maybe through composition of lambdas?
    //   --> or through local variables/functions
    // [ if-body 0 ] [ if-condition] while pop
    // TODO: division
    // TODO: modulo

}
// printf("returning...\n");

return 0;
}

// main interpreter
int main(){
    Y=stdin;
    // TODO: golf
    return interp(0);
}