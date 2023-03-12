#include <Emulator/CPU/CPU.hpp>
#include <Emulator/CPU/Instructions/InstrCommon.hpp>

/*
	MICRO-INSTRUCTIONS (if its called this way)
*/
// ADC, SBC, ADD, SUB
template<typename SUB, typename CARRY>
ALWAYS_INLINE void CPU::ADD_A_8(u8 operand)	
{
	m_tclock += 4; PC++;

	u16 res = GET_FLAG(CarryFlag) * CARRY + operand * (SUB * -1) + GET_REG(RegisterA);
	SET_FLAG(ZeroFlag, res == 0);
	SET_FLAG(SubstractFlag, SUB);
	SET_FLAG(HalfCarryFlag, (res >> 4) & 1);
	SET_FLAG(CarryFlag, (res >> 8) & 1);
	SET_REG(RegisterA, static_cast<u8>(res));
}
ALWAYS_INLINE void CPU::ADD_REG16_REG16(u8 dst, u8 src)
{
	m_tclock += 8; PC++;

	u32 res = GET_COMPOSED_REG(dst) + GET_COMPOSED_REG(src);
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, (res >> 8) & 1);
	SET_FLAG(CarryFlag, (res >> 16) & 1);
	SET_COMPOSED_REG(dst, static_cast<u16>(res));
}
ALWAYS_INLINE void CPU::ADC(u8 operand)			{ ADD_A_8<0, 1>(operand); }
ALWAYS_INLINE void CPU::SBC(u8 operand)			{ ADD_A_8<1, 1>(operand); }
ALWAYS_INLINE void CPU::ADD(u8 operand)			{ ADD_A_8<0, 0>(operand); }
ALWAYS_INLINE void CPU::SUB(u8 operand)			{ ADD_A_8<1, 0>(operand); }
ALWAYS_INLINE void CPU::DEC_REG16(u8 reg) 		{ m_tclock += 8; PC++; DEC_COMPOSED_REG(src); }
ALWAYS_INLINE void CPU::INC_REG16(u8 reg) 		{ m_tclock += 8; PC++; INC_COMPOSED_REG(src); }
ALWAYS_INLINE void CPU::DEC_REG8(u8 reg)  		{ m_tclock += 4; PC++; DEC_REG(src); SET_FLAG(ZeroFlag, GET_REG(src) == 0); SET_FLAG(SubstractFlag, 1); SET_FLAG(HalfCarryFlag, (GET_REG(src) >> 4) & 1); }
ALWAYS_INLINE void CPU::INC_REG8(u8 reg) 		{ m_tclock += 4; PC++; INC_REG(src); SET_FLAG(ZeroFlag, GET_REG(src) == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, (GET_REG(src) >> 4) & 1); }
ALWAYS_INLINE void CPU::DEC_ADDR(u16 address)	{ m_tclock += 4; PC++; u8 b = GET_BYTE(address) - 1; SET_FLAG(ZeroFlag, b == 0); SET_FLAG(SubstractFlag, 1); SET_FLAG(HalfCarryFlag, (b >> 4) & 1); SET_BYTE(address, b); }
ALWAYS_INLINE void CPU::INC_ADDR(u16 address)	{ m_tclock += 4; PC++; u8 b = GET_BYTE(address) + 1; SET_FLAG(ZeroFlag, b == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, (b >> 4) & 1); SET_BYTE(address, b); }

ALWAYS_INLINE void CPU::AND(u8 operand)			{ m_tclock += 4; PC++; u8 a = GET_REG(RegisterA) & operand; SET_REG(RegisterA, a); SET_FLAG(ZeroFlag, a == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, 1); SET_FLAG(CarryFlag, 0); }
ALWAYS_INLINE void CPU::XOR(u8 operand)			{ m_tclock += 4; PC++; u8 a = GET_REG(RegisterA) ^ operand; SET_REG(RegisterA, a); SET_FLAG(ZeroFlag, a == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, 0); SET_FLAG(CarryFlag, 0); }
ALWAYS_INLINE void CPU::OR(u8 operand)			{ m_tclock += 4; PC++; u8 a = GET_REG(RegisterA) | operand; SET_REG(RegisterA, a); SET_FLAG(ZeroFlag, a == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, 0); SET_FLAG(CarryFlag, 0); }
ALWAYS_INLINE void CPU::CP(u8 operand)
{
	m_tclock += 4; PC++;

	u8 a = GET_REG(RegisterA);
	SET_FLAG(ZeroFlag, a == operand);
	SET_FLAG(SubstractFlag, 1);
	SET_FLAG(HalfCarryFlag, ((a - operand) & 0xF) > (a & 0xF));
	SET_FLAG(CarryFlag, a < operand);
}

/*
	ALL THE INC,DEC INSTRUCTIONS
*/
void CPU::DEC_A()								{ DEC_REG8(RegisterA); }
void CPU::DEC_B()								{ DEC_REG8(RegisterB); }
void CPU::DEC_C()								{ DEC_REG8(RegisterC); }
void CPU::DEC_D()								{ DEC_REG8(RegisterD); }
void CPU::DEC_E()								{ DEC_REG8(RegisterE); }
void CPU::DEC_H()								{ DEC_REG8(RegisterH); }
void CPU::DEC_L()								{ DEC_REG8(RegisterL); }
void CPU::DEC_ADDR_HL()							{ DEC_ADDR(GET_COMPOSED_REG(RegisterHL)); }

void CPU::DEC_BC()								{ DEC_REG16(RegisterBC); }
void CPU::DEC_DE()								{ DEC_REG16(RegisterBC); }
void CPU::DEC_HL()								{ DEC_REG16(RegisterBC); }
void CPU::DEC_SP()								{ DEC_REG16(RegisterBC); }

void CPU::INC_A()								{ INC_REG8(RegisterA); }
void CPU::INC_B()								{ INC_REG8(RegisterB); }
void CPU::INC_C()								{ INC_REG8(RegisterC); }
void CPU::INC_D()								{ INC_REG8(RegisterD); }
void CPU::INC_E()								{ INC_REG8(RegisterE); }
void CPU::INC_H()								{ INC_REG8(RegisterH); }
void CPU::INC_L()								{ INC_REG8(RegisterL); }
void CPU::INC_ADDR_HL()							{ INC_ADDR(GET_COMPOSED_REG(RegisterHL)); }

void CPU::INC_BC()								{ INC_REG16(RegisterBC); }
void CPU::INC_DE()								{ INC_REG16(RegisterBC); }
void CPU::INC_HL()								{ INC_REG16(RegisterBC); }
void CPU::INC_SP()								{ INC_REG16(RegisterBC); }

/*
	ALL THE SBC, ADC, SUB, ADD INSTRUCTIONS
*/
void CPU::ADD_A_A()								{ ADD(GET_REG(RegisterA)); }
void CPU::ADD_A_B()								{ ADD(GET_REG(RegisterB)); }
void CPU::ADD_A_C()								{ ADD(GET_REG(RegisterC)); }
void CPU::ADD_A_D()								{ ADD(GET_REG(RegisterD)); }
void CPU::ADD_A_E()								{ ADD(GET_REG(RegisterE)); }
void CPU::ADD_A_H()								{ ADD(GET_REG(RegisterH)); }
void CPU::ADD_A_L()								{ ADD(GET_REG(RegisterL)); }
void CPU::ADD_A_ADDR_HL()						{ ADD(GET_BYTE(GET_COMPOSED_REG(RegisterHL))); }
void CPU::ADD_A_IMM8()							{ ADD(FETCH_BYTE(PC + 1)); }

void CPU::ADC_A_A()								{ ADC(GET_REG(RegisterA)); }
void CPU::ADC_A_B()								{ ADC(GET_REG(RegisterB)); }
void CPU::ADC_A_C()								{ ADC(GET_REG(RegisterC)); }
void CPU::ADC_A_D()								{ ADC(GET_REG(RegisterD)); }
void CPU::ADC_A_E()								{ ADC(GET_REG(RegisterE)); }
void CPU::ADC_A_H()								{ ADC(GET_REG(RegisterH)); }
void CPU::ADC_A_L()								{ ADC(GET_REG(RegisterL)); }
void CPU::ADC_A_ADDR_HL()						{ ADC(GET_BYTE(GET_COMPOSED_REG(RegisterHL))); }
void CPU::ADC_A_IMM8()							{ ADC(FETCH_BYTE(PC + 1)); }

void CPU::SUB_A_A()								{ SUB(GET_REG(RegisterA)); }
void CPU::SUB_A_B()								{ SUB(GET_REG(RegisterB)); }
void CPU::SUB_A_C()								{ SUB(GET_REG(RegisterC)); }
void CPU::SUB_A_D()								{ SUB(GET_REG(RegisterD)); }
void CPU::SUB_A_E()								{ SUB(GET_REG(RegisterE)); }
void CPU::SUB_A_H()								{ SUB(GET_REG(RegisterH)); }
void CPU::SUB_A_L()								{ SUB(GET_REG(RegisterL)); }
void CPU::SUB_A_ADDR_HL()						{ SUB(GET_BYTE(GET_COMPOSED_REG(RegisterHL))); }
void CPU::SUB_A_IMM8()							{ SUB(FETCH_BYTE(PC + 1)); }

void CPU::SBC_A_A()								{ SBC(GET_REG(RegisterA)); }
void CPU::SBC_A_B()								{ SBC(GET_REG(RegisterB)); }
void CPU::SBC_A_C()								{ SBC(GET_REG(RegisterC)); }
void CPU::SBC_A_D()								{ SBC(GET_REG(RegisterD)); }
void CPU::SBC_A_E()								{ SBC(GET_REG(RegisterE)); }
void CPU::SBC_A_H()								{ SBC(GET_REG(RegisterH)); }
void CPU::SBC_A_L()								{ SBC(GET_REG(RegisterL)); }
void CPU::SBC_A_ADDR_HL()						{ SBC(GET_BYTE(GET_COMPOSED_REG(RegisterHL))); }
void CPU::SBC_A_IMM8()							{ SBC(FETCH_BYTE(PC + 1)); }

void CPU::ADD_HL_BC()							{ ADD_REG16_REG16(RegisterHL, RegisterBC); }
void CPU::ADD_HL_DE()							{ ADD_REG16_REG16(RegisterHL, RegisterDE); }
void CPU::ADD_HL_HL()							{ ADD_REG16_REG16(RegisterHL, RegisterHL); }
void CPU::ADD_HL_SP()							{ ADD_REG16_REG16(RegisterHL, RegisterSP); }

void CPU::ADD_SP_IMM8()
{
	m_tclock += 12;
	
	u32 n = GET_COMPOSED_REG(RegisterSP) + static_cast<s8>(FETCH_BYTE(PC + 1));
	SET_REG(RegisterSP, static_cast<u16>(n));
	SET_FLAG(ZeroFlag, 0);
	SET_FLAG(SubstractFlag, 0);
    SET_FLAG(HalfCarryFlag, !!(n & 0x100));
    SET_FLAG(CarryFlag,		!!(n & 0x10000));

	PC += 1;
}

void CPU::AND_A_A()								{ AND(GET_REG(RegisterA)); }
void CPU::AND_A_B()								{ AND(GET_REG(RegisterB)); }
void CPU::AND_A_C()								{ AND(GET_REG(RegisterC)); }
void CPU::AND_A_D()								{ AND(GET_REG(RegisterD)); }
void CPU::AND_A_E()								{ AND(GET_REG(RegisterE)); }
void CPU::AND_A_H()								{ AND(GET_REG(RegisterH)); }
void CPU::AND_A_L()								{ AND(GET_REG(RegisterL)); }
void CPU::AND_A_ADDR_HL()						{ AND(GET_BYTE(GET_COMPOSED_REG(RegisterHL))); }
void CPU::AND_A_IMM8()							{ AND(FETCH_BYTE(PC + 1)); }

void CPU::OR_A_A()								{ OR(GET_REG(RegisterA)); }
void CPU::OR_A_B()								{ OR(GET_REG(RegisterB)); }
void CPU::OR_A_C()								{ OR(GET_REG(RegisterC)); }
void CPU::OR_A_D()								{ OR(GET_REG(RegisterD)); }
void CPU::OR_A_E()								{ OR(GET_REG(RegisterE)); }
void CPU::OR_A_H()								{ OR(GET_REG(RegisterH)); }
void CPU::OR_A_L()								{ OR(GET_REG(RegisterL)); }
void CPU::OR_A_ADDR_HL()						{ OR(GET_BYTE(GET_COMPOSED_REG(RegisterHL))); }
void CPU::OR_A_IMM8()							{ OR(FETCH_BYTE(PC + 1)); }

void CPU::XOR_A_A()								{ XOR(GET_REG(RegisterA)); }
void CPU::XOR_A_B()								{ XOR(GET_REG(RegisterB)); }
void CPU::XOR_A_C()								{ XOR(GET_REG(RegisterC)); }
void CPU::XOR_A_D()								{ XOR(GET_REG(RegisterD)); }
void CPU::XOR_A_E()								{ XOR(GET_REG(RegisterE)); }
void CPU::XOR_A_H()								{ XOR(GET_REG(RegisterH)); }
void CPU::XOR_A_L()								{ XOR(GET_REG(RegisterL)); }
void CPU::XOR_A_ADDR_HL()						{ XOR(GET_BYTE(GET_COMPOSED_REG(RegisterHL))); }
void CPU::XOR_A_IMM8()							{ XOR(FETCH_BYTE(PC + 1)); }

void CPU::CP_A_A()								{ CP(GET_REG(RegisterA)); }
void CPU::CP_A_B()								{ CP(GET_REG(RegisterB)); }
void CPU::CP_A_C()								{ CP(GET_REG(RegisterC)); }
void CPU::CP_A_D()								{ CP(GET_REG(RegisterD)); }
void CPU::CP_A_E()								{ CP(GET_REG(RegisterE)); }
void CPU::CP_A_H()								{ CP(GET_REG(RegisterH)); }
void CPU::CP_A_L()								{ CP(GET_REG(RegisterL)); }
void CPU::CP_A_ADDR_HL()						{ CP(GET_BYTE(GET_COMPOSED_REG(RegisterHL))); }
void CPU::CP_A_IMM8()							{ CP(FETCH_BYTE(PC + 1)); }

// DAA: Decimal Adjust A, the way the GB handles BCD (Binary-Coded Decimal) arithmetic
void CPU::DAA()
{
	m_tclock += 4; PC++;

	u8 a = GET_REG(RegisterA);
	if (GET_FLAG(SubstractFlag))
	{
		if (GET_FLAG(CarryFlag))
			a -= 0x60;
		if (GET_FLAG(HalfCarryFlag))
			a -= 0x6;
	}
	else
	{
		if (GET_FLAG(CarryFlag) || a > 0x99)
		{
			a += 0x60;
			SET_FLAG(CarryFlag);
		}
		if (GET_FLAG(HalfCarryFlag) || ((a & 0x0F) > 0x09))
			a += 0x6;
	}
	SET_FLAG(ZeroFlag, a == 0);
	SET_FLAG(HalfCarry, 0);

	SET_REG(RegisterA, a);
}

void CPU::CPL()
{
	m_tclock += 4; PC++;

	SET_REG(RegisterA, ~GET_REG(RegisterA));
	SET_FLAG(SubstractFlag, 1);
	SET_FLAG(HalfCarryFlag, 1);
}

void CPU::CCF()
{
	m_tclock += 4; PC++;

	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, !GET_FLAG(CarryFlag));
}

void CPU::SCF()
{
	m_tclock += 4; PC++;

	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, 1);
}