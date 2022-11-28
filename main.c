#include "clex/clex.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

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
  BYTE,
  WORD,
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
  BP,
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

typedef enum RegisterEncoding {
  ax = 0b000,
  cx = 0b001,
  dx = 0b010,
  bx = 0b011,
  sp = 0b100,
  bp = 0b101,
  si = 0b011,
  di = 0b111,
} RegisterEncoding;

RegisterEncoding get_16bit_register_encoding(TokenKind reg) {
  switch(reg) {
    case AX:
      return ax;
    case CX:
      return cx;
    case DX:
      return dx;
    case BX:
      return bx;
    case SP:
      return sp;
    case BP:
      return bp;
    case SI:
      return si;
    case DI:
      return di;
  }
}

typedef enum RegisterEncoding8 {
  al = 0b000,
  cl = 0b001,
  dl = 0b010,
  bl = 0b011,
  ah = 0b100,
  ch = 0b101,
  dh = 0b110,
  bh = 0b111,
} RegisterEncoding8;

typedef enum SegmentOverridePrefix {
  so_es = 0b00,
  so_cs = 0b01,
  so_ss = 0b10,
  so_ds = 0b11,
} SegmentOverridePrefix;

typedef enum Mod {
  MOD_REGISTER_INDIRECT = 0b00,
  MOD_ONE_BYTE_DISPLACEMENT = 0b01,
  MOD_TWO_BYTE_DISPLACEMENT = 0b10,
  MOD_REGISTER = 0b11,
} Mod;

typedef enum RM {
  bx_si = 0b000,
  bx_di = 0b001,
  bp_si = 0b010,
  bp_di = 0b011,
  na_si = 0b100,
  na_di = 0b101,
  bp_na = 0b110,
  bx_na = 0b111,
} RM;

RM get_rm_encoding(TokenKind reg1, TokenKind reg2) {
  if (reg1 == BX && reg2 == SI)
    return bx_si;
  if (reg1 == BX && reg2 == DI)
    return bx_di;
  if (reg1 == BP && reg2 == SI)
    return bp_si;
  if (reg1 == BP && reg2 == DI)
    return bp_di;
  if (reg1 == -1 && reg2 == SI)
    return na_si;
  if (reg1 == -1 && reg2 == DI)
    return na_di;
  if (reg1 == BP && reg2 == -1)
    return bp_na;
  if (reg1 == BX && reg2 == -1) {
    return bx_na;
  }
}

typedef struct __attribute__((__packed__)) ModRM {
  RM rm : 3;
  uint8_t mid : 3;
  Mod mod : 2;
} ModRM;

ModRM *make_modrm(uint8_t value) {
  ModRM *result = calloc(1, sizeof(ModRM));
  result->mod = (value & 0b11000000) >> 6;
  result->mid = (value & 0b00111000) >> 3;
  result->rm = value & 0b00000111;
  return result;
}

bool is_16bit_gp_register(TokenKind kind) {
  if (kind == AX || kind == BX || kind == CX || kind == DX)
    return true;
  return false;
}

bool is_16bit_rm_register(TokenKind kind) {
  if (kind == BX || kind == BP || kind == SI || kind == DI)
    return true;
  return false;
}

char *assemble(void) {
  char *result = calloc(10000, sizeof(char));
  Token token;
  while ((token = clex()).kind != -1) {
    switch(token.kind) {
      case AAA:
        sprintf(result + strlen(result), "%c", 0x37);
      case MOV:
        token = clex();
        if (is_16bit_gp_register(token.kind)) {
          TokenKind firstArg = token.kind;
          if (clex().kind == COMMA) {
            if (clex().kind == OSBRACE) {
              token = clex();
              if (is_16bit_rm_register(token.kind)) {
                TokenKind secondArg = token.kind;
                if (clex().kind == CSBRACE) {
                  ModRM *modrm = make_modrm(0b00000000);
                  modrm->mod = 0b00;
                  modrm->mid = get_16bit_register_encoding(firstArg);
                  modrm->rm = get_rm_encoding(secondArg == BP || secondArg == BX ? secondArg : -1, secondArg == SI || secondArg == DI ? secondArg : -1);
                  sprintf(result + strlen(result), "%c%c", 0x8B, (*(unsigned char *)modrm));
                }
              }
            }
          }
        }
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
  clexRegisterKind("byte", BYTE);
  clexRegisterKind("word", WORD);
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
  clexRegisterKind("bp", BP);
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
  printf("%s", result);
}
