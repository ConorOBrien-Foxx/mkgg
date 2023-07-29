// (non-golfed) reference file for how to write an extension
#define ITEM_MAX (1024)
#define BUFFER_MAX (65)
#define FUNC_REF_MAX (BUFFER_MAX + 8 + 1)
#define W BUFFER_MAX
#define M ITEM_MAX

typedef long long MKGG_STACK[ITEM_MAX][2];
typedef char MKGG_BUFFER[BUFFER_MAX];
typedef char MKGG_LONG_BUFFER[ITEM_MAX];

typedef struct {
    MKGG_BUFFER* input_buffer;
    MKGG_STACK* stack;
    long long (**stack_pointer)[2];
    MKGG_STACK* aux_stack;
    long long (**aux_stack_pointer)[2];
    char (*ftable)[ITEM_MAX][FUNC_REF_MAX];
    long long* ftable_max;
    char* (*call_stack)[ITEM_MAX];
    char*** call_stack_pointer;
    MKGG_LONG_BUFFER* base_path;
    void (*read_token)(void);
} MKGG_STATE;

#define EXPORT(name)                            \
    int mkgg_step(                              \
        MKGG_BUFFER* input_buffer,              \
        MKGG_STACK* stack,                      \
        long long (**stack_pointer)[2],         \
        MKGG_STACK* aux_stack,                  \
        long long (**aux_stack_pointer)[2],     \
        char (*ftable)[ITEM_MAX][FUNC_REF_MAX], \
        long long* ftable_max,                  \
        char* (*call_stack)[ITEM_MAX],          \
        char*** call_stack_pointer,             \
        MKGG_LONG_BUFFER* base_path,            \
        void (*read_token)(void)                \
    ) {                                         \
        MKGG_STATE state = {                    \
            input_buffer,                       \
            stack,                              \
            stack_pointer,                      \
            aux_stack,                          \
            aux_stack_pointer,                  \
            ftable,                             \
            ftable_max,                         \
            call_stack,                         \
            call_stack_pointer,                 \
            base_path,                          \
            read_token                          \
        };                                      \
        return name(&state);                    \
    }
