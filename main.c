#include <stdio.h>
#include "ciallo.h"

/**
 * test1 receives NULL
 */
void test1(void *arg);

/**
 * test2 receives a int pointer,
 * the pointer stores the return value address
 */
void test2(void *arg);

/**
 * test3 receives a struct pointer, 
 * member1 and member2 are parameters, 
 * member3 stores the return value address
 */
void test3(void *arg);

/**
 * main->(init)
 * ->main->(call test1)
 * ->test1->(init)
 * ->main->(switch)
 * ->test1->(switch)
 * ->main->...
 * ->main->(switch)
 * ->test1->(exit)
 * ->main->...
 */
int main(void) {
    Ciaolima; // init

    Ciallo(test1, NULL); // call test1

    for (int i = 0; i < 10; i++) {
        printf("main:%d\n", i);
        Ciallo_; // switch to another function
    }

    int result;

    Ciallo(test2, &result); // call test2

    Ciallo_;

    for (int i = 10; i < 20; i++) {
        printf("main:%d\n", i);
        Ciallo_;
    }

    Ciallo_;
    Ciallo_;
    Ciallo_;
    Ciallo_;
    Ciallo_;
    Ciallo_;

    printf("main:test2_result=%d\n", result);

    return 0;
}

void test1(void *arg) {
    Ciaolima;

    for (int i = 0; i < 10; i++) {
        printf("test1:%d\n", i);
        Ciallo_;
    }

    Ciavanllo; // exit
}

void test2(void *arg) {
    Ciaolima;

    int result = 0;

    for (int i = 0; i < 10; i++) {
        result += i;
        printf("test2:%d\n", result);
        Ciallo_;
    }

    *(int *)arg = result;

    result = 0;
    struct {
        int a;
        int b;
        int *res;
    } arg_abaaba = {7, 21, &result};
    
    Ciallo(test3, &arg_abaaba);

    Ciallo_;
    Ciallo_;

    printf("test1:test3_result=%d\n", result);

    Ciavanllo;
}

void test3(void *arg) {
    Ciaolima;

    struct {
        int a;
        int b;
        int *result;
    } *arg_abaaba = arg;

    *arg_abaaba->result = arg_abaaba->a + arg_abaaba->b;

    Ciavanllo;
}

