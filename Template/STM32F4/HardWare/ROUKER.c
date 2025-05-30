#include "ROUKER.h"

uint16_t Rocker_Value_BUFF[ADC_CHANNELS] = { 0 };

/****
	* @brief    	GPIO 配置		  
	* @param   		无
	* @return   	无
	* Sample usage:	EQUIPMENT_GPIO_Init();
    */
void EQUIPMENT_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//GPIO 时钟使能
	ROCKER_ADCx_GPIO_AHBxClockCMD(ROCKER_ADCx_GPIO_CLK | Electricity_ADCx_GPIO_CLK,ENABLE);
	//GPIO	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;		//模拟输入
	GPIO_InitStruct.GPIO_Pin = ROCKER_ADCL1_Pin | ROCKER_ADCL2_Pin | 
							   ROCKER_ADCR1_Pin | ROCKER_ADCR2_Pin | Electricity_ADC_Pin;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; 	//不上下拉电阻
	GPIO_Init(ROCKER_ADCx_GPIO_PORT,&GPIO_InitStruct);
	GPIO_Init(Electricity_ADCx_GPIO_PORT,&GPIO_InitStruct);
}

/*
typedef struct
{
  uint32_t DMA_Channel;      			//通道选择
  uint32_t DMA_PeripheralBaseAddr;		//外设地址
  uint32_t DMA_Memory0BaseAddr;  		//存储器0地址
  uint32_t DMA_DIR;       				//传输方向
  uint32_t DMA_BufferSize;    			//数据数目
  uint32_t DMA_PeripheralInc;  			//外设递增
  uint32_t DMA_MemoryInc;       		//存储器递增
  uint32_t DMA_PeripheralDataSize; 		//外设数据宽度
  uint32_t DMA_MemoryDataSize;  		//存储器数据宽度
  uint32_t DMA_Mode;     				//模式选择
  uint32_t DMA_Priority;    			//优先级
  uint32_t DMA_FIFOMode;       			//FIFO 模式
  uint32_t DMA_FIFOThreshold;  			//FIFO 阈值
  uint32_t DMA_MemoryBurst; 			//存储器突发传输
  uint32_t DMA_PeripheralBurst;     	//外设突发传输
}DMA_InitTypeDef;
*/
/****
	* @brief    	DMA 配置		  
	* @param   		无
	* @return   	无
	* Sample usage:	EQUIPMENT_DMA_Init();
    */
void EQUIPMENT_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStruct;
	DMA_DeInit(PERIPHERAL_DMAX_Stream);	//将 DMA 的所有寄存器重置为默认值	
	PERIPHERAL_DMAx_AHBxClockCMD(PERIPHERAL_DMAx_CLK,ENABLE);	//DMA 时钟使能

	DMA_InitStruct.DMA_BufferSize = ADC_CHANNELS;	//DMA 缓冲区大小
	DMA_InitStruct.DMA_Channel = PERIPHERAL_DMAx_Channel;	//DMA 通道 4
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;	//DMA 数据传输方向：外设到内存
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;		//禁止DMA FIFO ，使用直连模式
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;	//DMA FIFO 阈值：半满
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)Rocker_Value_BUFF;		//DMA 内存地址;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//DMA 内存数据大小：半字
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;	//DMA 内存突发传输模式：单次传输
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;		//DMA 内存地址自增：使能
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;				//DMA 模式：循环模式
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&EQUIPMENT_ADCx->DR;	//DMA 外设地址
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//DMA 外设突发传输模式：单次传输
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//DMA 外设数据大小：半字
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//DMA 外设地址自增：使能
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(PERIPHERAL_DMAX_Stream,&DMA_InitStruct);

	//使能 DMA 传输完成中断
	DMA_Cmd(PERIPHERAL_DMAX_Stream,ENABLE);
}

/****
	* @brief    	ADC 配置		  
	* @param   		无
	* @return   	无
	* Sample usage:	EQUIPMENT_ADC_Init();
    */
void EQUIPMENT_ADC_Init()
{
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ROCKER_ADCx_APBxClockCMD(ROCKER_ADCx_CLK,ENABLE);

	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 	//禁用DMA直接访问模式
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;			//ADC独立模式
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;		//ADC预分频器 2分频
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;		//采样延迟 5个周期
	ADC_CommonInit(&ADC_CommonInitStruct);

	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;			//启用连续转换模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;		//右对齐
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//无外部触发
	//使用软件触发，外部触发不用配置
	//ADC_InitStruct.ADC_ExternalTrigConv = ;
	ADC_InitStruct.ADC_NbrOfConversion = ADC_CHANNELS;		//通道数目
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;		//12位模式
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;				//扫描模式
	ADC_Init(EQUIPMENT_ADCx,&ADC_InitStruct);

	//设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(EQUIPMENT_ADCx,ROCKER_ADCx_Channel1,1,ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(EQUIPMENT_ADCx,ROCKER_ADCx_Channel2,2,ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(EQUIPMENT_ADCx,ROCKER_ADCx_Channel3,3,ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(EQUIPMENT_ADCx,ROCKER_ADCx_Channel4,4,ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(EQUIPMENT_ADCx,Electricity_ADCx_Channel5,5,ADC_SampleTime_56Cycles);

	ADC_DMARequestAfterLastTransferCmd(EQUIPMENT_ADCx,ENABLE);

	ADC_DMACmd(EQUIPMENT_ADCx,ENABLE);		//使能 ADC DMA

	ADC_Cmd(EQUIPMENT_ADCx,ENABLE);			//使能 ADC

	ADC_SoftwareStartConv(EQUIPMENT_ADCx);	//软件启动 ADC 转换
}

/****
	* @brief    	摇杆 初始化		  
	* @param   		无
	* @return   	无
	* Sample usage:	EQUIPMENT_Init();
    */
void EQUIPMENT_Init()	
{
	EQUIPMENT_GPIO_Init();
	EQUIPMENT_ADC_Init();
	EQUIPMENT_DMA_Init();
}

/****
	* @brief    	卡尔曼滤波
	* @param   		*Value	数据地址
	* @return   	无
	* Sample usage:	KalmanFilter(Rocker_Value_BUFF[4]);
    */
static void KalmanFilter(uint16_t *Value)
{
	static	float PrevDelta = 0;	//上一次的预测值
    static float R=0.0001;			//测量噪声方差
    static float P=10;				//预测噪声方差
    static float K=0;				//卡尔曼增益
    static float Q=0.001;			//过程噪声方差
   	P = P + Q;						//预测
	K = P / (P + R);				//计算卡尔曼增益
	*Value = PrevDelta + (K * (*Value - PrevDelta));	//更新
	P = (1 - K) * P;				//更新估计误差协方差	
	PrevDelta = *Value;				//更新上一次的预测值
}

/****
	* @brief    ADC 数据处理		  
	* @param   	Value   处理的数据
	* @return   无    	
	* Sample usage:ROCKER_Calculate(&Value);
    */
void ROCKER_Calculate(ROCKER_Value *Value)
{
	Value->ROCKER_RX_Value = (Rocker_Value_BUFF[0] - Y_ADC_MIN) * (Y_COORDINATE_MAX - Y_COORDINATE_MIN) / 
                             (Y_ADC_MAX - Y_ADC_MIN) + Y_COORDINATE_MIN;		//右摇杆 X 轴坐标
	Value->ROCKER_RX_Value = (Rocker_Value_BUFF[1] - Y_ADC_MIN) * (Y_COORDINATE_MAX - Y_COORDINATE_MIN) / 
                             (Y_ADC_MAX - Y_ADC_MIN) + Y_COORDINATE_MIN;		//右摇杆 Y 轴坐标					 
	Value->ROCKER_RX_Value = (Rocker_Value_BUFF[2] - Y_ADC_MIN) * (Y_COORDINATE_MAX - Y_COORDINATE_MIN) / 
                             (Y_ADC_MAX - Y_ADC_MIN) + Y_COORDINATE_MIN;		//左摇杆 X 轴坐标
	Value->ROCKER_RX_Value = (Rocker_Value_BUFF[3] - Y_ADC_MIN) * (Y_COORDINATE_MAX - Y_COORDINATE_MIN) / 
                             (Y_ADC_MAX - Y_ADC_MIN) + Y_COORDINATE_MIN;		//左摇杆 Y 轴坐标
	Value->ROCKER_RX_Value = (float)(Rocker_Value_BUFF[4] * 3.3 / 4095 * 3); 	//电池电压
}

/****
	* @brief    ADC 数据发送	  
	* @param   	无
	* @return   无    	
	* Sample usage:ROCKER_DirSend(); 
    */
void ROCKERData_Send()
{
	uint8_t ROCKER_Buff[11] = "0";
	ROCKER_Value Value;
	ROCKER_Calculate(&Value);
	ROCKER_Buff[0] = 'R';
    ROCKER_Buff[1] = 'Y';
    ROCKER_Buff[2] = Value.ROCKER_RY_Value;
    ROCKER_Buff[3] = 'X';
    ROCKER_Buff[4] = Value.ROCKER_RX_Value;
    ROCKER_Buff[5] = 'L';
    ROCKER_Buff[6] = 'Y';
    ROCKER_Buff[7] = Value.ROCKER_LY_Value;
   	ROCKER_Buff[8] = 'X';
    ROCKER_Buff[9] = Value.ROCKER_LX_Value;
    ROCKER_Buff[10] = 0;
}
