//
// Created by ubun on 16-5-13.
//

#include "vm.h"

#include <stdio.h>
#include <stdlib.h>


int registers[REGISTER_SIZE];

// instructions array
int *instructions;

// how many instructions to do
int instruction_count = 0;

// how much space is allocated for the instructions
// 4 instructions by default
int instruction_space = 4;

/** if the program is running */
bool running = true;

/** if the IP is assigned by jmp instructions(such as IF,IFN),it should not increase 1 any more **/
bool is_jmp = false;

/** quick ways to get SP and IP */
#define SP (registers[SP])
#define IP (registers[IP])

/** fetch current instruction set */
// #define FETCH (test_a[IP])
// #define FETCH (test_b[IP])
#define FETCH (instructions[IP])


/** prints the stack from A to B */
void print_stack() {
    for (int i = 0; i < SP; i++) {
        printf("0x%04d ", stack[i]);
        if ((i + 1) % 4 == 0) { printf("\n"); }
    }
    if (SP != 0) { printf("\n"); }
}

void print_registers() {
    printf("Register Dump:\n");
    for (int i = 0; i < REGISTER_SIZE; i++) {
        printf("%04d ", registers[i]);
        if ((i + 1) % 4 == 0) { printf("\n"); }
    }
}

int find_empty_register() {
    for (int i = 0; i < REGISTER_SIZE; i++) {
        if (i != registers[EX] && i != registers[EXA]) { return i; }
    }
    return EX;
}

void eval(int instr) {
    is_jmp = false;
    switch (instr) {
        case VM_OC_JUMP_AND_EXIT_CONTEXT: {
            running = false;
            printf("Finished Execution\n");
            // print_stack(0, 16);
            // print_registers();
            break;
        }
        case VM_OC_PUSH: {
            SP = SP + 1;
            IP = IP + 1;
            stack[SP] = instructions[IP];
            break;
        }
        case VM_OC_POP: {
            SP = SP - 1;
            break;
        }
        case VM_OC_ADD: {
            registers[A] = stack[SP];
            SP = SP - 1;

            registers[B] = stack[SP];
            /* SP = SP - 1; */

            registers[C] = registers[B] + registers[A];

            /* SP = SP + 1; */
            stack[SP] = registers[C];
            printf("%d + %d = %d\n", registers[B], registers[A], registers[C]);
            break;
        }
        case VM_OC_MUL: {
            registers[A] = stack[SP];
            SP = SP - 1;

            registers[B] = stack[SP];
            /*SP = SP - 1;*/

            registers[C] = registers[B] * registers[A];

            /*SP = SP + 1;*/
            stack[SP] = registers[C];
            printf("%d * %d = %d\n", registers[B], registers[A], registers[C]);
            break;
        }
        case VM_OC_DIV: {
            registers[A] = stack[SP];
            SP = SP - 1;

            registers[B] = stack[SP];
            /* SP = SP - 1;*/

            registers[C] = registers[B] / registers[A];

            /* SP = SP + 1; */
            stack[SP] = registers[C];
            printf("%d / %d = %d\n", registers[B], registers[A], registers[C]);
            break;
        }
        case VM_OC_SUB: {
            registers[A] = stack[SP];
            SP = SP - 1;

            registers[B] = stack[SP];
            /* SP = SP - 1; */

            registers[C] = registers[B] - registers[A];

            /* SP = SP + 1; */
            stack[SP] = registers[C];
            printf("%d - %d = %d\n", registers[B], registers[A], registers[C]);
            break;
        }
        case VM_OC_SLT: {
            SP = SP - 1;
            stack[SP] = stack[SP+1] < stack[SP];
            break;
        }
        case VM_OC_MOV: {
            registers[instructions[IP + 2]] = registers[instructions[IP + 1]];
            IP = IP + 2;
            break;
        }
        case VM_OC_SET: {
            registers[instructions[IP + 1]] = instructions[IP + 2];
            IP = IP + 2;
            break;
        }
        case VM_OC_LOG: {
            printf("%d\n", registers[instructions[IP + 1]]);
            IP = IP + 1;
            break;
        }
        case VM_OC_EQUAL: {
            if (registers[instructions[IP + 1]] == instructions[IP + 2]) {
                IP = instructions[IP + 3];
                is_jmp = true;
            }
            else{
                IP = IP + 3;
            }
            break;
        }
        case VM_OC_NOT_EQUAL: {
            if (registers[instructions[IP + 1]] != instructions[IP + 2]) {
                IP = instructions[IP + 3];
                is_jmp = true;
            }
            else {
                IP = IP + 3;
            }
            break;
        }
        case VM_OC_GLD: {
            SP = SP + 1;
            IP = IP + 1;
            stack[SP] = registers[instructions[IP]];
            break;
        }
        case VM_OC_GPT: {
            registers[instructions[IP + 1]] = stack[SP];
            IP = IP + 1;
            break;
        }
        case VM_OC_NONE: {
            printf("Do Nothing\n");
            break;
        }
        default: {
            printf("Unknown Instruction %d\n", instr);
            break;
        }
    }
}


void vm_init()
{

}

int vm_run(int* instr, int instr_count)
{
    // allocate space for instructions
    instructions = malloc(sizeof(*instructions) * instruction_space); // 4 instructions

    for (int i = 0; i < instr_count; ) {
        instructions[i] = instr[i];

        i++;
        if (i >= instruction_space) {
            instruction_space *= 2;
            instructions = realloc(instructions, sizeof(*instructions) * instruction_space); // double size
        }
    }

    // set 'instruction_count' to number of instructions read
    instruction_count = instr_count;


    // initialize stack pointer
    SP = -1;

    // loop through program, but don't
    // go out of the programs bounds
    while (running && IP < instruction_count) {
        eval(FETCH);
        if(!is_jmp){
            IP = IP + 1;
        }
    }

    // clean up instructions
    free(instructions);

    return 0;
}

void vm_finalize()
{

}


void vm_print(const char* lit_string)
{
    printf("%s", lit_string);
}