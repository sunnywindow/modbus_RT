/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "AHT20-21_DEMO_V1_3.h" 

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 0
#define REG_INPUT_NREGS 5

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
//static 
uint16_t   usRegInputBuf[REG_INPUT_NREGS];
uint16_t InputBuff[5];
uint32_t CT_data[2]={0,0};
	volatile int  c1,c2,t1,t2;

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
		int             i;
		AHT20_Read_CTdata_crc(CT_data);  //crcУ��󣬶�ȡAHT20���¶Ⱥ�ʪ������ 
		c1 = CT_data[0]*1000/1024/1024;  //����õ�ʪ��ֵc1���Ŵ���10����
		t1 = CT_data[1]*2000/1024/1024-500;//����õ��¶�ֵt1���Ŵ���10����
		c2 = c1/10 + (c1/10)%10;
		t2 = t1/10 + (t1/10)%10;
		InputBuff[0] = t2;
		InputBuff[1] = c2;
		InputBuff[2] = 0x01;
		InputBuff[3] = 0x01;
		
    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
				for(i=0;i<usNRegs;i++)
				{
					*pucRegBuffer=InputBuff[i+usAddress-1]>>8;
					pucRegBuffer++;
					*pucRegBuffer=InputBuff[i+usAddress-1]&0xff;
					pucRegBuffer++;
				}
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    return MB_ENOREG;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}

