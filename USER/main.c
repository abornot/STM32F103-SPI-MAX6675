#include "u_sys.h"
#include "u_delay.h"
#include "u_key.h"
#include "u_led.h"
#include "u_timer.h"
#include "u_usart.h"
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
  u8 retry=0;				 	
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
  {
    retry++;
    if(retry>200)
    {
      return 0;
    }
  }			  
  SPI_I2S_SendData(SPI1, TxData);               //通过外设SPIx发送一个数据
  retry=0;
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
  {
    retry++;
    if(retry>200)
    {
      return 0;
    }
  }	  						    
  return SPI_I2S_ReceiveData(SPI1);             //返回通过SPIx最近接收的数据					    
}
void SPI1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                       //设置SPI工作模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                   //设置SPI的数据大小
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                          //串行同步时钟的空闲状态为低电平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                        //串行同步时钟的第1个跳变沿（上升或下降）数据被采样
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                           //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//定义波特率预分频的值:波特率预分频值为256
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  //指定数据传输从MSB位还是LSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7;                            //CRC值计算的多项式
  SPI_Init(SPI1, &SPI_InitStructure);                                 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
  SPI_Cmd(SPI1, ENABLE);                                              //使能SPI外设
}
/**
 * 描述: SPI速度设置函数
 * 输入: 分频倍数
*/
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
  SPI1->CR1&=0XFFC7;
  SPI1->CR1|=SPI_BaudRatePrescaler;
  SPI_Cmd(SPI1, ENABLE); 
}
void ThermocoupleInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
  SPI1_Init();
  SPI1_SetSpeed(SPI_BaudRatePrescaler_8);                     //设置为4.5M时钟
}
/**
 * 描述: 读取热电偶温度
 * 输入: 热电偶编号
 * 返回: 温度
*/
float readThermocoupleTemperature(u8 Num)
{
  u8 i, pBuffer[2];
  float Temp;
  switch (Num)
  {
    case 0:
      CS_K1 = 0;
      break;
    case 1:
      CS_K2 = 0;
      break;
    case 2:
      CS_K3 = 0;
      break;
    default:
      break;
  }
  for(i = 0; i < 2; i ++)
  {
    pBuffer[i] = SPI1_ReadWriteByte(0XFF);
  }
  switch (Num)
  {
    case 0:
      CS_K1 = 1;
      break;
    case 1:
      CS_K2 = 1;
      break;
    case 2:
      CS_K3 = 1;
      break;
    default:
      break;
  }
  Temp=((((pBuffer[1]|pBuffer[0]<<8)))>>3);
  Temp*=0.25;
  return Temp;
}
/**
 * 描述: 温度计算
 * 位置: 100ms
*/
void runTempCalc(void)
{
  u8 i;
  GulThermocoupleTemperatureTemp[GucThermocoupleNum] += readThermocoupleTemperature(GucThermocoupleNum);
  GucThermocoupleNum ++;
  if (GucThermocoupleNum > 2)
  {
    GucThermocoupleNum = 0;
    GucCalcNum ++;
  }
  if (GucCalcNum >= 3)
  {
    GucCalcNum = 0;
    for (i = 0; i < 3; i++)
    {
      GssThermocoupleTemperature[i] = (u16)(GulThermocoupleTemperatureTemp[i]/3);
      GulThermocoupleTemperatureTemp[i] = 0;
    }
  }
}
/**
 * 描述: 时间循环函数
 * 位置: 主循环
*/
void runTimeTask(void)
{
  //1ms
  if (1 == Bflg1msDelaytime)
  {
    Bflg1msDelaytime = 0;
    Guc10msDelaytime ++;
  }
  //10ms
  if (Guc10msDelaytime >= 10)
  {
    Guc10msDelaytime = 0;
    Guc100msDelaytime ++;
  }
  //100ms
  if (Guc100msDelaytime >= 10)
  {
    Guc100msDelaytime = 0;
    runTempCalc();
  }
}
int main(void)
{
  delay_init();	    	                            //延时函数初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	ThermocoupleInit();
  TIM3_Int_Init(9,7199);                          //10Khz的计数频率，计数到10为1ms
  while(1)
  {
    runTimeTask();
  }
}
