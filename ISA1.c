#include<stdio.h>
#include<string.h>
/* 
	Assembly language to machine instruction conversion code
	
	NOTE: only negative numbers can be input

*/


void function(int n){					//function converting integer to 7 bit binary string and printing(for immediate constant conversion to machine instruction)
     int cnt=1,flag=0,i,ans=0,temp;
     for(i=n;i>0;){
         temp=i/2;
         flag=i-2*temp;
         ans+=flag*cnt;
         cnt*=10;
         i=i/2;
     }
     char str[7];
     int k=6;
     while(ans>0){
         temp=ans%10;
         str[k]=temp+'0';
         k--;
         ans/=10;
     }
     while(k>=0){
         str[k]='0';
         k--;
     }
     for(i=0;i<7;i++)
        printf("%c",str[i]);  			
}
	
void function11(int n){					//function converting integer to 5 bit binary string and printing(for instruction number conversion to machine instruction)
     int cnt=1,flag=0,i,ans=0,temp;
     for(i=n;i>0;){
         temp=i/2;
         flag=i-2*temp;
         ans+=flag*cnt;
         cnt*=10;
         i=i/2;
     }
     char str[5];
     int k=4;
     while(ans>0){
         temp=ans%10;
         str[k]=temp+'0';
         k--;
         ans/=10;
     }
     while(k>=0){
         str[k]='0';
         k--;
     }
     for(i=0;i<5;i++)
        printf("%c",str[i]);  
}

struct hash{		//HASH TABLES

/*this structure is for opcodes and their corresponding machine instructions*/
	char ind[100] , val[100];			
};

struct label{		//LABEL SYMBOL TABLE

/*this structure will contain the abel names and their corresponding line numbers */	
	char name[100];
	int in;
};

struct label lab[100];

struct hash opc[100] , regs[100];

int main(){
	
	FILE *fp;	fp = freopen("output.txt", "w+", stdout);				//all output given out to output.txt file
	FILE *fp1;	fp1 = freopen("input.txt", "r", stdin);					//all input taken from input.txt file
	
	/*values given to opcodes and their correponding machine instructions*/
	strcpy(opc[0].ind , "HLT"); 	strcpy(opc[0].val  , "00000000000000000000000000000000");
	strcpy(opc[1].ind , "MOV"); 	strcpy(opc[1].val  , "01000000000000000000");
	strcpy(opc[2].ind , "CMP"); 	strcpy(opc[2].val  , "01000000000000000001");
	strcpy(opc[3].ind , "AND"); 	strcpy(opc[3].val  , "01000000000000000010");
	strcpy(opc[4].ind , "MUL"); 	strcpy(opc[4].val  , "01000000000000000011");
	strcpy(opc[5].ind , "DIV"); 	strcpy(opc[5].val  , "01000000000000000100");
	
	strcpy(opc[8].ind , "MOVI"); 	strcpy(opc[8].val  , "1000000000000000000000");
	strcpy(opc[9].ind , "MULT"); 	strcpy(opc[9].val  , "1000000000000000000001");	
	strcpy(opc[10].ind , "ADD"); 	strcpy(opc[10].val , "1000000000000000000010");
	strcpy(opc[11].ind , "SUB"); 	strcpy(opc[11].val , "1000000000000000000011");
	strcpy(opc[12].ind , "DIVI"); 	strcpy(opc[12].val , "1000000000000000000100");
	
	strcpy(opc[13].ind , "JMP"); 	strcpy(opc[13].val , "110000000000000000000000000");
	strcpy(opc[14].ind , "JEQ"); 	strcpy(opc[14].val , "110000000000000000000000001");
	strcpy(opc[15].ind , "INC"); 	strcpy(opc[15].val , "110000000000000000000000010");
	strcpy(opc[16].ind , "DEC"); 	strcpy(opc[16].val , "110000000000000000000000011");
	strcpy(opc[7].ind , "OUT"); 	strcpy(opc[7].val  , "110000000000000000000000100");
	
	/*values given to registers and their correponding machine instructions*/
	strcpy(regs[0].ind , "R00"); 	strcpy(regs[0].val , "00000");
	strcpy(regs[1].ind , "R01"); 	strcpy(regs[1].val , "00001");
	strcpy(regs[2].ind , "R02"); 	strcpy(regs[2].val , "00010");
	strcpy(regs[3].ind , "R03"); 	strcpy(regs[3].val , "00011");
	strcpy(regs[4].ind , "R04"); 	strcpy(regs[4].val , "00100");
	strcpy(regs[5].ind , "R05"); 	strcpy(regs[5].val , "00101");
	strcpy(regs[6].ind , "R06"); 	strcpy(regs[6].val , "00110");
	strcpy(regs[7].ind , "R07"); 	strcpy(regs[7].val , "00111");
	strcpy(regs[8].ind , "R08"); 	strcpy(regs[8].val , "01000");
	strcpy(regs[9].ind , "R09"); 	strcpy(regs[9].val , "01001");
	strcpy(regs[10].ind , "R10"); 	strcpy(regs[10].val , "01010");
	strcpy(regs[11].ind , "R11"); 	strcpy(regs[11].val , "01011");
	strcpy(regs[12].ind , "R12"); 	strcpy(regs[12].val , "01100");
	strcpy(regs[13].ind , "R13"); 	strcpy(regs[13].val , "01101");
	strcpy(regs[14].ind , "R14"); 	strcpy(regs[14].val , "01110");
	strcpy(regs[15].ind , "R15"); 	strcpy(regs[15].val , "01111");
	
	int sp=0, i,  tot_opc = 17, tot_reg = 16, j , cnt = -1 , tot_lab = 0;
	char ins[100];
	
	/* 
		Two passes are done through the assembly code .
		1st one makes the hash table for labels
		which are used in 2nd run in JMP and JEQ opcodes to give them line number corresponding 
		to their own labels.		
	*/
	 
	while(1){			/* 1st run through assembly code to take make the hash table for labels */
		
		cnt++;		//variable maintained to count instruction number
		gets(ins);
		sp = 0;
		
		for(i = 0; i < strlen(ins); i++){	/*counting number of spaces in instruction*/
			if(ins[i] == ' ')
				sp++;
		}
	
		if(sp == 0){		//zero space case -> label and Halt case
			
			if(ins[strlen(ins) - 1] == ':'){	//label detected by a ':' at the end	
				ins[strlen(ins) - 1] = '\0';
				strcpy(lab[tot_lab].name , ins);	//updating label array by adding a new label name		
				lab[tot_lab++].in = cnt;			//and the line number it points to
				cnt--;
				continue;
			}
			
			if( !strcmp(ins , "HLT")){		//Halt detected
				break;
			}
		}
	}
	
	fclose(fp1);
	fp1 = freopen("input.txt", "r", stdin);	
	
	
	while(1){			/* 2nd run through assembly code */
		
		gets(ins);
		sp = 0;
	
		for(i = 0; i < strlen(ins); i++){	/*counting number of spaces in instruction*/
			if(ins[i] == ' ')
				sp++;
		}
	
		if(sp == 0){		//zero space case -> label and Halt case
			
			if( !strcmp(ins , "HLT")){		//Halt detected
				printf("%s" , opc[0].val);
				break;
			}
			else continue;
		}
		
		if(sp == 1){		//one space case	-> jump or increment/decrement cases
			
			char tmpp[100];
			i = 0; j = 0;		
			
			while(ins[i] != ' '){
				tmpp[i] = ins[i];
				i++;	
			}tmpp[i] = '\0';		//opcode stored in tmpp		
			
			for( i = 0; i < tot_opc ; i++){
				if( !strcmp(opc[i].ind , tmpp )){
					printf("%s", opc[i].val);				//finding opcode in the opcode array and printing
					break;
				}				
			}
			char temp[100];
			strcpy(temp , tmpp);
			
			i = strlen(tmpp) + 1;
			j = 0;
			while(i != strlen(ins))
				tmpp[j++] = ins[i++];
			tmpp[j] = '\0';					//tmpp consists of rest of the instruction(either register name or label name)
						
			if( !strcmp(temp , "JEQ") || !strcmp(temp , "JMP")){
				
				for( i = 0; i < tot_lab; i++){
					if( !strcmp(lab[i].name , tmpp )){		//label name comparison and printing instruction number corresponding to label
						function11(lab[i].in);				//converting to 5 bit
						printf("\n");
						break;
					}
				}
				continue;
			}
			
			for( i = 0; i < tot_reg ; i++){		///INC /DEC case
				if( !strcmp(regs[i].ind , tmpp )){
					printf("%s", regs[i].val);				//finding register and printing
					break;
				}				
			}			
		}
		
		if(sp == 2){				//2 space case		-> 2 register and 1 register 1 immediate constant case
			
			char tmpp[100];
			i = 0; j = 0;		
			
			while(ins[i] != ' '){
				tmpp[i] = ins[i];
				i++;	
			}tmpp[i] = '\0';				//opcode stored in tmpp
			
			for( i = 0; i < tot_opc ; i++){
				if( !strcmp(opc[i].ind , tmpp )){
					printf("%s", opc[i].val);				//finding opcode in the opcode array and printing
					break;
				}		
			}
			
			i = strlen(tmpp) + 1;
			int opc_len = i - 1;
			j = 0;
			
			while(i != opc_len + 4)
				tmpp[j++] = ins[i++];
			tmpp[j] = '\0';
	
			for( i = 0; i < tot_reg ; i++){
				if( !strcmp(regs[i].ind , tmpp )){
					printf("%s", regs[i].val);				//finding register and printing
					break;
				}			
			}
			
			if(ins[opc_len + 5] == '#'){		//immediate constant detected ( 1 register 1 immediate constant )
				
				j = 0;
				i = opc_len + 6;
				
				while(i != strlen(ins))
					tmpp[j++] = ins[i++];
				tmpp[j] = '\0';					//getting string of immediate constant in tmpp
				
				int imm = atoi(tmpp);						//converting string to integer
				function(imm);								//converting to 7 bit	
			}
			
			else{								//2 registers	
				
				j = 0; 
				i = opc_len + 5;
				
				while(i != strlen(ins))
					tmpp[j++] = ins[i++];
				tmpp[j] = '\0';
				
				for( i = 0; i < tot_reg ; i++){
					if( !strcmp(regs[i].ind , tmpp )){
						printf("%s", regs[i].val);			//finding register and printing
						break;
					}				
				}
			}
			
		}
		
		printf("\n");
	}
	
	fclose(fp);
	fclose(fp1);
	return 0;
}
