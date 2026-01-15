#pragma once

extern volatile int is_cialloing;

typedef unsigned long long uint64;

typedef struct context {
    uint64 rbx;
    uint64 rcx;
    uint64 rdx;
    uint64 rsi;
    uint64 rdi;
    uint64 rbp;
    uint64 rsp;
    uint64 r8;
    uint64 r9;
    uint64 r10;
    uint64 r11;
    uint64 r12;
    uint64 r13;
    uint64 r14;
    uint64 r15;
    uint64 eflags;
    uint64 rip;
} context;

typedef struct function {
    void (*function_ptr)(void *);
    void  *function_arg;
} function;

__attribute__((naked))
void contexts_save(context *);

__attribute__((naked, noreturn))
void contexts_load(context *);

int contexts_is_empty(void);
int contexts_is_full(void);
int contexts_append(const context *);
int contexts_remove(context *);

int functions_is_empty(void);
int functions_is_full(void);
int functions_append(const function *);
int functions_remove(function *);

int is_stack_top(void);
int update_stack_top(void);

#define CONTEXTS_MAX  0x100
#define FUNCTIONS_MAX 0x100

#define CONTEXTS_SAVE(curr) \
    asm volatile ( \
        "movq   %%rbx,     0x00(%0)\n\t" \
        "movq   %%rcx,     0x08(%0)\n\t" \
        "movq   %%rdx,     0x10(%0)\n\t" \
        "movq   %%rsi,     0x18(%0)\n\t" \
        "movq   %%rdi,     0x20(%0)\n\t" \
        "movq   %%rbp,     0x28(%0)\n\t" \
        "movq   %%rsp,     0x30(%0)\n\t" \
        "movq   %%r8,      0x38(%0)\n\t" \
        "movq   %%r9,      0x40(%0)\n\t" \
        "movq   %%r10,     0x48(%0)\n\t" \
        "movq   %%r11,     0x50(%0)\n\t" \
        "movq   %%r12,     0x58(%0)\n\t" \
        "movq   %%r13,     0x60(%0)\n\t" \
        "movq   %%r14,     0x68(%0)\n\t" \
        "movq   %%r15,     0x70(%0)\n\t" \
        "pushfq                    \n\t" \
        "popq   0x78(%0)           \n\t" \
        "leaq   1f(%%rip), %%rcx   \n\t" \
        "movq   %%rcx,     0x80(%0)\n\t" \
        : \
        : "a" (&curr) \
        : "memory")

#define CONTEXTS_LOAD(next) \
    asm volatile ( \
        "movq  0x00(%0), %%rbx\n\t" \
        "movq  0x08(%0), %%rcx\n\t" \
        "movq  0x10(%0), %%rdx\n\t" \
        "movq  0x18(%0), %%rsi\n\t" \
        "movq  0x20(%0), %%rdi\n\t" \
        "movq  0x28(%0), %%rbp\n\t" \
        "movq  0x30(%0), %%rsp\n\t" \
        "movq  0x38(%0), %%r8 \n\t" \
        "movq  0x40(%0), %%r9 \n\t" \
        "movq  0x48(%0), %%r10\n\t" \
        "movq  0x50(%0), %%r11\n\t" \
        "movq  0x58(%0), %%r12\n\t" \
        "movq  0x60(%0), %%r13\n\t" \
        "movq  0x68(%0), %%r14\n\t" \
        "movq  0x70(%0), %%r15\n\t" \
        "pushq 0x78(%0)       \n\t" \
        "popfq                \n\t" \
        "jmp   *0x80(%0)      \n\t" \
        "1:                   \n\t" \
        : \
        : "a" (&next) \
        : "memory")

#define INIT { \
    context curr; \
    context next; \
    CONTEXTS_SAVE(curr); \
    contexts_append(&curr); \
    if (!functions_is_empty()) { \
        function func; \
        functions_remove(&func); \
        func.function_ptr(func.function_arg); \
    } \
    update_stack_top(); \
    contexts_remove(&next); \
    CONTEXTS_LOAD(next); \
}

#define NEXT { \
    context curr; \
    context next; \
    CONTEXTS_SAVE(curr); \
    contexts_append(&curr); \
    if (is_stack_top()) { \
        if (!functions_is_empty()) { \
            function func; \
            functions_remove(&func); \
            func.function_ptr(func.function_arg); \
        } \
        update_stack_top(); \
    } \
    contexts_remove(&next); \
    CONTEXTS_LOAD(next); \
}

#define CALL(fun, arg) { \
    context curr; \
    context next; \
    CONTEXTS_SAVE(curr); \
    contexts_append(&curr); \
    if (is_stack_top()) { \
        fun(arg); \
    } \
    function func = {fun, arg}; \
    functions_append(&func); \
    contexts_remove(&next); \
    CONTEXTS_LOAD(next); \
}

#define EXIT { \
    context next = {0}; \
    if (is_stack_top()) { \
        update_stack_top(); \
    } \
    contexts_remove(&next); \
    CONTEXTS_LOAD(next); \
}

#define Ciaolima         do { INIT           } while (0)
#define Ciallo(fun, arg) do { CALL(fun, arg) } while (0)
#define Ciallo_          do { NEXT           } while (0)
#define Ciavanllo        do { EXIT           } while (0)