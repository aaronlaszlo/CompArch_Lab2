/***************************************************************/
/*                                                             */
/*   ARMv4-32 Instruction Level Simulator                      */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#ifndef _SIM_ISA_H_
#define _SIM_ISA_H_
#define N_CUR ( (CURRENT_STATE.CPSR>>31) & 0x00000001 )
#define Z_CUR ( (CURRENT_STATE.CPSR>>30) & 0x00000001 )
#define C_CUR ( (CURRENT_STATE.CPSR>>29) & 0x00000001 )
#define V_CUR ( (CURRENT_STATE.CPSR>>28) & 0x00000001 )
#define N_NXT ( (NEXT_STATE.CPSR>>31) & 0x00000001 )
#define Z_NXT ( (NEXT_STATE.CPSR>>30) & 0x00000001 )
#define C_NXT ( (NEXT_STATE.CPSR>>29) & 0x00000001 )
#define V_NXT ( (NEXT_STATE.CPSR>>28) & 0x00000001 )

#define N_N 0x80000000
#define Z_N 0x40000000
#define C_N 0x20000000
#define V_N 0x10000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int ADD (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
    if (cur > 1)
      NEXT_STATE.CPSR |= V_N;
  }	


  return 0;

}

int ADC (int Rd, int Rn, int Operand2, int I, int S, int CC){
  
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
    if (cur > 1)
      NEXT_STATE.CPSR |= V_N;
  }	

  return 0;
}
int AND (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] & (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
  }	

  return 0;
}

int ASR (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0)    //bit4 is a shifted register
      /*if(sh=10){
        LSB = (CURRENT_STATE.REGS[Rm] >> (32));
        //LSB = (CURRENT_STATE.REGS[Rm] & 0x0000FFFF);
        
      } */
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] >> 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5))); 
	  break;

      }     
    else    
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] >> 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }    

  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] >> (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
  }	
  return 0;
}



int BIC (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      !((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      !((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] & !(Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
  }	

  return 0;
}



int CMN (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
    if (cur > 1)
      NEXT_STATE.CPSR |= V_N;
  }	

  return 0;
}
int CMP (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] - (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
    if (cur > 1)
      NEXT_STATE.CPSR |= V_N;
  }	

  return 0;
}
int EOR (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;        
    int shamt5 = (Operand2 & 0x00000F80) >> 7;    
    int bit4 = (Operand2 & 0x00000010) >> 4;        
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] ^
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] ^
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] ^ 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] ^
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] ^
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] ^
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] ^ (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
  }	
  
  return 0;
}

int LSL (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0)    //bit4 is a shifted register
      /*if(sh=10){
        LSB = (CURRENT_STATE.REGS[Rm] >> (32));
        //LSB = (CURRENT_STATE.REGS[Rm] & 0x0000FFFF);
        
      } */
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] << 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] << 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] << 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    break;
      case 3: cur = CURRENT_STATE.REGS[Rn] << 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5))); 
	  break;

      }     
    else    
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] << 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] << 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] << 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] << 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }    

  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] << (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
  }	
  return 0;
}

int LSR (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0)    //bit4 is a shifted register
      /*if(sh=10){
        LSB = (CURRENT_STATE.REGS[Rm] >> (32));
        //LSB = (CURRENT_STATE.REGS[Rm] & 0x0000FFFF);
        
      } */
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] >> 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5))); 
	  break;

      }     
    else    
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] >> 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }    

  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] >> (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
  }	
  return 0;
}

int MLA (char* i_);

int MOV (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;  
    int bit4 = (Operand2 & 0x00000010) >> 4; 
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur =  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur =  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
  }	
  
  return 0;
}
int MUL (char* i_);

int MVN (int Rd, int Rn, int Operand2, int I, int S, int CC){
    int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;  
    int bit4 = (Operand2 & 0x00000010) >> 4; 
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur =  !(CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur =  !(CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = !(CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = !((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = !(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = !(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = !(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = !((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = !(Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
  }	
  
  return 0;
}

int ORR (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;        
    int shamt5 = (Operand2 & 0x00000F80) >> 7;    
    int bit4 = (Operand2 & 0x00000010) >> 4;        
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] | 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] |
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] | 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] | (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
  }	
  
  return 0;
}

int ROR (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  int a = 0;
  int b = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    int msb = Rm & 0x10000000;
    
    if (bit4 == 0) 
      switch (sh) {
      case 0:
        a = CURRENT_STATE.REGS[Rn];
        b = CURRENT_STATE.REGS[Rm] << shamt5;
        cur = a;
	      break;
      case 1:
        a = CURRENT_STATE.REGS[Rn];
        b = CURRENT_STATE.REGS[Rm] >> shamt5;
        cur = a;
	      break;
      case 2: 
        a = CURRENT_STATE.REGS[Rn];
        b = CURRENT_STATE.REGS[Rm] >> shamt5;
        int msb = CURRENT_STATE.REGS[Rm] & 0x10000000;
       
        cur = a;
    	  break;
      case 3:
	      a = CURRENT_STATE.REGS[Rn];
        b = (CURRENT_STATE.REGS[Rm] >> shamt5) | (CURRENT_STATE.REGS[Rm] << (32 - shamt5));
        cur = a;
    	  break;
      }     
    else
      switch (sh) {
      case 0:
	      a = CURRENT_STATE.REGS[Rn];
        b = CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs];
        cur = a;
    	  break;
      case 1:
	      a = CURRENT_STATE.REGS[Rn];
        b = CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs];
        cur = a;
    	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	      a = CURRENT_STATE.REGS[Rn];
        b = CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs];
        int msb = CURRENT_STATE.REGS[Rm] & 0x10000000;
       
        cur = a;
    	  break;
      case 3: 
        a = CURRENT_STATE.REGS[Rn];
        b = (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) | (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs]));
        cur = a;
    	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    a = CURRENT_STATE.REGS[Rn];
    cur = a;
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur > 0xFFFFFFFF)
      NEXT_STATE.CPSR |= C_N;
  }	
  return 0;
}


int SBC (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] - (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
    if (cur > 1)
      NEXT_STATE.CPSR |= V_N;
  }	

  return 0;
}


int SUB (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] - (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
    if (cur > 1)
      NEXT_STATE.CPSR |= V_N;
  }	

  return 0;
}
int TEQ (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;        
    int shamt5 = (Operand2 & 0x00000F80) >> 7;    
    int bit4 = (Operand2 & 0x00000010) >> 4;        
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] || 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] || 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] || 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] || 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] || 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] || 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] || 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] || 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] || (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
  }	
  
  return 0;
}
int TST (int Rd, int Rn, int Operand2, int I, int S, int CC){
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] &&
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] && 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] && 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] && 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] && 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] && 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] && 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] && 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] && (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
    if (cur == 1)
      NEXT_STATE.CPSR |= C_N;
  }	

  return 0;
}

//branch instructions 
int B (int imm24) {
  NEXT_STATE.PC = (CURRENT_STATE.PC + 8) + (imm24 << 2);
  return 0;
}
int BL (int imm24) {
  B(imm24);
  NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (imm24 << 2);
  NEXT_STATE.REGS[14] = (CURRENT_STATE.PC + 8) - 4;
  return 0;
}


//multiplication instuctions not listed in instructions

//memory instructions
int STR (int Rd, int Rn, int Operand2, int I){
  int cur = 0;
  int address = 0;
  int src2 = 0;
  if (~I == 0){    
    src2 = Operand2;
  } else {      
    int shamt5 = (Operand2 & 0x00000F80) >> 7; 
    int sh = (Operand2 & 0x00000060) >> 5;
    int bit4 = (Operand2 & 0x00000010) >> 4; 
    int Rm = Operand2 & 0x0000000F;
    switch (sh) {
      case 0: 
        src2 = CURRENT_STATE.REGS[Rm] << shamt5;
	      break;
      case 1: 
        src2 = CURRENT_STATE.REGS[Rm] >> shamt5;
	      break;
      case 2: 
        if (CURRENT_STATE.REGS[Rm] < 0 && shamt5 > 0) {
          src2 = CURRENT_STATE.REGS[Rm] >> shamt5 | ~(~0U >> shamt5);
        } else {
          src2 = CURRENT_STATE.REGS[Rm] >> shamt5;
        }
    	  break;
      case 3:
        src2 = (CURRENT_STATE.REGS[Rm] >> shamt5) | (CURRENT_STATE.REGS[Rm] << (32 - shamt5));
    	  break;
      }     
  }
  address = CURRENT_STATE.REGS[Rn] + src2;
  mem_write_32(address, CURRENT_STATE.REGS[Rd]);
  return 0;
}

int LDR (int Rd, int Rn, int Operand2, int I){
  int cur = 0;
  int address = 0;
  int src2 = 0;
  if (~I == 0){    
    src2 = Operand2;
  } else {       
    int shamt5 = (Operand2 & 0x00000F80) >> 7; 
    int sh = (Operand2 & 0x00000060) >> 5;
    int bit4 = (Operand2 & 0x00000010) >> 4; 
    int Rm = Operand2 & 0x0000000F;
    switch (sh) {
      case 0: 
        src2 = CURRENT_STATE.REGS[Rm] << shamt5;
	      break;
      case 1: 
        src2 = CURRENT_STATE.REGS[Rm] >> shamt5;
	      break;
      case 2: 
        if (CURRENT_STATE.REGS[Rm] < 0 && shamt5 > 0) {
          src2 = CURRENT_STATE.REGS[Rm] >> shamt5 | ~(~0U >> shamt5);
        } else {
          src2 = CURRENT_STATE.REGS[Rm] >> shamt5;
        }
    	  break;
      case 3: 
        src2 = (CURRENT_STATE.REGS[Rm] >> shamt5) | (CURRENT_STATE.REGS[Rm] << (32 - shamt5));
    	  break;
      }     
  }
  address = CURRENT_STATE.REGS[Rn] + src2;
  CURRENT_STATE.REGS[Rd] = mem_read_32(address);
  return 0;
}

int STRB (int Rd, int Rn, int Operand2, int I){
  int cur = 0;
  int address = 0;
  int src2 = 0;
  if (~I == 0){   
    src2 = Operand2;
  } else {       
    int shamt5 = (Operand2 & 0x00000F80) >> 7; 
    int sh = (Operand2 & 0x00000060) >> 5;
    int bit4 = (Operand2 & 0x00000010) >> 4; 
    int Rm = Operand2 & 0x0000000F;
    switch (sh) {
      case 0: 
        src2 = CURRENT_STATE.REGS[Rm] << shamt5;
	      break;
      case 1: 
        src2 = CURRENT_STATE.REGS[Rm] >> shamt5;
	      break;
      case 2: 
        if (CURRENT_STATE.REGS[Rm] < 0 && shamt5 > 0) {
          src2 = CURRENT_STATE.REGS[Rm] >> shamt5 | ~(~0U >> shamt5);
        } else {
          src2 = CURRENT_STATE.REGS[Rm] >> shamt5;
        }
    	  break;
      case 3: 
        src2 = (CURRENT_STATE.REGS[Rm] >> shamt5) | (CURRENT_STATE.REGS[Rm] << (32 - shamt5));
    	  break;
      }     
  }
  address = CURRENT_STATE.REGS[Rn] + src2;
  mem_write_32(address, CURRENT_STATE.REGS[Rd] & 0xFF);
  return 0;
}

int LDRB (int Rd, int Rn, int Operand2, int I){
  int cur = 0;
  int address = 0;
  int src2 = 0;
  if (~I == 0){    
    src2 = Operand2;
  } else {        
    int shamt5 = (Operand2 & 0x00000F80) >> 7; 
    int sh = (Operand2 & 0x00000060) >> 5;
    int bit4 = (Operand2 & 0x00000010) >> 4; 
    int Rm = Operand2 & 0x0000000F;
    switch (sh) {
      case 0: 
        src2 = CURRENT_STATE.REGS[Rm] << shamt5;
	      break;
      case 1: 
        src2 = CURRENT_STATE.REGS[Rm] >> shamt5;
	      break;
      case 2: 
        if (CURRENT_STATE.REGS[Rm] < 0 && shamt5 > 0) {
          src2 = CURRENT_STATE.REGS[Rm] >> shamt5 | ~(~0U >> shamt5);
        } else {
          src2 = CURRENT_STATE.REGS[Rm] >> shamt5;
        }
    	  break;
      case 3: 
        src2 = (CURRENT_STATE.REGS[Rm] >> shamt5) | (CURRENT_STATE.REGS[Rm] << (32 - shamt5));
    	  break;
      }     
  }
  address = CURRENT_STATE.REGS[Rn] + src2;
  CURRENT_STATE.REGS[Rd] = mem_read_32(address) & 0xFF;
  return 0;
}

int SWI (char* i_){return 0;}

#endif
