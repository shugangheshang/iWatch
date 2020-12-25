#include "oled.h"

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
void delay_ms(unsigned int ms)//@24.000MHz
{
	unsigned char i, j;
	for(ms;ms>0;ms--)
	{
		i = 24;
		j = 85;
		do
		{
			while (--j);
		} while (--i);
	}
}

//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	//OLED_RD_Set();
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();
	OLED_WR_Clr();	 
	OLED_DAT = dat;
	OLED_WR_Set();	  
	//OLED_DC_Set();	 
} 	    	    
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD); 
}   
//����OLED��ʾ    
void OLED_Display_On(void)		//������ʾ
{
	OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14, OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)		//�ر���ʾ
{
	OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10, OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD);  //DISPLAY OFF
}		
void OLED_Set_Brightness(unsigned char brightness)	//���öԱȶȣ����ȣ�
{
	OLED_WR_Byte(0x81, OLED_CMD);				//--set contrast control register
	OLED_WR_Byte(brightness, OLED_CMD);	// Set SEG Output Current Brightness
}
/*
void OLED_Horizental_Reverse(unsigned char reverse)	//ˮƽ��ת
{
	if(reverse)
		OLED_WR_Byte(0xA0, OLED_CMD);
	else
		OLED_WR_Byte(0xA1, OLED_CMD);
}
void OLED_Vertical_Reverse(unsigned char reverse)		//��ֱ��ת
{
	if(reverse)
		OLED_WR_Byte(0xC0, OLED_CMD);
	else
		OLED_WR_Byte(0xC8, OLED_CMD);
}
*/
void OLED_Inverse(unsigned char k)									//��ɫ
{
	if(k)
		OLED_WR_Byte(0xA7, OLED_CMD);
	else
		OLED_WR_Byte(0xA6, OLED_CMD);
}
void OLED_Clear(void)		//����
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n = 0; n < 128; n++)
			OLED_WR_Byte(0,OLED_DATA); 
	}
}
void OLED_Refresh(unsigned char *buf)
{
	unsigned char data i, j;
	unsigned int data n;	
	for(i = 0; i < 8; i++)  
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD);
		OLED_WR_Byte(0x10, OLED_CMD);
		OLED_WR_Byte(0x00, OLED_CMD); 
		n = i * 128;
		for(j = 0; j < 128; j++)
			OLED_WR_Byte(buf[n + j], OLED_DATA); 
	}
}
//��ʼ��SSD1315					    
void OLED_Init()
{
	OLED_RST_Set();
	delay_ms(20);
	OLED_RST_Clr();
	delay_ms(10);
	OLED_RST_Set(); 
	OLED_RD_Set();
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0x7f,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xC8,OLED_CMD);
	OLED_WR_Byte(0xA1,OLED_CMD);
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	//OLED_WR_Byte(0xD6,OLED_CMD);
	//OLED_WR_Byte(0x01,OLED_CMD);
	//OLED_WR_Byte(0x23,OLED_CMD);
	//OLED_WR_Byte(0x30,OLED_CMD);
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  
