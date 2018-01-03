


	/*
	This is just a skeleton. It DOES NOT implement all the requirements.
	It only recognizes the "ADD", "SUB" and "ADDI"instructions and prints
	"Unkown Instruction" for all other instructions!

	References:
	(1) The risc-v ISA Manual ver. 2.1 @ https://riscv.org/specifications/
	(2) https://github.com/michaeljclark/riscv-meta/blob/master/meta/opcodes
	*/


	// check BEQ BGE // PC+4
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>


	using namespace std;
	int regs[32] = { 0 };
	unsigned int pc = 0x0;
	int base;
	const int memsize = 8 * 1024;


	char memory[memsize];	// only 8KB of memory located at address 0

	//8*1024
	void emitError(char *s)
	{
		cout << s;
		system("pause");
		exit(0);
	}

	void printPrefix(unsigned int instA, unsigned int instW) {
		cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
	}
	bool instDecExec(unsigned int instWord)
	{
		unsigned int rd, rs1, rs2, funct3, funct7, opcode;
		unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
		unsigned int address;

		unsigned int instPC = pc - 4;

		opcode = instWord & 0x0000007F;
		rd = (instWord >> 7) & 0x0000001F;
		funct3 = (instWord >> 12) & 0x00000007;
		funct7 = (instWord >> 25);
		rs1 = (instWord >> 15) & 0x0000001F;
		rs2 = (instWord >> 20) & 0x0000001F;

		// — inst[31] — inst[30:25] inst[24:21] inst[20]
		I_imm = ((instWord >> 20) & 0x7FF) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));

		B_imm = ((instWord >> 8) & 0xF) |
			(((instWord >> 25) & 0x3F) << 4) |
			(((instWord >> 7) & 0x1) << 10) |
			(((instWord >> 31) & 0x1) << 11);

		B_imm = (B_imm << 1);
		B_imm = B_imm | ((B_imm >> 12) ? 0xFFFFE000 : 0x0);

		//S_imm = ((instWord >> 7) & 0x1F) | (((instWord >> 25) & 0x7F) << 5 );                                             
		//S_imm = S_imm | ((S_imm >> 11) ? 0xFFFFF000 : 0x0);

		S_imm = ((funct7 << 5) | rd) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));

		J_imm = ((instWord >> 21) & 0x3FF) |
			(((instWord >> 20) & 0x1) << 10) |
			(((instWord >> 12) & 0xFF) << 11) |
			((instWord >> 31) << 19);


		J_imm = (J_imm << 1) | (((J_imm >> 19) ? 0xFFE00000 : 0x0));

		U_imm = instWord & 0xFFFFF000;


		unsigned int shInst = I_imm >> 5;
		unsigned int Shamt = I_imm & 0x1F;

		printPrefix(instPC, instWord);

		if (opcode == 0x33) {		// R Instructions
			switch (funct3) {
			case 0: if (funct7 == 32) {
				cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				regs[rd] = regs[rs1] - regs[rs2];
			}
					else {
						cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
						regs[rd] = regs[rs1] + regs[rs2];
					}
					break;
			case 1: //sll check
				cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				if (rd != 0)
					regs[rd] = regs[rs1] << regs[rs2];
				break;
			case 2: //slt
				cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				if (rd != 0)
					regs[rd] = regs[rs1] < regs[rs2];
				break;
			case 3: //sltu
				cout << "\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				if (rd != 0)
					regs[rd] = unsigned(regs[rs1]) < unsigned(regs[rs2]);
				break;
			case 4: //xor
				cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				if (rd != 0)
					regs[rd] = regs[rs1] ^ regs[rs2];
				break;
			case 5: // srl // sra
				if (funct7 == 0) {
					cout << "\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
					if (rd != 0)
						regs[rd] = unsigned(regs[rs1]) >> unsigned(regs[rs2]);
				}
				else {
					cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
					if (rd != 0)
						regs[rd] = regs[rs1] >> regs[rs2];
				}

				break;
			case 6: // or
				cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				if (rd != 0)
					regs[rd] = regs[rs1] | regs[rs2];
				break;
			case 7:	//and
				cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				if (rd != 0)
					regs[rd] = regs[rs1] & regs[rs2];
				break;

			default:
				cout << "\tUnkown R Instruction \n";
			}
		}
		else if (opcode == 0x13) {	// I instructions
			switch (funct3) {
			case 0:	cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
				regs[rd] = regs[rs1] + (int)I_imm;
				break;
			case 1: //sll check
				cout << "\tSLLI\tx" << dec << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";

				regs[rd] = regs[rs1] << (int)I_imm;//shamt VS Iimm
				break;
			case 2:
				cout << "\tSLTI\tx" << dec << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
				regs[rd] = (regs[rs1] < (int)I_imm);
				break;
			case 3:
				cout << "\tSLTIU\tx" << dec << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
				regs[rd] = ((unsigned)regs[rs1] < (int)I_imm);
				break;
			case 4:
				cout << "\tXORI\tx" << dec << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
				regs[rd] = regs[rs1] ^ (int)I_imm;
			case 5:

				if (funct7 == 0) {
					cout << "\tSRLI\tx" << dec << rd << ", x" << rs1 << ", " << hex << "0x" << Shamt << "\n";
					regs[rd] = (unsigned)regs[rs1] >> (int)I_imm;//shamt VS Iimm
					break;
				}
				else if (funct7 == 32)
				{
					cout << "\tSRAI\tx" << dec << rd << ", x" << rs1 << ", " << hex << "0x" << Shamt << "\n";
					regs[rd] = regs[rs1] >> (int)Shamt;
					break;
				}
				/*	else {
				cout << "ERROR! " << endl;
				break;
				}*/
				break;
			case 6:
				cout << "\tORI\tx" << dec << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
				regs[rd] = regs[rs1] | (int)I_imm;
				break;

			case 7:
				cout << "\tANDI\tx" << dec << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
				regs[rd] = regs[rs1] & (int)I_imm;
				break;

			default:
				cout << "\tUnkown I Instruction \n";
			}
		}
		else if (opcode == 0x3) // I+1 instruction
		{
			switch (funct3) {
			case 0:
				cout << "\tLB\tx" << dec << rd << ", " << hex << "0x" << (int)I_imm << "(x" << dec << rs1 << ")" << "\n";
				regs[rd] = (memory[regs[rs1] + (int)I_imm] & 0xFF) |
					(((memory[regs[rs1] + (int)I_imm] >> 7) ? 0xFFFFFF00 : 0x0));
				break;

			case 1:
				cout << "\tLH\tx" << dec << rd << ", " << hex << "0x" << (int)I_imm << "(x" << dec << rs1 << ")" << "\n";
				regs[rd] = (memory[regs[rs1] + (int)I_imm] & 0xFF) |
					((memory[regs[rs1] + (int)I_imm + 1] << 8) & 0xFFFF) |
					((memory[regs[rs1] + (int)I_imm + 1] >> 7) ? 0xFFFF0000 : 0x0);
			case 2:
				cout << "\tLW\tx" << dec << rd << ", " << hex << "0x" << (int)I_imm << "(x" << dec << rs1 << ")" << "\n";
				regs[rd] = ((memory[regs[rs1] + (int)I_imm]) & 0xFF) |
					((memory[regs[rs1] + (int)I_imm + 1] << 8) & 0xFFFF) |
					((memory[regs[rs1] + (int)I_imm + 2] << 16) & 0xFFFFFF) |
					((memory[regs[rs1] + (int)I_imm + 3] << 24));
				break;
			case 4:
				cout << "\tLBU\tx" << dec << rd << ", " << hex << "0x" << (int)I_imm << "(x" << dec << rs1 << ")" << "\n";
				regs[rd] = (memory[regs[rs1] + (int)I_imm] & 0xFF);
				break;
			case 5:
				cout << "\tLHU\tx" << dec << rd << ", " << hex << "0x" << (int)I_imm << "(x" << dec << rs1 << ")" << "\n";
				regs[rd] = memory[regs[rs1] + (int)I_imm] | (memory[regs[rs1] + (int)I_imm + 1] << 8);
				break;
			default:
				cout << "\tUnkown Instruction \n";
				break;

			}
		}
		else if (opcode == 0x23) // S instruction
		{
			switch (funct3) {
			case 0:
				cout << "\tSB\tx" << dec << rs2 << hex << ", 0x" << (int)S_imm << "(x" << dec << rs1 << ")" << "\n";
				memory[regs[rs1] + S_imm] = regs[rs2] & 0xFF;
				break;
			case 1:	
				cout << "\tSH\tx" << dec << rs2 << hex << ", 0x" << (int)S_imm << "(x" << dec << rs1 << ")" << "\n";
				memory[regs[rs1] + S_imm] = regs[rs2] & 0xFF;
				memory[regs[rs1] + S_imm + 1] = (regs[rs2] >> 8) & 0xFF;
				break;
			case 2:	cout << "\tSW\tx" << dec << rs2 << hex << ", 0x" << (int)S_imm << "(x" << dec << rs1 << ")" << "\n";
				memory[regs[rs1] + S_imm] = regs[rs2] & 0xFF;
				memory[regs[rs1] + S_imm + 1] = (regs[rs2] >> 8) & 0xFF;
				memory[regs[rs1] + S_imm + 2] = (regs[rs2] >> 16) & 0xFF;
				memory[regs[rs1] + S_imm + 3] = (regs[rs2] >> 24) & 0xFF;
				break;

			default:
				cout << "\tUnkown I Instruction \n";
			}
		}
		else if (opcode == 0x6F) // J INSTRUCTIONS 
		{
			cout << "\tJAL\tx" << dec << rd << ", " << hex << "0x" << (int)J_imm << "\n";
			regs[rd] = pc;
			regs[0] = 0;
			pc = instPC + (int)J_imm;
		}
		else if (opcode == 0x67) // JALR instruction	
		{
			cout << "\tJALR\tx" << dec << rd << ", x" << ", " << hex << "0x" << (int)I_imm << "\n";
			if (rd != 0)
				regs[rd] = pc;
			else regs[rd] = 0;
			pc = (I_imm + regs[rs1]) & 0xFFFFFFFE;

			// register x0 can be used as the destination if the result is not required 
		}
		else if (opcode == 0x63) // SB instruction format
		{

			switch (funct3) {
			case 0: cout << "\tBEQ\tx" << dec << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
				if (regs[rs1] == regs[rs2])
					pc = instPC + B_imm;
				break;
			case 1: cout << "\tBNE\tx" << dec << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
				if (regs[rs1] != regs[rs2])
					pc = instPC + B_imm;
				break;
			case 4: cout << "\tBLT\tx" << dec << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
				if (regs[rs1] < regs[rs2])
					pc = instPC + B_imm;
				break;
			case 5: cout << "\tBGE\tx" << dec << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
				if (regs[rs1] >= regs[rs2])
					pc = instPC + B_imm;
				break;
			case 6: cout << "\tBLTU\tx" << dec << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
				if (unsigned(regs[rs1]) < unsigned(regs[rs2]))
					pc = instPC + B_imm;
				break;
			case 7: cout << "\tBGEU\tx" << dec << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
				if (unsigned(regs[rs1]) >= unsigned(regs[rs2]))
					pc = instPC + B_imm;
				break;

			default:
				cout << "\tUnkown SB Instruction \n";
			}
			
		}
		else if (opcode == 0x37)// Load upper immediate 
		{
			cout << "\tLUI\tx" << dec << rd << ", " << hex << "0x" << ((U_imm >> 12) | (((instWord >> 31) ? 0xFFFFF800 : 0x0))) << "\n";
			regs[rd] = U_imm << 12;
		}
		else if (opcode == 0x17) // Add Upper Immediate Program Counter
		{
			cout << "\tAUIPC\tx" << dec << rd << ", " << hex << "0x" << (int)U_imm << "\n";
			regs[rd] = pc + (U_imm << 12);                                                  // Instpc or current PC
		}
		else if (opcode == 0x73) // ecall
		{
			cout << "\tECALL\t" << "\n";
			switch (regs[10]) { //DO NOT FORGET TO CHANGE
			case 1:
				cout << dec << regs[10] << endl; //a0??????????
				break;
			case 4:
				 base = regs[10]; //a0?????????
				while (memory[base] != '\0')
				{
					cout << memory[base];
					base++;
				}
				cout << endl;
				break;
			case 5:
				cin >> regs[10];
				break;
			case 10:
				return false;
				break;
			default:
				break;
			}
			
			
		}

		else {
			
			cout << "\tUnkown Instruction \n";
			return false;
		
		}
		return true;
	}

	int main(int argc, char *argv[]) {

		unsigned int instWord = 0;
		ifstream inFile;
		ofstream outFile;

		if (argc<1) emitError("use: rv32i_sim <machine_code_file_name>\n");

		inFile.open(argv[1], ios::in | ios::binary | ios::ate);

		if (inFile.is_open())
		{
			int fsize = inFile.tellg(); //position of the first charachter

			inFile.seekg(0, inFile.beg);
			if (!inFile.read((char *)memory, fsize)) emitError("Cannot read from input file\n");
			bool flag = true;
			regs[2] = memsize - 1;
			while (flag) {
				instWord = (unsigned char)memory[pc] |
					(((unsigned char)memory[pc + 1]) << 8) |
					(((unsigned char)memory[pc + 2]) << 16) |
					(((unsigned char)memory[pc + 3]) << 24);
				pc += 4;
				// remove the following line once you have a complete simulator
				//  if (pc == 15*4+8) break;			// stop when PC reached address 32
				flag=instDecExec(instWord);
				regs[0] = 0;
			}

			// dump the registers
			for (int i = 0; i<32; i++)
				cout << "x" << dec << i << ": \t" << "0x" << hex << std::setfill('0') << std::setw(8) << regs[i] << "\n";

			system("pause");

		}
		else emitError("Cannot access input file\n");
		return 0;
	}
