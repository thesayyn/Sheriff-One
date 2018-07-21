#ifndef HG_CPACKET
#define HG_CPACKET
#include "common.h"

class CPacket
{
public:
	CPacket(WORD opcode);
	CPacket();
	~CPacket();

	void WriteByte(BYTE value);
	void WriteBytes(BYTE* src, int count);
	void WriteBytesCR(BYTE * src, int len);
	void WriteUInt16(uint16_t value);
	void WriteUInt32(uint32_t value);
	void WriteAscii(std::string string);

	int GetRawSize();
	BYTE* GetRawBytes();

private:
	BYTE* m_buffer;
	int m_bufferPos;
};

#endif