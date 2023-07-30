#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
// TODO: add linux support for DLLs
#define M 1024 /* max string size */
#define W 65   /* max buffer size + 1 */
char B[W]={0}, // input buffer
*b, // buffer/string iterator
E; // eof type
long long S[M][2], // stack
(*P)[2]=S, // stack head pointer
(*J)[2], // stack iterator
A[M][2], // auxiliary stack
(*R)[2]=A, // auxiliary stack pointer
X, // function table max
t; // temporary

typedef int(*D)(
    char(*)[W],
    long long(*)[M][2],
    long long(**)[2],
    long long(*)[M][2],
    long long(**)[2],
    char(*)[M][W+8+1],
    long long*,
    char*(*)[M],
    char***,
    char(*)[M],
    void(*)(void)
);

// external libraries
D L[W]={0};

char p=
#ifdef _WIN32
'\\'
#else
'/'
#endif
;

// each item on the stack is an int pair [data,type]
char F[M][W+8+1]={0}; // function table (format: 16 character buffer, 0, 8 character "address"
// TODO: changing 25 to 26 in the above line fixed everything; am i off-by-one elsewhere, or is this the correct length?

char *Q[M]={0}, // call stack of read source (0 means read from stdin)
**H=Q; // head of the call stack

char G[M]={0},// file path handler
u[M],//other temporary buffer
*c;//generic char pointer

/*
void debug_function(char* addr) {
    puts("Debugging function...");
    for(int i = 0; *addr; i++, addr += W) {
        printf(" %i = [%s]\n", i, addr);
    }
    puts("Done debugging function!");
}

void debug_function_inline(char* addr) {
    printf("[ ");
    for(int i = 0; *addr; i++, addr += W) {
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
}*/

// get token, stores in B
FILE*Y; // source to read from
void g(){
*B=0;
// read from queue rather than stdin
if(H>Q){
    H--;
    if(*H && **H){
        strcpy(B,*H);
        *H+=W;
        // re-increment only if we have not reached eof of *H
        if(**H)H++;
    }
}
else{b=B;do*b=fgetc(Y);while(*b>32&&*b^';'&&++b-B<W-1);
if(*b==';')for(;*b!='\n'&&!feof(Y);*b=fgetc(Y));
E=*b;*b=0;}
}

#define C(s);}else if(!strcmp(#s,B)){

// uses the call stack to build the function
char* store_function(int count, int depth) {
    char temp_buff[W];
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
        address -= W;
        strcpy(address, temp_buff);
        return address;
    }
    // let's just do it lazily atm
    // printf("NEED TO ALLOCATE %i !!\n", count);
    // if(!count) count = 1;
    int amount = W * count;
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
while(subroutine ? start_h <= H : H > Q || !feof(Y)) {
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
            memcpy(F[idx] + W+1, (char*)&**P, sizeof(long long));
            // increment table counter if we made a new entry
            if(idx == X) X++;
        }
        else {
            // TODO: remove; golfing doesn't need error checking
            puts("ERROR: CANNOT STORE NON-FUNCTION AS FUNCTION");
            return 1;
        }
    }
    else if(*B == '%') {
        // TODO: .dll/.so for windows/linux
        // load from file
        if(B[1] == '/') {
            // @/ relative load
            sprintf(u,"%s.dll",B+2);
        }
        else {
            // @ library load
            sprintf(u,"%s%clib%c%s.dll",G,p,p,B+1);
        }
        // printf("Gonna read from [%s]\n", u);
        HMODULE l=LoadLibrary(u);
        // TODO: don't allow duplicates?
        if(l) {
            D*q=L;
            for(;*q;q++);
            // printf("Saving to %lli...\n", q-L);
            *q=(D)GetProcAddress(l,"mkgg_step");
        }
        else {
            puts("Could not load .dll");
        }
    }
    else if(*B == '#') {
        // load from file
        if(B[1] == '/') {
            // #/ relative load
            sprintf(u,"%s.mkgg",B+2);
        }
        else {
            // # library load
            sprintf(u,"%s%clib%c%s.mkgg",G,p,p,B+1);
        }
        // printf("Reading from %s...\n", u);
        // TODO: multiple files?
        // TODO: only include once?
        FILE* my_file = fopen(u,"r");
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
    else if(!strcmp("while", B) || !strcmp("if", B)) {
         // --> [ body ] [ condition ] while/if
        // puts("LET'S START WHILE!");
        // debug_stack();
        int is_if = *B == 'i';
        P--;
        
        // GOLF: (char*) is not necessary
        char* cond_addr = (char*) **P;
        // puts("COND READ!");
        // debug_stack();
        P--;
        char* body_addr = (char*) **P;
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
    else {
        // check against externals
        int skip=0;
        for(D*q=L;*q&&!skip;q++) {
            // printf("Checking %lli...\n", q-L);
            skip=(*q)(
                &B,//input buffer
                &S,//stack
                &P,//stack head pointer
                &A,//auxiliary stack
                &R,//auxiliary stack pointer
                &F,//function table
                &X,//function table max
                &Q,//call stack
                &H,//call stack head
                &u,//base path
                &g //get token
            );
        }
        // alternatively,
        // check if in function table
        // printf("Checking against unknown token '%s'\n", B);
        // puts("Examining function table");
        for(long long tmp = 0; !skip && tmp < X; tmp++) {
            // printf(" %lli = '%s'\n", tmp, F[tmp]);
            if(!strcmp(F[tmp], B)) {
                // puts("INNER HIT!");
                // printf("%*s| Expanding definition...\n", depth*4, "");
                char* address = (char*) *(long long*) (F[tmp] + W+1);
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
int main(int argc, char** argv){
    Y=stdin;
    // initialize base path
    strncpy(G,*argv,(c=strrchr(*argv,p))?c-*argv:0);
    if(!*G)*G='.';
    
    // TODO: golf
    return interp(0);
}