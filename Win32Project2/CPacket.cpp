#include "CPacket.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "Utility.h"

CPacket::CPacket(WORD opcode)
{
	m_buffer = new byte[4096];
	m_bufferPos = 0;

	WriteUInt16(0x0000);

	WriteUInt16(opcode);

	WriteUInt16(0x0000);
}
CPacket::CPacket()
{
	m_buffer = new byte[4096];
	m_bufferPos = 0;

}
CPacket::~CPacket()
{
	delete[] m_buffer;
}


void CPacket::WriteByte(BYTE value)
{
	m_buffer[m_bufferPos] = value;
	m_bufferPos += 1;
}

void CPacket::WriteBytes(BYTE* src, int len)
{
	memcpy(m_buffer + m_bufferPos, src, len);
	m_bufferPos += len;
}

void CPacket::WriteBytesCR(BYTE* src, int len)
{
	WriteUInt16(len);
	memcpy(m_buffer + m_bufferPos, src, len);
	m_bufferPos += len;
}

void CPacket::WriteUInt16(uint16_t value)
{
	memcpy(m_buffer + m_bufferPos, &value,2);
	m_bufferPos += 2;
}

void CPacket::WriteUInt32(uint32_t value)
{
	memcpy(m_buffer + m_bufferPos, &value, 4);
	m_bufferPos += 4;
}

void CPacket::WriteAscii(std::string value)
{
	uint16_t len = (value.length());
	WriteUInt16(len);



	std::vector<byte> v(len);
	copy(value.begin(), value.end(), v.begin());
	WriteBytes(v.data(),len);

}


int CPacket::GetRawSize()
{
	return m_bufferPos;
}
BYTE * CPacket::GetRawBytes()
{
	*(WORD*) (m_buffer + 0) = m_bufferPos - 6;
	return m_buffer;
}