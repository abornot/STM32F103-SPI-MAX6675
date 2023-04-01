#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h" 	 

//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_OS		0		//定义系统文件夹是否支持UCOS
																	    
	 
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址

#define PSB                   PCout(0)                     //PSB
#define JR3                   PCout(13)                    //加热3
#define JR2                   PCout(14)                    //加热2
#define JR1                   PCout(15)                    //加热1
#define CS_K3                 PAout(3)                     //
#define CS_K2                 PAout(4)                     //
#define CS_K1                 PAout(7)                     //
#define PUMP1                 PBout(9)                     //水泵1
#define PUMP2                 PBout(8)                     //水泵2
#define PUMP3                 PBout(7)                     //水泵3
#define DCF1                  PBout(6)                     //电磁阀1
#define LED1                  PBout(4)                     //加热指示灯 红色
#define LED2                  PBout(3)                     //喷雾指示灯 绿色
#define LED3                  PDout(2)                     //预留指示灯 绿色
#define Buzz                  PCout(12)                    //蜂鸣器
#define LCD_CLK               PBout(2)                     //
#define LCD_SID               PBout(10)                    //
#define LCD_CS                PBout(11)                    //

#define SW1                   PCin(4)                      //
#define SW2                   PCin(5)                      //
#define K4                    PAin(11)                     //
#define K3                    PAin(8)                      //
#define K2                    PCin(9)                      //
#define K1                    PCin(8)                      //

#define AlarmRing             1
#define OneShortRing          2
#define TwoShortRing          3
#define ThreeShortRing        4
#define InvalidKeyRing        5
#define LongRing1s            6
#define GucCodeVersion        1
#define GucSpeedGear0         0
#define GucSpeedGear1         1049      //静档 300RPM
#define GucSpeedGear2         1359      //中档 500RPM
#define GucSpeedGear3         2199      //高档 850RPM
#define GucSpeedGear4         3599      //急速 1200RPM
extern u8 Bflg1msDelaytime;
extern u8 BflgKey1Done;
extern u8 BflgBuzzerRingDelaytime;
extern u8 BflgBuzzerStopDelaytime;
extern u8 BflgBuzzerRing;
extern u8 BflgSwitchOnDone;
extern u8 BflgWaterPumpEnable;
extern u8 BflgProcessDone;
extern u8 BflgKey1EnableType;
extern u8 Guc10msDelaytime;
extern u8 Guc100msDelaytime;
extern u8 GucSwitchEnableOnDelaytime;
extern u8 GucSwitchEnableOffDelaytime;
extern u8 GucKey1EnableOffDelaytime;
extern u8 GucBuzzerRingType;
extern u8 GucBuzzerRingCount;
extern u8 GucBuzzerStopDelaytime;
extern u8 GucBuzzerStopDelaytimeTemp;
extern u8 GucRunningDelaytime;
extern u8 GucRunningStatusDelaytime;
extern u8 GucLastTimeHour;
extern u8 GucLastTimeMinute;
extern u8 GucLastTimeSecond;
extern u8 GucThermocoupleNum;
extern u8 GucCalcNum;
extern u8 GucMenuSwitch;
extern u8 GucBackToMainMenuDelaytime;
extern u8 GucParameterNum;
extern u16 GusBuzzerDelaytime;
extern u16 GusBuzzerRingDelaytime;
extern u16 GusLastTimeDelaytime;
extern u16 GusKey1EnableOnDelaytime;
extern s16 GssThermocoupleTemperature[3];
extern s16 GssTargetTemperature;
extern u32 GulThermocoupleTemperatureTemp[3];
#endif
