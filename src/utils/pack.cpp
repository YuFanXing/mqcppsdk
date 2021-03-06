#include "../../include/utils/pack.h"

ByteInputStream::ByteInputStream(const uint8_t *buffer)
{
    Buffer = buffer;
    Offset = 0;
}

bool ByteInputStream::Check(const uint32_t len, const uint32_t i)
{
    return len >= Offset + i;
}

void ByteInputStream::Set(const uint8_t *buffer)
{
    Buffer = buffer;
    Offset = 0;
}

void ByteInputStream::Skip(uint32_t i)
{
    Offset += i;
}

uint8_t ByteInputStream::GetByte()
{
    return Buffer[Offset++];
}

uint16_t ByteInputStream::GetShort()
{
    uint16_t i;
    i = Buffer[Offset] << 8;
    i |= Buffer[++Offset];
    ++Offset;
    return i;
}

uint32_t ByteInputStream::GetInt()
{
    uint32_t i;
    i = Buffer[Offset] << 24;
    i |= Buffer[++Offset] << 16;
    i |= Buffer[++Offset] << 8;
    i |= Buffer[++Offset];
    ++Offset;
    return i;
}

uint64_t ByteInputStream::GetLong()
{
    uint64_t i;
    i = (uint64_t)Buffer[Offset] << 56;
    i |= (uint64_t)Buffer[++Offset] << 48;
    i |= (uint64_t)Buffer[++Offset] << 40;
    i |= (uint64_t)Buffer[++Offset] << 32;
    i |= (uint64_t)Buffer[++Offset] << 24;
    i |= (uint64_t)Buffer[++Offset] << 16;
    i |= (uint64_t)Buffer[++Offset] << 8;
    i |= (uint64_t)Buffer[++Offset];
    ++Offset;
    return i;
}

float ByteInputStream::GetFloat()
{
    return 0.0f;
}

double ByteInputStream::GetDouble()
{
    return 0.0;
}

const uint8_t *ByteInputStream::GetBin(const uint32_t len)
{
    uint32_t tmp = Offset;
    ByteInputStream::Skip(len);
    return Buffer + tmp;
}

const uint8_t *ByteInputStream::GetCurrentPoint()
{
    return Buffer + Offset;
}

uint32_t ByteInputStream::GetReadedLength()
{
    return Offset;
}

void UnPack::Reset(const uint8_t *buffer, const uint32_t bufferlen)
{
    ByteInputStream::Set(buffer);
    BufferLen = bufferlen;
}

//自动跳长度
void UnPack::Reset(const LPBYTE buffer)
{
    ByteInputStream::Set(buffer);
    ByteInputStream::Skip(4);
    BufferLen = XBin::Bin2Int(buffer);
}

void UnPack::Reset(const std::vector<uint8_t> *buffer)
{
    ByteInputStream::Set((*buffer).data());
    BufferLen = (*buffer).size();
}

uint8_t UnPack::GetByte()
{
    if (Check(BufferLen, 1))
        return ByteInputStream::GetByte();
    return 0;
}

uint16_t UnPack::GetShort()
{
    if (Check(BufferLen, 2))
        return ByteInputStream::GetShort();
    return 0;
}

uint32_t UnPack::GetInt()
{
    if (Check(BufferLen, 4))
        return ByteInputStream::GetInt();
    return 0;
}

uint64_t UnPack::GetLong()
{
    if (Check(BufferLen, 8))
        return ByteInputStream::GetLong();
    return 0;
}

float UnPack::GetFloat()
{
    if (Check(BufferLen, 4))
        return ByteInputStream::GetFloat();
    return 0;
}

double UnPack::GetDouble()
{
    if (Check(BufferLen, 8))
        return ByteInputStream::GetDouble();
    return 0;
}

const char *UnPack::GetStr(const uint32_t len)
{
    if (Check(BufferLen, len))
        return (const char *)ByteInputStream::GetBin(len);
    return 0;
}

const uint8_t *UnPack::GetBin(const uint32_t len)
{
    if (Check(BufferLen, len))
        return ByteInputStream::GetBin(len);
    return 0;
}

const LPBYTE UnPack::GetBin()
{
    uint32_t len = UnPack::GetInt() - 4;
    if (Check(BufferLen, len))
        return (const LPBYTE)(ByteInputStream::GetBin(len) - 4);
    return 0;
}

uint32_t UnPack::GetLeftLength()
{
    return BufferLen - ByteInputStream::GetReadedLength();
}