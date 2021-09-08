#ifndef __OPCODE__
#define __OPCODE__
/* instruction types */

/* IFormat */
#define _ADDIU 0
#define _ANDI 1
#define _BEQ 2
#define _BNE 3
#define _LUI 4
#define _LW 5
#define _ORI 6
#define _SLTIU 7
#define _SW 8

/* JFormat */
#define _J 9
#define _JAL 10
#define _RFE 11

#define RTYPE 12

/* RFormat */
#define _ADDU 0
#define _AND 1
#define _JR 2
#define _OR 3
#define _SLTU 4
#define _SLL 5
#define _SRL 6
#define _SUBU 7
#define _XOR 8
#define _SYSCALL 9
#define _EXIT 10

/* end instruction types */
#define INT_SYSCALL 1
#define INT_BUSERR 2
#define INT_INSERR 3
#define INT_SEGFAULT 4
#define INT_RFE 5

#endif
