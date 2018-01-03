#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include <string>
#include <iostream>
using namespace std;
using namespace std;
int regs[32] = { 0 };
unsigned int pc = 0x0;




void convertMIPStoRISC(int instWordMIPS, int &instWord) {

	
	unsigned int opcode = unsigned(instWordMIPS & 0xFC000000) >> 26;
	
	unsigned int funct = unsigned(instWordMIPS & 0x3F);
	

	unsigned int rd = unsigned(instWordMIPS & 0xF800) >> 11;
	unsigned int rs1 = unsigned(instWordMIPS & 0x1F0000) >> 16;
	unsigned int rs2 = unsigned(instWordMIPS & 0x3E00000) >> 21;


	unsigned int imm = (instWordMIPS & 0xFFFF);
	//imm = imm | (((imm >> 15) ? 0xFFFF8000 : 0x0));
	unsigned int opcodeRISC;
	unsigned int funct3RISC;
	unsigned int funct7;

	unsigned int address = unsigned(instWordMIPS & 0x3FFFFFF);
	unsigned int immRISC;
	unsigned int imm11_5;
	unsigned int imm12;
	unsigned int imm4_0;
	unsigned int imm4_1;
	unsigned int imm10_5;
	unsigned int imm11;
	unsigned int imm20;
	unsigned int imm10_1;
	unsigned int imm19_12;
	unsigned int imm31_12;

	if (opcode == 0x0) {		// R Instructions

		switch (funct) {

		case 12:
			instWord = 0x73000000;
			break;

		case 32:
			//add
			rd = rd << 7;
			rs1 = rs1 << 15;
			rs2 = rs2 << 20;
			opcodeRISC = 51;
			funct3RISC = 0 << 12;
			funct7 = 0 << 25;
			instWord = rd | rs1 | rs2 | opcodeRISC | funct3RISC | funct7;
			break;
		case 33:
			//addu
			cout << "This instruction doesn't have an equivilant in RISC-V";
			break;

		case 36:
			//and
			rd = rd << 7;
			rs1 = rs1 << 15;
			rs2 = rs2 << 20;
			opcodeRISC = 51;
			funct3RISC = 7 << 12;
			funct7 = 0 << 25;
			instWord = rd | rs1 | rs2 | opcodeRISC | funct3RISC | funct7;
			break;

		case 8:
			//jr
			//jalr
			//opcode 0x67, funct3=0
			opcodeRISC = 0x67;
			funct3RISC = 0;


			break;
		case 39:
			//nor
			cout << "This instruction doesn't have an equivilant in RISC-V";
			break;
		case 37:
			//or
			rd = rd << 7;
			rs1 = rs1 << 15;
			rs2 = rs2 << 20;
			opcodeRISC = 51;
			funct3RISC = 6 << 12;
			funct7 = 0 << 25;
			instWord = rd | rs1 | rs2 | opcodeRISC | funct3RISC | funct7;
			break;

		case 42:
			//slt
			rd = rd << 7;
			rs1 = rs1 << 15;
			rs2 = rs2 << 20;
			opcodeRISC = 51;
			funct3RISC = 2 << 12;
			funct7 = 0 << 25;
			instWord = rd | rs1 | rs2 | opcodeRISC | funct3RISC | funct7;
			break;

		case 43:
			//sltu
			rd = rd << 7;
			rs1 = rs1 << 15;
			rs2 = rs2 << 20;
			opcodeRISC = 51;
			funct3RISC = 3 << 12;
			funct7 = 0 << 25;
			instWord = rd | rs1 | rs2 | opcodeRISC | funct3RISC | funct7;
			break;

		case 0:
			//sll
			rd = rd << 7;
			rs1 = rs1 << 15;
			rs2 = rs2 << 20;
			opcodeRISC = 51;
			funct3RISC = 1 << 12;
			funct7 = 0 << 25;
			instWord = rd | rs1 | rs2 | opcodeRISC | funct3RISC | funct7;
			break;

		case 2:
			//srl
			rd = rd << 7;
			rs1 = rs1 << 15;
			rs2 = rs2 << 20;
			opcodeRISC = 51;
			funct3RISC = 5 << 12;
			funct7 = 0 << 25;
			instWord = rd | rs1 | rs2 | opcodeRISC | funct3RISC | funct7;
			break;

		case 34:
			//sub
			rd = rd << 7;
			rs1 = rs1 << 15;
			rs2 = rs2 << 20;
			opcodeRISC = 51;
			funct3RISC = 0 << 12;
			funct7 = 32 << 25;
			instWord = rd | rs1 | rs2 | opcodeRISC | funct3RISC | funct7;
			break;

		case 35:
			//subu
			cout << "This instruction doesn't have an equivilant in RISC-V";
			break;
		}

	}


	else if (opcode == 0x8) {//addi
							 //opcode: 0010011,0x13 //funct3: 000, 0x0
							 //imm[11:0]+rs1+funct3+rd+opcode
		opcodeRISC = 19;
		funct3RISC = 0;
		immRISC = imm & 0xFFF; //?
		immRISC = immRISC << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		rs2 = rs2 << 7;
		instWord = immRISC | rs1 | funct3RISC | rs2 | opcodeRISC;
		
	}


	else if (opcode == 0x9) {//addiu??????????????????????????????
							 //opcode: 0010011,0x13 //funct3: 000, 0x0
							 //imm[11:0]+rs1+funct3+rd+opcode
		opcodeRISC = 0x13;
		funct3RISC = 0;
		immRISC = unsigned(imm) & 0xFFF; //?
		immRISC = immRISC << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		rs2 = rs2 << 7;
		instWord = immRISC | rs1 | funct3RISC | rs2 | opcodeRISC;

	}

	else if (opcode == 0xc) {//andi
							 //opcode: 0010011,0x13 //funct3: 111, 0x7
							 //imm[11:0]+rs1+funct3+rd+opcode
		opcodeRISC = 0x13;
		funct3RISC = 7;
		immRISC = imm & 0xFFF; //?
		immRISC = immRISC << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		rs2 = rs2 << 7;
		instWord = immRISC | rs1 | funct3RISC | rs2 | opcodeRISC;
		
	}

	else if (opcode == 0x4) {//beq
							 //opcode: 0x63 //funct3: 000
							 //imm[12]+imm[10:5]+rs2+rs1+funct3+imm[4:1]+imm[11]+opcode
		opcodeRISC = 0x63;
		funct3RISC = 0;
		immRISC = imm & 0xFFF; //?
		imm12 = (immRISC & 0x1000) << 20;
		imm10_5 = (immRISC & 0x7E0) << 25;
		rs2 = rs2 << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		imm4_1 = (immRISC & 0x1E) << 8;
		imm11 = (immRISC & 0x800) << 7;
		instWord = imm12 | imm10_5 | rs2 | rs1 | funct3RISC | imm4_1 | imm11 | opcodeRISC;
	}

	else if (opcode == 0x5) {//bne
							 //opcode: 1100011,0x63 //funct3: 001, 0x1
							 //imm[12]+imm[10:5]+rs2+rs1+funct3+imm[4:1]+imm[11]+opcode
		opcodeRISC = 0x63;
		funct3RISC = 1;
		immRISC = imm & 0xFFF; //?
		imm12 = (immRISC & 0x1000) << 20;
		imm10_5 = (immRISC & 0x7E0) << 25;
		rs2 = rs2 << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		imm4_1 = (immRISC & 0x1E) << 8;
		imm11 = (immRISC & 0x800) << 7;
		instWord = imm12 | imm10_5 | rs2 | rs1 | funct3RISC | imm4_1 | imm11 | opcodeRISC;
	}


	else if (opcode == 0x24) {//lbu
							  //opcode: 0000011,0x3 //funct3: 100, 0x4
							  //imm[11:0]+rs1+funct3+rd+opcode
		opcodeRISC = 0x3;
		funct3RISC = 4;
		immRISC = imm & 0xFFF; //?
		immRISC = immRISC << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		rs2 = rs2 << 7;
		instWord = immRISC | rs1 | funct3RISC | rs2 | opcodeRISC;
	}

	else if (opcode == 0x25) {//lhu
							  //opcode: 0000011,0x3 //funct3: 101, 0x5
							  //imm[11:0]+rs1+funct3+rd+opcode
		opcodeRISC = 0x3;
		funct3RISC = 5;
		immRISC = imm & 0xFFF; //?
		immRISC = immRISC << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		rs2 = rs2 << 7;
		instWord = immRISC | rs1 | funct3RISC | rs2 | opcodeRISC;

	}

	else if (opcode == 0xf) {//lui ???????
							 //opcode: 0110111,0x37 
		opcodeRISC = 0x37;
		imm31_12 = (imm << 16) & 0xFFFFF000;
		rs2 = rs2 << 7;
		instWord = opcodeRISC | imm31_12 | rs2;

	}

	else if (opcode == 0x23) {//lw
							  //opcode: 0000011,0x3 //funct3: 010, 0x2
							  //imm[11:0]+rs1+funct3+rd+opcode
		opcodeRISC = 0x3;
		funct3RISC = 2;
		immRISC = imm & 0xFFF; //?
		immRISC = immRISC << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		rs2 = rs2 << 7;
		instWord = immRISC | rs1 | funct3RISC | rs2 | opcodeRISC;
	}

	else if (opcode == 0xd) {//ori
							 //opcode: 0010011,0x13 //funct3: 110, 0x6
							 //imm[11:0]+rs1+funct3+rd+opcode
		opcodeRISC = 0x13;
		funct3RISC = 6;
		immRISC = imm & 0xFFF; //?
		immRISC = immRISC << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		rs2 = rs2 << 7;
		instWord = immRISC | rs1 | funct3RISC | rs2 | opcodeRISC;
	}

	else if (opcode == 0xa) {//slti
							 //opcode: 0010011,0x13 //funct3: 010, 0x2
							 //imm[11:0]+rs1+funct3+rd+opcode
		opcodeRISC = 0x13;
		funct3RISC = 2;
		immRISC = imm & 0xFFF; //?
		immRISC = immRISC << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		rs2 = rs2 << 7;
		instWord = immRISC | rs1 | funct3RISC | rs2 | opcodeRISC;
	}

	else if (opcode == 0xb) {//sltiu
							 //opcode: 0010011,0x13 //funct3: 011, 0x3
							 //imm[11:0]+rs1+funct3+rd+opcode
		opcodeRISC = 0x13;
		funct3RISC = 3;
		immRISC = imm & 0xFFF; //?
		immRISC = unsigned(immRISC) << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		rs2 = rs2 << 7;
		instWord = immRISC | rs1 | funct3RISC | rs2 | opcodeRISC;
	}

	else if (opcode == 0x28) {//sb
							  //opcode: 0100011,0x23 //funct3: 000, 0x0
							  //imm[11:5]+rs2+rs1+funct3+imm[4:0]+opcode
		opcodeRISC = 0x23;
		funct3RISC = 0;
		immRISC = imm & 0xFFF; //?
		imm11_5 = (immRISC & 0xFE0) << 25;
		rs2 = rs2 << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		imm4_0 = (immRISC & 0x1F) << 8; 

		instWord = imm11_5 | rs2 | rs1 | funct3RISC | imm4_0 | opcodeRISC;
	}

	else if (opcode == 0x29) {//sh
							  //opcode: 0100011,0x23 //funct3: 001, 0x1
							  //imm[11:5]+rs2+rs1+funct3+imm[4:0]+opcode
		opcodeRISC = 0x23;
		funct3RISC = 1;
		immRISC = imm & 0xFFF; //?
		imm11_5 = (immRISC & 0xFE0) << 25;
		rs2 = rs2 << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		imm4_0 = (immRISC & 0x1F) << 8;

		instWord = imm11_5 | rs2 | rs1 | funct3RISC | imm4_0 | opcodeRISC;
	}

	else if (opcode == 0x2b) {//sw
							  //opcode: 0100011,0x23 //funct3: 010, 0x2
							  //imm[11:5]+rs2+rs1+funct3+imm[4:0]+opcode
		opcodeRISC = 0x23;
		funct3RISC = 2;
		immRISC = imm & 0xFFF; //?
		imm11_5 = (immRISC & 0xFE0) << 25;
		rs2 = rs2 << 20;
		rs1 = rs1 << 15;
		funct3RISC = funct3RISC << 12;
		imm4_0 = (immRISC & 0x1F) << 8;

		instWord = imm11_5 | rs2 | rs1 | funct3RISC | imm4_0 | opcodeRISC;

	}

	else if (opcode == 0x2) {//j????????????????
			//opcode: 0x6f 
		opcodeRISC = 0x6f;
		rd = 0;
		rd = rd << 7;
		//shift?
		imm11 = (address & 0x800) << 20 ;
		imm20 = (address & 0x100000) << 31  ;
		imm10_1 = (address & 0x7FE) << 21 ;
		imm19_12 = (address & 0xFF000) << 12;

		instWord = opcodeRISC | rd | imm11 | imm20 | imm10_1 | imm19_12;


	}

	else if (opcode == 0x3) {//jal

	}
	else
		cout << "UNKNOWN INSTRUCTION" << endl;
}

int main() {

	string word;
	unsigned int instWord = 0;
	cout << "Enter MIPS instruction in binary" << endl;
	cin >> word;
	instWord = unsigned(stoi(word,nullptr,2));
	 int instWordRISC = 0;
	convertMIPStoRISC(instWord, instWordRISC);
	cout << "Instruction in RISC-V is: 0x";
	cout << hex<<instWordRISC << endl;

	system("pause");
	return 0;
}

