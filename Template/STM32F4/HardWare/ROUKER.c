#include "ROUKER.h"

uint16_t Rocker_Value_BUFF[ADC_CHANNELS] = { 0 };

/****
	* @brief    	GPIO ����		  
	* @param   		��
	* @return   	��
	* Sample usage:	EQUIPMENT_GPIO_Init();
    */
void EQUIPMENT_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//GPIO ʱ��ʹ��
	ROCKER_ADCx_GPIO_AHBxClockCMD(ROCKER_ADCx_GPIO_CLK | Electricity_ADCx_GPIO_CLK,ENABLE);
	//GPIO	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;		//ģ������
	GPIO_InitStruct.GPIO_Pin = ROCKER_ADCL1_Pin | ROCKER_ADCL2_Pin | 
							   ROCKER_ADCR1_Pin | ROCKER_ADCR2_Pin | Electricity_ADC_Pin;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; 	//������������
	GPIO_Init(ROCKER_ADCx_GPIO_PORT,&GPIO_InitStruct);
	GPIO_Init(Electricity_ADCx_GPIO_PORT,&GPIO_InitStruct);
}

/*
typedef struct
{
  uint32_t DMA_Channel;      			//ͨ��ѡ��
  uint32_t DMA_PeripheralBaseAddr;		//�����ַ
  uint32_t DMA_Memory0BaseAddr;  		//�洢��0��ַ
  uint32_t DMA_DIR;       				//���䷽��
  uint32_t DMA_BufferSize;    			//������Ŀ
  uint32_t DMA_PeripheralInc;  			//�������
  uint32_t DMA_MemoryInc;       		//�洢������
  uint32_t DMA_PeripheralDataSize; 		//�������ݿ��
  uint32_t DMA_MemoryDataSize;  		//�洢�����ݿ��
  uint32_t DMA_Mode;     				//ģʽѡ��
  uint32_t DMA_Priority;    			//���ȼ�
  uint32_t DMA_FIFOMode;       			//FIFO ģʽ
  uint32_t DMA_FIFOThreshold;  			//FIFO ��ֵ
  uint32_t DMA_MemoryBurst; 			//�洢��ͻ������
  uint32_t DMA_PeripheralBurst;     	//����ͻ������
}DMA_InitTypeDef;
*/
/****
	* @brief    	DMA ����		  
	* @param   		��
	* @return   	��
	* Sample usage:	EQUIPMENT_DMA_Init();
    */
void EQUIPMENT_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStruct;
	DMA_DeInit(PERIPHERAL_DMAX_Stream);	//�� DMA �����мĴ�������ΪĬ��ֵ	
	PERIPHERAL_DMAx_AHBxClockCMD(PERIPHERAL_DMAx_CLK,ENABLE);	//DMA ʱ��ʹ��

	DMA_InitStruct.DMA_BufferSize = ADC_CHANNELS;	//DMA ��������С
	DMA_InitStruct.DMA_Channel = PERIPHERAL_DMAx_Channel;	//DMA ͨ�� 4
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;	//DMA ���ݴ��䷽�����赽�ڴ�
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;		//��ֹDMA FIFO ��ʹ��ֱ��ģʽ
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;	//DMA FIFO ��ֵ������
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)Rocker_Value_BUFF;		//DMA �ڴ��ַ;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//DMA �ڴ����ݴ�С������
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;	//DMA �ڴ�ͻ������ģʽ�����δ���
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;		//DMA �ڴ��ַ������ʹ��
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;				//DMA ģʽ��ѭ��ģʽ
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&EQUIPMENT_ADCx->DR;	//DMA �����ַ
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//DMA ����ͻ������ģʽ�����δ���
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//DMA �������ݴ�С������
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//DMA �����ַ������ʹ��
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(PERIPHERAL_DMAX_Stream,&DMA_InitStruct);

	//ʹ�� DMA ��������ж�
	DMA_Cmd(PERIPHERAL_DMAX_Stream,ENABLE);
}

/****
	* @brief    	ADC ����		  
	* @param   		��
	* @return   	��
	* Sample usage:	EQUIPMENT_ADC_Init();
    */
void EQUIPMENT_ADC_Init()
{
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ROCKER_ADCx_APBxClockCMD(ROCKER_ADCx_CLK,ENABLE);

	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 	//����DMAֱ�ӷ���ģʽ
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;			//ADC����ģʽ
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;		//ADCԤ��Ƶ�� 2��Ƶ
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;		//�����ӳ� 5������
	ADC_CommonInit(&ADC_CommonInitStruct);

	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;			//��������ת��ģʽ
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;		//�Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//���ⲿ����
	//ʹ������������ⲿ������������
	//ADC_InitStruct.ADC_ExternalTrigConv = ;
	ADC_InitStruct.ADC_NbrOfConversion = ADC_CHANNELS;		//ͨ����Ŀ
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;		//12λģʽ
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;				//ɨ��ģʽ
	ADC_Init(EQUIPMENT_ADCx,&ADC_InitStruct);

	//����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(EQUIPMENT_ADCx,ROCKER_ADCx_Channel1,1,ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(EQUIPMENT_ADCx,ROCKER_ADCx_Channel2,2,ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(EQUIPMENT_ADCx,ROCKER_ADCx_Channel3,3,ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(EQUIPMENT_ADCx,ROCKER_ADCx_Channel4,4,ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(EQUIPMENT_ADCx,Electricity_ADCx_Channel5,5,ADC_SampleTime_56Cycles);

	ADC_DMARequestAfterLastTransferCmd(EQUIPMENT_ADCx,ENABLE);

	ADC_DMACmd(EQUIPMENT_ADCx,ENABLE);		//ʹ�� ADC DMA

	ADC_Cmd(EQUIPMENT_ADCx,ENABLE);			//ʹ�� ADC

	ADC_SoftwareStartConv(EQUIPMENT_ADCx);	//������� ADC ת��
}

/****
	* @brief    	ҡ�� ��ʼ��		  
	* @param   		��
	* @return   	��
	* Sample usage:	EQUIPMENT_Init();
    */
void EQUIPMENT_Init()	
{
	EQUIPMENT_GPIO_Init();
	EQUIPMENT_ADC_Init();
	EQUIPMENT_DMA_Init();
}

/****
	* @brief    	�������˲�
	* @param   		*Value	���ݵ�ַ
	* @return   	��
	* Sample usage:	KalmanFilter(Rocker_Value_BUFF[4]);
    */
static void KalmanFilter(uint16_t *Value)
{
	static	float PrevDelta = 0;	//��һ�ε�Ԥ��ֵ
    static float R=0.0001;			//������������
    static float P=10;				//Ԥ����������
    static float K=0;				//����������
    static float Q=0.001;			//������������
   	P = P + Q;						//Ԥ��
	K = P / (P + R);				//���㿨��������
	*Value = PrevDelta + (K * (*Value - PrevDelta));	//����
	P = (1 - K) * P;				//���¹������Э����	
	PrevDelta = *Value;				//������һ�ε�Ԥ��ֵ
}

/****
	* @brief    ADC ���ݴ���		  
	* @param   	Value   ���������
	* @return   ��    	
	* Sample usage:ROCKER_Calculate(&Value);
    */
void ROCKER_Calculate(ROCKER_Value *Value)
{
	Value->ROCKER_RX_Value = (Rocker_Value_BUFF[0] - Y_ADC_MIN) * (Y_COORDINATE_MAX - Y_COORDINATE_MIN) / 
                             (Y_ADC_MAX - Y_ADC_MIN) + Y_COORDINATE_MIN;		//��ҡ�� X ������
	Value->ROCKER_RX_Value = (Rocker_Value_BUFF[1] - Y_ADC_MIN) * (Y_COORDINATE_MAX - Y_COORDINATE_MIN) / 
                             (Y_ADC_MAX - Y_ADC_MIN) + Y_COORDINATE_MIN;		//��ҡ�� Y ������					 
	Value->ROCKER_RX_Value = (Rocker_Value_BUFF[2] - Y_ADC_MIN) * (Y_COORDINATE_MAX - Y_COORDINATE_MIN) / 
                             (Y_ADC_MAX - Y_ADC_MIN) + Y_COORDINATE_MIN;		//��ҡ�� X ������
	Value->ROCKER_RX_Value = (Rocker_Value_BUFF[3] - Y_ADC_MIN) * (Y_COORDINATE_MAX - Y_COORDINATE_MIN) / 
                             (Y_ADC_MAX - Y_ADC_MIN) + Y_COORDINATE_MIN;		//��ҡ�� Y ������
	Value->ROCKER_RX_Value = (float)(Rocker_Value_BUFF[4] * 3.3 / 4095 * 3); 	//��ص�ѹ
}

/****
	* @brief    ADC ���ݷ���	  
	* @param   	��
	* @return   ��    	
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
