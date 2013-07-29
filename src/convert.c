// -------------------------------------------------------------------------
// -					Functions for data convertion					   -
// -------------------------------------------------------------------------

// Reverse the direction of a Byte
uint8 RvsByte(uint8 dByte)
{
	uint8 i;
	uint8 tmp = 0;
	for (i = 0; i < 8; i++)
	{
		tmp = (tmp<<1) | ((dByte>>i) & 0x01);
	}
	return tmp;
}

// Convert 123 to '1','2','3'
uint8* NumberToChar(uint8 Number)
{
	// Base code + ASCII offset
	static uint8 Dest[3];
	Dest[0] = Number/100 + 48;
	Dest[1] = (Number%100)/10 + 48;
	Dest[2] = (Number%100)%10 + 48;
	return Dest;
}

