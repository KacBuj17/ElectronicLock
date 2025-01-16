#include "rfid.h"

#define MFRC522_SPI_SPEED 1000000

void MFRC522_Init(void) 
{
		spi_init(SPI_MODE_0, MFRC522_SPI_SPEED, SPI_SIDE_MASTER);
	
    PORTA->PCR[MFRC522_CS_PIN] = PORT_PCR_MUX(1);
    PTA->PDDR |= (1 << MFRC522_CS_PIN);
    PTA->PSOR = (1 << MFRC522_CS_PIN);
	
    PORTA->PCR[MFRC522_RST_PIN] = PORT_PCR_MUX(1);
    PTA->PDDR |= (1 << MFRC522_RST_PIN);
    PTA->PSOR = (1 << MFRC522_RST_PIN);

    MFRC522_Reset();

    Write_MFRC522(TModeReg, 0x8D);       // Tauto=1; f(Timer) = 6.78MHz/TPreScaler
    Write_MFRC522(TPrescalerReg, 0x3E); // TModeReg[3..0] + TPrescalerReg
    Write_MFRC522(TReloadRegL, 30);
    Write_MFRC522(TReloadRegH, 0);

    Write_MFRC522(TxAutoReg, 0x40);     // Force 100% ASK modulation
    Write_MFRC522(ModeReg, 0x3D);       // CRC Initial value 0x6363

    AntennaOn();
}

void Write_MFRC522(uint8_t addr, uint8_t val) 
{
    PTA->PCOR = (1 << MFRC522_CS_PIN);

    spi_master_write((addr << 1) & 0x7E);
    spi_master_write(val);

    PTA->PSOR = (1 << MFRC522_CS_PIN);
}

uint8_t Read_MFRC522(uint8_t addr) 
{
    PTA->PCOR = (1 << MFRC522_CS_PIN);

    spi_master_write(((addr << 1) & 0x7E) | 0x80);
    uchar val = spi_master_write(0x00);

    PTA->PSOR = (1 << MFRC522_CS_PIN);

    return val;
}

void SetBitMask(uint8_t reg, uint8_t mask) 
{
		uchar tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp | mask);
} 

void ClearBitMask(uint8_t reg, uint8_t mask) 
{
    uchar tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp & (~mask));
}

void AntennaOn(void) 
{
		Read_MFRC522(TxControlReg);
    SetBitMask(TxControlReg, 0x03);
}

void AntennaOff(void) 
{
    ClearBitMask(TxControlReg, 0x03);
}

void MFRC522_Reset(void) 
{
    Write_MFRC522(CommandReg, PCD_RESETPHASE);
}

uchar MFRC522_ToCard(uchar command, uchar *sendData, uchar sendLen, uchar *backData, uint *backLen)
{
    uchar status = MI_ERR;
    uchar irqEn = 0x00;
    uchar waitIRq = 0x00;
    uchar lastBits;
    uchar n;
    uint i;

    switch (command)
    {
        case PCD_AUTHENT:		// Certification cards close
				{
					irqEn = 0x12;
					waitIRq = 0x10;
					break;
				}
				case PCD_TRANSCEIVE:	// Transmit FIFO data
				{
					irqEn = 0x77;
					waitIRq = 0x30;
					break;
				}
				default:
					break;
    }

    Write_MFRC522(CommIEnReg, irqEn|0x80);	// Interrupt request
    ClearBitMask(CommIrqReg, 0x80);			// Clear all interrupt request bit
    SetBitMask(FIFOLevelReg, 0x80);			// FlushBuffer=1, FIFO Initialization

		Write_MFRC522(CommandReg, PCD_IDLE);	// NO action; Cancel the current command

	// Writing data to the FIFO
    for (i=0; i<sendLen; i++)
    {
			Write_MFRC522(FIFODataReg, sendData[i]);
		}

    // Execute the command
		Write_MFRC522(CommandReg, command);
    if (command == PCD_TRANSCEIVE)
    {
			SetBitMask(BitFramingReg, 0x80);		// StartSend=1,transmission of data starts
		}

    // Waiting to receive data to complete
		i = 2000;	// i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms
    do
    {
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
			n = Read_MFRC522(CommIrqReg);
			i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitIRq));

    ClearBitMask(BitFramingReg, 0x80);			//StartSend=0

    if (i != 0)
    {
        if(!(Read_MFRC522(ErrorReg) & 0x1B))	//BufferOvfl Collerr CRCErr ProtecolErr
        {
					status = MI_OK;
					if (n & irqEn & 0x01)
					{
					status = MI_NOTAGERR;
					}

					if (command == PCD_TRANSCEIVE)
					{
						n = Read_MFRC522(FIFOLevelReg);
						lastBits = Read_MFRC522(ControlReg) & 0x07;
						if (lastBits)
						{
							*backLen = (n-1)*8 + lastBits;
						}
						else
						{
							*backLen = n*8;
						}

						if (n == 0)
						{
							n = 1;
						}
										if (n > MAX_LEN)
										{
							n = MAX_LEN;
						}

						// Reading the received data in FIFO
						for (i=0; i<n; i++)
						{
							backData[i] = Read_MFRC522(FIFODataReg);
						}
          }
        }
        else
        {
					status = MI_ERR;
				}
    }

    //SetBitMask(ControlReg,0x80);           //timer stops
    //Write_MFRC522(CommandReg, PCD_IDLE);

    return status;
}

uchar MFRC522_Request(uchar reqMode, uchar *TagType)
{
	uchar status;
	uint backBits;			 // The received data bits

	Write_MFRC522(BitFramingReg, 0x07);		//TxLastBists = BitFramingReg[2..0]

	TagType[0] = reqMode;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

	if ((status != MI_OK) || (backBits != 0x10))
	{
		status = MI_ERR;
	}

	return status;
}

uchar MFRC522_Anticoll(uchar *serNum)
{
    uchar status;
    uchar i;
		uchar serNumCheck=0;
    uint unLen;

	Write_MFRC522(BitFramingReg, 0x00);		//TxLastBists = BitFramingReg[2..0]

    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

    if (status == MI_OK)
		{
			//Check card serial number
			for (i=0; i<4; i++)
			{
				serNumCheck ^= serNum[i];
			}
			if (serNumCheck != serNum[i])
			{
				status = MI_ERR;
			}
    }

    return status;
}

void CalulateCRC(uchar *pIndata, uchar len, uchar *pOutData)
{
    uchar i, n;

    ClearBitMask(DivIrqReg, 0x04);			//CRCIrq = 0
    SetBitMask(FIFOLevelReg, 0x80);			//Clear the FIFO pointer

    //Writing data to the FIFO
    for (i=0; i<len; i++)
    {
			Write_MFRC522(FIFODataReg, *(pIndata+i));
		}
    Write_MFRC522(CommandReg, PCD_CALCCRC);

    //Wait CRC calculation is complete
    i = 0xFF;
    do
    {
        n = Read_MFRC522(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));			//CRCIrq = 1

    //Read CRC calculation result
    pOutData[0] = Read_MFRC522(CRCResultRegL);
    pOutData[1] = Read_MFRC522(CRCResultRegH);
}

uchar MFRC522_SelectTag(uchar *serNum)
{
	uchar i;
	uchar status;
	uchar size;
	uint recvBits;
	uchar buffer[9];

	//ClearBitMask(Status2Reg, 0x08);			//MFCrypto1On=0

    buffer[0] = PICC_SElECTTAG;
    buffer[1] = 0x70;
    for (i=0; i<5; i++)
    {
    	buffer[i+2] = *(serNum+i);
    }
		CalulateCRC(buffer, 7, &buffer[7]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);

    if ((status == MI_OK) && (recvBits == 0x18))
    {
			size = buffer[0];
		}
    else
    {
			size = 0;
		}

    return size;
}

uchar MFRC522_Auth(uchar authMode, uchar BlockAddr, uchar *Sectorkey, uchar *serNum)
{
    uchar status;
    uint recvBits;
    uchar i;
		uchar buff[12];

	//Verify the command block address + sector + password + card serial number
    buff[0] = authMode;
    buff[1] = BlockAddr;
    for (i=0; i<6; i++)
    {
			buff[i+2] = *(Sectorkey+i);
		}
    for (i=0; i<4; i++)
    {
			buff[i+8] = *(serNum+i);
		}
    status = MFRC522_ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);

    if ((status != MI_OK) || (!(Read_MFRC522(Status2Reg) & 0x08)))
    {
			status = MI_ERR;
		}

    return status;
}

uchar MFRC522_Read(uchar blockAddr, uchar *recvData)
{
    uchar status;
    uint unLen;

    recvData[0] = PICC_READ;
    recvData[1] = blockAddr;
    CalulateCRC(recvData,2, &recvData[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);

    if ((status != MI_OK) || (unLen != 0x90))
    {
        status = MI_ERR;
    }

    return status;
}

uchar MFRC522_Write(uchar blockAddr, uchar *writeData)
{
    uchar status;
    uint recvBits;
    uchar i;
		uchar buff[18];

    buff[0] = PICC_WRITE;
    buff[1] = blockAddr;
    CalulateCRC(buff, 2, &buff[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);

    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
    {
			status = MI_ERR;
		}

    if (status == MI_OK)
    {
			for (i=0; i<16; i++)		//Data to the FIFO write 16Byte
			{
				buff[i] = *(writeData+i);
			}
			CalulateCRC(buff, 16, &buff[16]);
			status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);

			if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
			{
				status = MI_ERR;
			}
    }

    return status;
}

void MFRC522_Halt(void)
{
	uint unLen;
	uchar buff[4];

	buff[0] = PICC_HALT;
	buff[1] = 0;
	CalulateCRC(buff, 2, &buff[2]);

	MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);
}
