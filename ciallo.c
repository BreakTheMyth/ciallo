#include "ciallo.h"

/**
 * @deprecated 记录当前是否处于切换上下文状态
 */
volatile int is_cialloing = 0;

static context contexts[CONTEXTS_MAX] = {0};
static uint64  contexts_count         =  0;
static uint64  contexts_head          =  0;
static uint64  contexts_tail          =  0;

static function functions[FUNCTIONS_MAX] = {0};
static uint64   functions_count          =  0;
static uint64   functions_head           =  0;
static uint64   functions_tail           =  0;

static uint64 stack_top_index = 0;

/**
 * @deprecated 保存当前上下文
 */
__attribute__((naked))
void contexts_save(context *) {
    asm volatile (
        "movl   $1,        is_cialloing(%rip)\n\t"
#if defined(_WIN64)
        "movq   %rcx,      %rax              \n\t"
#elif defined(__x86_64__)
        "movq   %rdi,      %rax              \n\t"
#else
#error I refuse to compile
#endif
        "movq   %rbx,      0x00(%rax)        \n\t"
        "movq   %rcx,      0x08(%rax)        \n\t"
        "movq   %rdx,      0x10(%rax)        \n\t"
        "movq   %rsi,      0x18(%rax)        \n\t"
        "movq   %rdi,      0x20(%rax)        \n\t"
        "movq   %rbp,      0x28(%rax)        \n\t"
        "movq   %rsp,      0x30(%rax)        \n\t"
        "movq   %r8,       0x38(%rax)        \n\t"
        "movq   %r9,       0x40(%rax)        \n\t"
        "movq   %r10,      0x48(%rax)        \n\t"
        "movq   %r11,      0x50(%rax)        \n\t"
        "movq   %r12,      0x58(%rax)        \n\t"
        "movq   %r13,      0x60(%rax)        \n\t"
        "movq   %r14,      0x68(%rax)        \n\t"
        "movq   %r15,      0x70(%rax)        \n\t"
        "pushfq                              \n\t"
        "popq   0x78(%rax)                   \n\t"
        "ret                                 \n\t");
}

/**
 * @deprecated 切换至目标上下文并跳转
 */
__attribute__((naked, noreturn))
void contexts_load(context *) {
    asm volatile (
#if defined(_WIN64)
        "movq  %rcx,       %rax\n\t"
#elif defined(__x86_64__)
        "movq  %rdi,       %rax\n\t"
#else
#error I refuse to compile
#endif
        "movq  0x00(%rax), %rbx              \n\t"
        "movq  0x08(%rax), %rcx              \n\t"
        "movq  0x10(%rax), %rdx              \n\t"
        "movq  0x18(%rax), %rsi              \n\t"
        "movq  0x20(%rax), %rdi              \n\t"
        "movq  0x28(%rax), %rbp              \n\t"
        "movq  0x30(%rax), %rsp              \n\t"
        "movq  0x38(%rax), %r8               \n\t"
        "movq  0x40(%rax), %r9               \n\t"
        "movq  0x48(%rax), %r10              \n\t"
        "movq  0x50(%rax), %r11              \n\t"
        "movq  0x58(%rax), %r12              \n\t"
        "movq  0x60(%rax), %r13              \n\t"
        "movq  0x68(%rax), %r14              \n\t"
        "movq  0x70(%rax), %r15              \n\t"
        "pushq 0x78(%rax)                    \n\t"
        "popfq                               \n\t"
        "movl  $0,         is_cialloing(%rip)\n\t"
        "jmp   *0x80(%rax)                   \n\t");
}

int contexts_is_empty(void) {
    return contexts_count == 0;
}

int contexts_is_full(void) {
    return contexts_count == CONTEXTS_MAX;
}

int contexts_append(const context *curr) {
    if (contexts_is_full()) return -1;

    contexts[contexts_tail].rbx    = curr->rbx;
    contexts[contexts_tail].rcx    = curr->rcx;
    contexts[contexts_tail].rdx    = curr->rdx;
    contexts[contexts_tail].rsi    = curr->rsi;
    contexts[contexts_tail].rdi    = curr->rdi;
    contexts[contexts_tail].rbp    = curr->rbp;
    contexts[contexts_tail].rsp    = curr->rsp;
    contexts[contexts_tail].r8     = curr->r8;
    contexts[contexts_tail].r9     = curr->r9;
    contexts[contexts_tail].r10    = curr->r10;
    contexts[contexts_tail].r11    = curr->r11;
    contexts[contexts_tail].r12    = curr->r12;
    contexts[contexts_tail].r13    = curr->r13;
    contexts[contexts_tail].r14    = curr->r14;
    contexts[contexts_tail].r15    = curr->r15;
    contexts[contexts_tail].eflags = curr->eflags;
    contexts[contexts_tail].rip    = curr->rip;

    contexts_tail = (contexts_tail + 1) % CONTEXTS_MAX;

    contexts_count++;

    return 0;
}

int contexts_remove(context *next) {
    if (contexts_is_empty()) return -1;

    next->rbx    = contexts[contexts_head].rbx;
    next->rcx    = contexts[contexts_head].rcx;
    next->rdx    = contexts[contexts_head].rdx;
    next->rsi    = contexts[contexts_head].rsi;
    next->rdi    = contexts[contexts_head].rdi;
    next->rbp    = contexts[contexts_head].rbp;
    next->rsp    = contexts[contexts_head].rsp;
    next->r8     = contexts[contexts_head].r8;
    next->r9     = contexts[contexts_head].r9;
    next->r10    = contexts[contexts_head].r10;
    next->r11    = contexts[contexts_head].r11;
    next->r12    = contexts[contexts_head].r12;
    next->r13    = contexts[contexts_head].r13;
    next->r14    = contexts[contexts_head].r14;
    next->r15    = contexts[contexts_head].r15;
    next->eflags = contexts[contexts_head].eflags;
    next->rip    = contexts[contexts_head].rip;

    contexts_head = (contexts_head + 1) % CONTEXTS_MAX;

    contexts_count--;

    return 0;
}

int functions_is_empty(void) {
    return functions_count == 0;
}

int functions_is_full(void) {
    return functions_count == FUNCTIONS_MAX;
}

int functions_append(const function *element) {
    if (functions_is_full()) return -1;

    functions[functions_tail].function_ptr = element->function_ptr;
    functions[functions_tail].function_arg = element->function_arg;

    functions_tail = (functions_tail + 1) % FUNCTIONS_MAX;

    functions_count++;

    return 0;
}

int functions_remove(function *element) {
    if (functions_is_empty()) return -1;

    element->function_ptr = functions[functions_head].function_ptr;
    element->function_arg = functions[functions_head].function_arg;

    functions_head = (functions_head + 1) % FUNCTIONS_MAX;

    functions_count--;

    return 0;
}

int is_stack_top(void) {
    return stack_top_index == (contexts_head - 1 + CONTEXTS_MAX) % CONTEXTS_MAX;
}

int update_stack_top(void) {
    stack_top_index = (contexts_tail - 1 + CONTEXTS_MAX) % CONTEXTS_MAX;

    return 0;
}
