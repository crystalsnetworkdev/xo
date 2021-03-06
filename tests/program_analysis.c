#include "check.h"
#include "../src/program.h"
#include "../src/register_set.h"

#define R0 XO_REGISTER_SET_R0
#define R1 XO_REGISTER_SET_R1
#define R2 XO_REGISTER_SET_R2
#define R3 XO_REGISTER_SET_R3
#define R4 XO_REGISTER_SET_R4
#define R5 XO_REGISTER_SET_R5
#define R6 XO_REGISTER_SET_R6
#define R7 XO_REGISTER_SET_R7

// test helper function
static bool program_regs(const char *input, xo_register_set expected_input_regs, xo_register_set expected_output_regs)
{
  xo_program *prog = xo_program_create_from_str(input);
  if(!prog)
    return false;

  xo_register_set input_regs = 0, output_regs = 0;
  xo_program_analyze(prog, &input_regs, &output_regs);

  if(input_regs != expected_input_regs || output_regs != expected_output_regs)
  {
    fprintf(stderr, "i: 0x%.2x != 0x%.2x\n", input_regs, expected_input_regs);
    fprintf(stderr, "o: 0x%.2x != 0x%.2x\n", output_regs, expected_output_regs);
    return false;
  }

  return true;
}

int main()
{
  // nullary insns
  CHECK(program_regs("clc;", 0, 0));
  CHECK(program_regs("cmc;", 0, 0));
  CHECK(program_regs("stc;", 0, 0));

  // unary insns
  CHECK(program_regs("dec r0;", R0, R0));
  CHECK(program_regs("inc r0;", R0, R0));
  CHECK(program_regs("neg r0;", R0, R0));
  CHECK(program_regs("not r0;", R0, R0));

  // binary insns
  CHECK(program_regs("adc r0,r1;",    R0|R1, R0));
  CHECK(program_regs("adc r0,r0;",    R0,    R0));
  CHECK(program_regs("add r0,r1;",    R0|R1, R0));
  CHECK(program_regs("add r0,r0;",    R0,    R0));
  CHECK(program_regs("and r0,r1;",    R0|R1, R0));
  CHECK(program_regs("and r0,r0;",    R0,    R0));
  CHECK(program_regs("cmova r0,r1;",  R0|R1, R0));
  CHECK(program_regs("cmova r0,r0;",  R0,    R0));
  CHECK(program_regs("cmovbe r0,r1;", R0|R1, R0));
  CHECK(program_regs("cmovbe r0,r0;", R0,    R0));
  CHECK(program_regs("cmovc r0,r1;",  R0|R1, R0));
  CHECK(program_regs("cmovc r0,r0;",  R0,    R0));
  CHECK(program_regs("cmovg r0,r1;",  R0|R1, R0));
  CHECK(program_regs("cmovg r0,r0;",  R0,    R0));
  CHECK(program_regs("cmovge r0,r1;", R0|R1, R0));
  CHECK(program_regs("cmovge r0,r0;", R0,    R0));
  CHECK(program_regs("cmovl r0,r1;",  R0|R1, R0));
  CHECK(program_regs("cmovl r0,r0;",  R0,    R0));
  CHECK(program_regs("cmovle r0,r1;", R0|R1, R0));
  CHECK(program_regs("cmovle r0,r0;", R0,    R0));
  CHECK(program_regs("cmovnc r0,r1;", R0|R1, R0));
  CHECK(program_regs("cmovnc r0,r0;", R0,    R0));
  CHECK(program_regs("cmovno r0,r1;", R0|R1, R0));
  CHECK(program_regs("cmovno r0,r0;", R0,    R0));
  CHECK(program_regs("cmovnp r0,r1;", R0|R1, R0));
  CHECK(program_regs("cmovnp r0,r0;", R0,    R0));
  CHECK(program_regs("cmovns r0,r1;", R0|R1, R0));
  CHECK(program_regs("cmovns r0,r0;", R0,    R0));
  CHECK(program_regs("cmovnz r0,r1;", R0|R1, R0));
  CHECK(program_regs("cmovnz r0,r0;", R0,    R0));
  CHECK(program_regs("cmovo r0,r1;",  R0|R1, R0));
  CHECK(program_regs("cmovo r0,r0;",  R0,    R0));
  CHECK(program_regs("cmovp r0,r1;",  R0|R1, R0));
  CHECK(program_regs("cmovp r0,r0;",  R0,    R0));
  CHECK(program_regs("cmovs r0,r1;",  R0|R1, R0));
  CHECK(program_regs("cmovs r0,r0;",  R0,    R0));
  CHECK(program_regs("cmovz r0,r1;",  R0|R1, R0));
  CHECK(program_regs("cmovz r0,r0;",  R0,    R0));
  CHECK(program_regs("cmp r0,r1;",    R0|R1, 0));  // no output
  CHECK(program_regs("cmp r0,r0;",    R0,    0));  // no output
  CHECK(program_regs("mov r0,r1;",    R1,    R0)); // output is not also input
  CHECK(program_regs("mov r0,r0;",    R0,    R0)); // output is not also input
  CHECK(program_regs("or r0,r1;",     R0|R1, R0));
  CHECK(program_regs("or r0,r0;",     R0,    R0));
  CHECK(program_regs("sbb r0,r1;",    R0|R1, R0));
  CHECK(program_regs("sbb r0,r0;",    0,     R0)); // no input
  CHECK(program_regs("sub r0,r1;",    R0|R1, R0));
  CHECK(program_regs("sub r0,r0;",    0,     R0)); // no input
  CHECK(program_regs("xor r0,r1;",    R0|R1, R0));
  CHECK(program_regs("xor r0,r0;",    0,     R0)); // no input

  // multiple insns
  CHECK(program_regs("add r0,r1;add r0,r1;",           R0|R1, R0));
  CHECK(program_regs("mov r0,r1;add r0,r1;",           R1,    R0));
  CHECK(program_regs("mov r1,r0;add r0,r1;",           R0,    R0));
  CHECK(program_regs("mov r0,r1;mov r2,r3;",           R1|R3, R2));
  CHECK(program_regs("mov r0,r1;mov r2,r3;mov r4,r0;", R1|R3, R4));

  return EXIT_SUCCESS;
}
