#include "clex/clex.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef enum TokenKind {
  AAA,
  AAD,
  AAM,
  AAS,
  ADC,
  ADD,
  AND,
  CALL,
  CBW,
  CLC,
  CLD,
  CLI,
  CMC,
  CMP,
  CMPSB,
  CMPSW,
  CWD,
  DAA,
  DAS,
  DEC,
  DIV,
  HLT,
  IDIV,
  IMUL,
  IN,
  INC,
  INT,
  INTO,
  IRET,
  JA,
  JAE,
  JB,
  JBE,
  JC,
  JCXZ,
  JE,
  JG,
  JGE,
  JL,
  JLE,
  JMP,
  JNA,
  JNAE,
  JNB,
  JNBE,
  JNC,
  JNE,
  JNG,
  JNGE,
  JNL,
  JNLE,
  JNO,
  JNP,
  JNS,
  JNZ,
  JO,
  JP,
  JPE,
  JPO,
  JS,
  JZ,
  LAHF,
  LDS,
  LEA,
  LES,
  LODSB,
  LODSW,
  LOOP,
  LOOPE,
  LOOPNE,
  LOOPNZ,
  LOOPZ,
  MOV,
  MOVSB,
  MOVSW,
  MUL,
  NEG,
  NOP,
  NOT,
  OR,
  OUT,
  POP,
  POPA,
  POPF,
  PUSH,
  PUSHA,
  PUSHF,
  RCL,
  RCR,
  REP,
  REPE,
  REPNE,
  REPNZ,
  REPZ,
  RET,
  RETF,
  ROL,
  ROR,
  SAHF,
  SAL,
  SAR,
  SBB,
  SCASB,
  SCASW,
  SHL,
  SHR,
  STC,
  STD,
  STI,
  STOSB,
  STOSW,
  SUB,
  TEST,
  XCHG,
  XLATB,
  XOR,
  BITS,
  NUMBER,
  HEX,
  AX,
  BX,
  CX,
  DX,
  AH,
  BH,
  CH,
  DH,
  AL,
  BL,
  CL,
  DL,
  SP,
  DP,
  SI,
  DI,
  CS,
  DS,
  SS,
  ES,
  IP,
  FLAGS,
  OSBRACE,
  CSBRACE,
  COMMA,
} TokenKind;

char *assemble(void) {
  char *result = calloc(10000, sizeof(char));
  Token token;
  while ((token = clex()).kind != -1) {
    switch(token.kind) {
      case AAA:
        sprintf(result + strlen(result), "%c", 0x37);
      case MOV:
        sprintf(result + strlen(result), "mov");
      default:
        continue;
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
  clexRegisterKind("aaa", AAA);
  clexRegisterKind("aad", AAD);
  clexRegisterKind("aam", AAM);
  clexRegisterKind("aas", AAS);
  clexRegisterKind("adc", ADC);
  clexRegisterKind("add", ADD);
  clexRegisterKind("and", AND);
  clexRegisterKind("call", CALL);
  clexRegisterKind("cbw", CBW);
  clexRegisterKind("clc", CLC);
  clexRegisterKind("cld", CLD);
  clexRegisterKind("cli", CLI);
  clexRegisterKind("cmc", CMC);
  clexRegisterKind("cmp", CMP);
  clexRegisterKind("cmpsb", CMPSB);
  clexRegisterKind("cmpsw", CMPSW);
  clexRegisterKind("cwd", CWD);
  clexRegisterKind("daa", DAA);
  clexRegisterKind("das", DAS);
  clexRegisterKind("dec", DEC);
  clexRegisterKind("div", DIV);
  clexRegisterKind("hlt", HLT);
  clexRegisterKind("idiv", IDIV);
  clexRegisterKind("imul", IMUL);
  clexRegisterKind("in", IN);
  clexRegisterKind("inc", INC);
  clexRegisterKind("int", INT);
  clexRegisterKind("into", INTO);
  clexRegisterKind("iret", IRET);
  clexRegisterKind("ja", JA);
  clexRegisterKind("jae", JAE);
  clexRegisterKind("jb", JB);
  clexRegisterKind("jbe", JBE);
  clexRegisterKind("jc", JC);
  clexRegisterKind("jcxz", JCXZ);
  clexRegisterKind("je", JE);
  clexRegisterKind("jg", JG);
  clexRegisterKind("jge", JGE);
  clexRegisterKind("jl", JL);
  clexRegisterKind("jle", JLE);
  clexRegisterKind("jmp", JMP);
  clexRegisterKind("jna", JNA);
  clexRegisterKind("jnae", JNAE);
  clexRegisterKind("jnb", JNB);
  clexRegisterKind("jnbe", JNBE);
  clexRegisterKind("jnc", JNC);
  clexRegisterKind("jne", JNE);
  clexRegisterKind("jng", JNG);
  clexRegisterKind("jnge", JNGE);
  clexRegisterKind("jnl", JNL);
  clexRegisterKind("jnle", JNLE);
  clexRegisterKind("jno", JNO);
  clexRegisterKind("jnp", JNP);
  clexRegisterKind("jns", JNS);
  clexRegisterKind("jnz", JNZ);
  clexRegisterKind("jo", JO);
  clexRegisterKind("jp", JP);
  clexRegisterKind("jpe", JPE);
  clexRegisterKind("jpo", JPO);
  clexRegisterKind("js", JS);
  clexRegisterKind("jz", JZ);
  clexRegisterKind("lahf", LAHF);
  clexRegisterKind("lds", LDS);
  clexRegisterKind("lea", LEA);
  clexRegisterKind("les", LES);
  clexRegisterKind("lodsb", LODSB);
  clexRegisterKind("lodsw", LODSW);
  clexRegisterKind("loop", LOOP);
  clexRegisterKind("loope", LOOPE);
  clexRegisterKind("loopne", LOOPNE);
  clexRegisterKind("loopnz", LOOPNZ);
  clexRegisterKind("loopz", LOOPZ);
  clexRegisterKind("mov", MOV);
  clexRegisterKind("movsb", MOVSB);
  clexRegisterKind("movsw", MOVSW);
  clexRegisterKind("mul", MUL);
  clexRegisterKind("neg", NEG);
  clexRegisterKind("nop", NOP);
  clexRegisterKind("not", NOT);
  clexRegisterKind("or", OR);
  clexRegisterKind("out", OUT);
  clexRegisterKind("pop", POP);
  clexRegisterKind("popa", POPA);
  clexRegisterKind("popf", POPF);
  clexRegisterKind("push", PUSH);
  clexRegisterKind("pusha", PUSHA);
  clexRegisterKind("pushf", PUSHF);
  clexRegisterKind("rcl", RCL);
  clexRegisterKind("rcr", RCR);
  clexRegisterKind("rep", REP);
  clexRegisterKind("repe", REPE);
  clexRegisterKind("repne", REPNE);
  clexRegisterKind("repnz", REPNZ);
  clexRegisterKind("repz", REPZ);
  clexRegisterKind("ret", RET);
  clexRegisterKind("retf", RETF);
  clexRegisterKind("rol", ROL);
  clexRegisterKind("ror", ROR);
  clexRegisterKind("sahf", SAHF);
  clexRegisterKind("sal", SAL);
  clexRegisterKind("sar", SAR);
  clexRegisterKind("sbb", SBB);
  clexRegisterKind("scasb", SCASB);
  clexRegisterKind("scasw", SCASW);
  clexRegisterKind("shl", SHL);
  clexRegisterKind("shr", SHR);
  clexRegisterKind("stc", STC);
  clexRegisterKind("std", STD);
  clexRegisterKind("sti", STI);
  clexRegisterKind("stosb", STOSB);
  clexRegisterKind("stosw", STOSW);
  clexRegisterKind("sub", SUB);
  clexRegisterKind("test", TEST);
  clexRegisterKind("xchg", XCHG);
  clexRegisterKind("xlatb", XLATB);
  clexRegisterKind("xor", XOR);
  clexRegisterKind("bits", BITS);
  clexRegisterKind("[0-9][0-9]*", NUMBER);
  clexRegisterKind("0x([A-F0-9])*", HEX);
  clexRegisterKind("ax", AX);
  clexRegisterKind("bx", BX);
  clexRegisterKind("cx", CX);
  clexRegisterKind("dx", DX);
  clexRegisterKind("ah", AH);
  clexRegisterKind("bh", BH);
  clexRegisterKind("ch", CH);
  clexRegisterKind("dh", DH);
  clexRegisterKind("al", AL);
  clexRegisterKind("bl", BL);
  clexRegisterKind("cl", CL);
  clexRegisterKind("dl", DL);
  clexRegisterKind("sp", SP);
  clexRegisterKind("dp", DP);
  clexRegisterKind("si", SI);
  clexRegisterKind("di", DI);
  clexRegisterKind("cs", CS);
  clexRegisterKind("ds", DS);
  clexRegisterKind("ss", SS);
  clexRegisterKind("es", ES);
  clexRegisterKind("ip", IP);
  clexRegisterKind("flags", FLAGS);
  clexRegisterKind("\\[", OSBRACE);
  clexRegisterKind("\\]", CSBRACE);
  clexRegisterKind(",", COMMA);

  char *buffer = 0;
  long length;
  FILE *f = fopen(argv[1], "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer)
      fread(buffer, 1, length, f);
    fclose (f);
  }

  if (!buffer) {
    fprintf(stderr, "Failed to read file\n");
    exit(1);
  }

  clexInit(buffer);

  char *result = assemble();
  printf("%s\n", result);
}
