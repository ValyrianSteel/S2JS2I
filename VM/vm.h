//
// Created by ubun on 16-5-13.
//

#ifndef LC_VM_H
#define LC_VM_H


#include <stdbool.h>

#define STACK_SIZE 256
int stack[STACK_SIZE];

/** Instructions */
typedef enum {
    // HLT, // 0   84  -- hlt              :: halts program
    // PSH, // 1   3  -- psh val          :: pushes <val> to stack
    // POP, // 2   1  -- pop              :: pops value from stack
    // ADD, // 3   55  -- add              :: adds top two vals on stack
    // MUL, // 4   57  -- mul              :: multiplies top two vals on stack
    // DIV, // 5   58  -- div              :: divides top two vals on stack
    // SUB, // 6   56  -- sub              :: subtracts top two vals on stack
    // SLT, // 7   85  -- slt reg_a, reg_b :: pushes (reg_a < reg_b) to stack
    // MOV, // 8   84  -- mov reg_a, reg_b :: movs the value in reg_a to reg_b
    // SET, // 9   83  -- set reg, val     :: sets the reg to value
    // LOG, // 10  88  -- log a            :: prints out a
    // IF,  // 11  60 -- if reg val ip    :: if the register == val branch to the ip
    // IFN, // 12  61 -- ifn reg val ip   :: if the register != val branch to the ip
    // GLD, // 13  89-- gld reg          :: loads a register to the stack
    // GPT, // 14  90-- gpt reg          :: pushes top of stack to the given register
    // NOP, // 15  0 -- nop              :: nothing

    VM_OC_NONE,                    // 0  -- none /**< do nothing */
    VM_OC_POP,                     // 1  -- pop /**< pop from stack */
    VM_OC_POP_BLOCK,               // 2  -- /**< pop block */
    VM_OC_PUSH,                    // 3  -- /**< push one element  */
    VM_OC_PUSH_TWO,                // 4  -- /**< push two elements onto the stack */
    VM_OC_PUSH_THREE,              // 5  --/**< push three elements onto the stack */
    VM_OC_PUSH_UNDEFINED,          // 6  /**< push undefined value */
    VM_OC_PUSH_TRUE,               // 7  /**< push true value */
    VM_OC_PUSH_FALSE,              // 8  /**< push false value */
    VM_OC_PUSH_NULL,               // 9  /**< push null value */
    VM_OC_PUSH_THIS,               // 10 /**< push this */
    VM_OC_PUSH_NUMBER,             // 11 /**< push number */
    VM_OC_PUSH_OBJECT,             // 12 /**< push object */
    VM_OC_SET_PROPERTY,            // 13 /**< set property */
    VM_OC_SET_GETTER,              // 14 /**< set getter */
    VM_OC_SET_SETTER,              // 15 /**< set setter */
    VM_OC_PUSH_UNDEFINED_BASE,     // 16 /**< push undefined base */
    VM_OC_PUSH_ARRAY,              // 17 /**< push array */
    VM_OC_PUSH_ELISON,             // 18 /**< push elison */
    VM_OC_APPEND_ARRAY,            // 19 /**< append array */
    VM_OC_IDENT_REFERENCE,         // 20 /**< ident reference */
    VM_OC_PROP_REFERENCE,          // 21 /**< prop reference */
    VM_OC_PROP_GET,                // 22 /**< prop get */
    // 23
    /* These eight opcodes must be in this order. */
            VM_OC_PROP_PRE_INCR,           // 24 /**< prefix increment of a property */
    VM_OC_PRE_INCR,                // 25 /**< prefix increment  */
    VM_OC_PROP_PRE_DECR,           // 26 /**< prop prefix decrement of a property */
    VM_OC_PRE_DECR,                // 27 /**< prefix decrement */
    VM_OC_PROP_POST_INCR,          // 28 /**< prop postfix increment of a property */
    VM_OC_POST_INCR,               // 29 /**< postfix increment */
    VM_OC_PROP_POST_DECR,          // 30 /**< prop postfix decrement of a property */
    VM_OC_POST_DECR,               // 31 /**< postfix decrement */

    VM_OC_PROP_DELETE,             // 32 /**< delete property */
    VM_OC_DELETE,                  // 33 /**< delete */

    VM_OC_ASSIGN,                  // 34 /**< assign */
    VM_OC_ASSIGN_PROP,             // 35 /**< assign property */
    VM_OC_ASSIGN_PROP_THIS,        // 36 /**< assign prop this */

    VM_OC_RET,                     // 37 /**< return */
    VM_OC_THROW,                   // 38 /**< throw */
    VM_OC_THROW_REFERENCE_ERROR,   // 39 /**< throw reference error */

    VM_OC_EVAL,                    // 40 /**< eval */
    VM_OC_CALL,                    // 41 /**< call */
    VM_OC_NEW,                     // 42 /**< new */

    VM_OC_JUMP,                    // 43 /**< jump */
    VM_OC_BRANCH_IF_STRICT_EQUAL,  // 44 /**< branch if stric equal */

    /* These four opcodes must be in this order. */
            VM_OC_BRANCH_IF_TRUE,          // 45 /**< branch if true */
    VM_OC_BRANCH_IF_FALSE,         // 46 /**< branch if false */
    VM_OC_BRANCH_IF_LOGICAL_TRUE,  // 47 /**< branch if logical true */
    VM_OC_BRANCH_IF_LOGICAL_FALSE, // 48 /**< branch if logical false */

    VM_OC_PLUS,                    // 49 /**< unary plus */
    VM_OC_MINUS,                   // 50 /**< unary minus */
    VM_OC_NOT,                     // 51 /**< not */
    VM_OC_BIT_NOT,                 // 52 /**< bitwise not */
    VM_OC_VOID,                    // 53 /**< void */
    VM_OC_TYPEOF_IDENT,            // 54 /**< typeof identifier */
    VM_OC_TYPEOF,                  // 55 /**< typeof */

    VM_OC_ADD,                     // 56 /**< binary add */
    VM_OC_SUB,                     // 57 /**< binary sub */
    VM_OC_MUL,                     // 58 /**< mul */
    VM_OC_DIV,                     // 59 /**< div */
    VM_OC_MOD,                     // 60 /**< mod */

    VM_OC_EQUAL,                   // 61 /**< equal */
    VM_OC_NOT_EQUAL,               // 62 /**< not equal */
    VM_OC_STRICT_EQUAL,            // 63 /**< strict equal */
    VM_OC_STRICT_NOT_EQUAL,        // 64 /**< strict not equal */
    VM_OC_LESS,                    // 65 /**< less */
    VM_OC_GREATER,                 // 66 /**< greater */
    VM_OC_LESS_EQUAL,              // 67 /**< less equal */
    VM_OC_GREATER_EQUAL,           // 68 /**< greater equal */
    VM_OC_IN,                      // 69 /**< in */
    VM_OC_INSTANCEOF,              // 70 /**< instanceof */

    VM_OC_BIT_OR,                  // 71 /**< bitwise or */
    VM_OC_BIT_XOR,                 // 72 /**< bitwise xor */
    VM_OC_BIT_AND,                 // 73 /**< bitwise and */
    VM_OC_LEFT_SHIFT,              // 74 /**< left shift */
    VM_OC_RIGHT_SHIFT,             // 75 /**< right shift */
    VM_OC_UNS_RIGHT_SHIFT,         // 76 /**< unsigned right shift */

    VM_OC_WITH,                    // 77 /**< with */
    VM_OC_FOR_IN_CREATE_CONTEXT,   // 78 /**< for in create context */
    VM_OC_FOR_IN_GET_NEXT,         // 79 /**< get next */
    VM_OC_FOR_IN_HAS_NEXT,         // 80 /**< has next */
    VM_OC_TRY,                     // 81 /**< try */
    VM_OC_CATCH,                   // 82 /**< catch */
    VM_OC_FINALLY,                 // 83 /**< finally */
    VM_OC_CONTEXT_END,             // 84 /**< context end */
    VM_OC_JUMP_AND_EXIT_CONTEXT,   // 85 /**< jump and exit context */


    VM_OC_SLT,                     // 86 -- slt reg_a, reg_b :: pushes (reg_a < reg_b) to stack
    VM_OC_MOV,                     // 87 -- mov reg_a, reg_b :: movs the value in reg_a to reg_b
    VM_OC_SET,                     // 88 -- set reg, val     :: sets the reg to value
    VM_OC_LOG,                     // 89 -- log a            :: prints out a
    VM_OC_GLD,                     // 90 -- gld reg          :: loads a register to the stack
    VM_OC_GPT,                     // 91 -- gpt reg          :: pushes top of stack to the given register

} Instructions;

/** Registers */
typedef enum {
    A, B, C, D, E, F, I, J, // GENERAL PURPOSE
    EX,                     // EXCESS
    EXA,                    // MORE EXCESS
    IP,                     // INSTRUCTION POINTER
    SP,                     // STACK POINTER
    REGISTER_SIZE
} Registers;


/** prints the stack from A to B */
void print_stack();

void print_registers();

int find_empty_register();

void eval(int instr);

void vm_init();

int vm_run(int* instr, int instr_count);

void vm_finalize();

void vm_print(const char* lit_string);

#endif //LC_VM_H
