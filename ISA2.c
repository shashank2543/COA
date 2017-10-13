/* 
	
	Taking input from output.txt(which is output from 1st C code)
	Execution of machine intruction and output in output_final.txt
	
	NOTE: only non-negative numbers can be input

*/

#include<stdio.h>
#include<string.h>

#define MOV convert("000000000000000000")
#define CMP convert("000000000000000001")
#define AND convert("000000000000000010")
#define MUL convert("000000000000000011")
#define DIV convert("000000000000000100")

#define MOVI convert("00000000000000000000")
#define MULT convert("00000000000000000001")
#define ADD convert("00000000000000000010")
#define SUB convert("00000000000000000011")
#define DIVI convert("00000000000000000100")

#define JMP convert("0000000000000000000000000")
#define JEQ convert("0000000000000000000000001")
#define INC convert("0000000000000000000000010")
#define DEC convert("0000000000000000000000011")
#define OUT convert("0000000000000000000000100")

//conversion function that returns integer value from the boolean string input here
long long convert(char a[]){

	long long i , ans = 0 , pow = 1;
	
	for( i = strlen(a) - 1 ;i >= 0; i--){
		if(a[i] == '1')
			ans += pow;
		pow *= 2;
	}
	return ans;
}

//in flag register ,first bit or LSB is for negative flag and 3rd bit is for zero as shown below
int flag_register = 0;		// 00000000000000000000000000000Z0N

//function for updating the flag registers
void set_flags(int value){			
	
	if(value < 0)			//set negative bit
		flag_register |= 1;						
	
	else{							//unset negative bit
		if(  1 & flag_register )
			flag_register ^= 1;
	}
					
	if(value == 0)			//set zero bit
		flag_register |= 4;						
	
	else{							//unset zero bit
		if(  4 & flag_register )
			flag_register ^= 4;
	}	
}

int main(){
	
	FILE *fp;	fp = freopen("output_final.txt", "w+", stdout);		//final answer in this file
	FILE *fp1;	fp1 = freopen("output.txt", "r", stdin);			//Using output file (machine instruction) of first C program as input file of this program
	
	int tot_ins = 0,tot_instt = 0 , cr = 0 ,j, k , regs[20] = {0} ;
	char ins[35][35];	//will store machine instruction in chararter form
	
	long long instt[70];	//will store machine instruction in INTEGER FORM**************************
	
	gets( ins[tot_ins] );
	instt[tot_instt++] = convert(ins[tot_ins]);
	
	//first run through machine instruction to store instructions in 2-D character array
	
	while( strcmp(ins[tot_ins++] , "00000000000000000000000000000000") ){	//taking binary instructions and converting to integer
		gets(ins[tot_ins]);
		instt[tot_instt++] = convert(ins[tot_ins]);		// binary instructions stored as integers***************
	}
	
	while( 1 ){
		
		printf("Instruction number: %d\n", cr + 1);
		
		long long strt = convert("11000000000000000000000000000000");	//Used TO MASK TO CHECK FIRST 2 BITS		
		
		strt &= instt[cr];
		
		strt >>= 30;
					
		if(strt == 0){		//00
			printf("HALTS\n");
			break;
		}

		if(strt == 1){					//01 FOR ONE ADDRESSING AND IMMEDIATE
			
			long long temp ,r1_int , imd_int;
			
			temp = convert("00111111111111111111000000000000");		//Used TO MASK TO STORE 18 BITS OPCODE
			
			temp &= instt[cr];
			
			temp >>= 12;
			
			r1_int = convert("00000000000000000000111110000000");		//Used TO MASK TO STORE REGISTER NUMBER
			
			r1_int &= instt[cr];
			
			r1_int >>= 7;
			
			imd_int = convert("00000000000000000000000001111111");			//Used TO MASK TO STORE IMMEDTIATE CONSTANT
			
			imd_int &= instt[cr];
			
			if( temp == MOV ){          //MOV
			
				printf("moving %d to register number %d\n",imd_int , r1_int);	
				regs[r1_int] = imd_int;
				
				set_flags( regs[r1_int] );
				printf("value in register %d : %d\n",r1_int, imd_int);
			}
			
			if( temp == CMP ){             //CMP
				
				printf("comparing %d to value in register number %d\n",imd_int , r1_int);	
				if(regs[r1_int] == imd_int)
					flag_register |= 4;						//set zero bit
				
				else if(regs[r1_int] < imd_int)
					flag_register |= 1;						//set negative bit
			}
			
			if( temp == AND ){              //AND
				
				printf("AND operation of %d with value in register number %d\n",imd_int , r1_int);	
				regs[r1_int] = regs[r1_int] & imd_int;
				
				set_flags( regs[r1_int] );
				printf("value in register %d : %d\n",r1_int , regs[r1_int]);
			}
			
			if( temp == MUL ){               //MUL
				
				printf("MULTIPLICATION operation of %d with value in register number %d\n",imd_int , r1_int);
				regs[r1_int] = regs[r1_int] * imd_int;
				
				set_flags( regs[r1_int] );
				printf("value in register %d : %d\n",r1_int , regs[r1_int]);		
			}

			if( temp == DIV ){               //DIV
				
				printf("DIVITION operation of %d with value in register number %d\n",imd_int , r1_int);
				regs[r1_int] = regs[r1_int] / imd_int;
				
				set_flags( regs[r1_int] );
				printf("value in register %d : %d\n",r1_int , regs[r1_int]);
			}
		}

		if(strt == 2){					//10 FOR TWO ADDRESSING 
			
			long long temp ,r1_int , r2_int;
			
			temp = convert("00111111111111111111110000000000");			//Used TO MASK TO STORE 20 BITS OPCODE
			
			temp &= instt[cr];
			
			temp >>= 10;
			
			r1_int = convert("00000000000000000000001111100000");		//Used TO MASK TO STORE REGISTER NUMBER
			
			r1_int &= instt[cr];
			
			r1_int >>= 5;
			
			r2_int = convert("00000000000000000000000000011111");		//Used TO MASK TO STORE REGISTER NUMBER
			
			r2_int &= instt[cr];
			

			if( temp == MOVI ){                    //MOV R2 TO R1
			
				printf("moving of value of register %d to value in register number %d\n",r2_int , r1_int);
				regs[r1_int] = regs[r2_int];
				
				set_flags( regs[r1_int] );
				printf("value in register %d : %d\nvalue in register %d : %d\n",r1_int , regs[r1_int] , r2_int , regs[r2_int]  );
			}

			if( temp == MULT ){					//MUL R1*R2 AND PUT RESULT INTO R1
			
				printf("MULTIPLICATION of value of register %d to value in register number %d and store\n",r2_int , r1_int,r1_int);
				regs[r1_int] = regs[r1_int] * regs[r2_int];
				
				set_flags( regs[r1_int] );
				printf("value in register %d : %d\nvalue in register %d : %d\n",r1_int , regs[r1_int] , r2_int , regs[r2_int]  );
			}

			if( temp == ADD ){					//ADD R1+R2 AND PUT RESULT INTO R1
			
				printf("ADDITION of value of register %d to value in register number %d and store\n",r2_int , r1_int,r1_int);
				regs[r1_int] = regs[r1_int] + regs[r2_int];
				
				set_flags( regs[r1_int] );
				printf("value in register %d : %d\nvalue in register %d : %d\n",r1_int , regs[r1_int] , r2_int , regs[r2_int]  );
			}

			if( temp == SUB ){					//SUB R1-R2 AND PUT RESULT INTO R1
			
				printf("SUBTRACTION of value of register %d from value in register number %d and store\n",r2_int , r1_int,r1_int);
				regs[r1_int] = regs[r1_int] - regs[r2_int];
				
				set_flags( regs[r1_int] );
				printf("value in register %d : %d\nvalue in register %d : %d\n",r1_int , regs[r1_int] , r2_int , regs[r2_int]  );
			}

			if( temp == DIVI ){					//DIV R1/R2 AND PUT RESULT INTO R1
			
				printf("DIVITION of value of register %d to value in register number %d and store\n",r2_int , r1_int,r1_int);
				regs[r1_int] = regs[r1_int] / regs[r2_int];
				
				set_flags( regs[r1_int] );
				printf("value in register %d : %d\nvalue in register %d : %d\n",r1_int , regs[r1_int] , r2_int , regs[r2_int]  );
			}

		}		
	
		if(strt == 3){					//11 FOR ONE ADDRESSING
			
			long long temp ,r1_int;
			
			temp = convert("00111111111111111111111111100000");		//Used TO MASK TO STORE 25 BITS OPCODE
			
			temp &= instt[cr];
			
			temp >>= 5;
			
			r1_int = convert("00000000000000000000000000011111");	//Used TO MASK TO STORE REGISTER NUMBER/LINE NUMBER
			
			r1_int &= instt[cr];
			
			if( temp == JMP ){		//JMP
			
				printf("jump to instruction number %d\n\n",r1_int + 1);	
				cr = r1_int;
				continue;
			}
			
			if( temp == JEQ ){		//JEQ
				
				if( flag_register & 4 ){		//checking zero bit
					printf("jump to instruction number %d as zero bit is set\n\n",r1_int + 1);
					cr = r1_int;
					continue;
				}		
			}
			
			if( temp == INC ){		//INC
			
				printf("increment register %d value by one\n",r1_int);		
				regs[r1_int] ++;
				
				set_flags( regs[r1_int] );
				printf("value in register %d : %d\n",r1_int , regs[r1_int] );
			}
			
			if( temp == DEC ){		//DEC
				
				printf("decrement register %d value by one\n",r1_int);
				regs[r1_int] --;
				
				set_flags( regs[r1_int] );
				printf("value in register %d : %d\n",r1_int , regs[r1_int] );
			}
			
			if( temp == OUT ){		//OUT
			
				printf("output register %d\n",r1_int);
				printf("%d\n\n",regs[r1_int]);
				cr++;
				continue;	
			}
			
		}
		
		printf("-Zero flag: ");
		
		if(  4 & flag_register )			//check for status of zero bit in status flag
			printf("Set\n");
		else
			printf("Unset\n");
			
		printf("-Negative flag: ");
		
		if(  1 & flag_register )			//check for status of negative bit in status flag
			printf("Set\n\n");
		else
			printf("Unset\n\n");
		
		cr++;
	}
		
	fclose(fp1);
	fclose(fp);
	return 0;
}
