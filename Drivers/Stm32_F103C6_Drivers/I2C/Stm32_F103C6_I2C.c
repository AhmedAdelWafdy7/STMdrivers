/*
 * Stm32_F103C6_I2C.c
 *
 *  Created on: May 1, 2023
 *      Author: Ahmed Adel Wafdy
 */
#include "Stm32_F103C6_I2C.h"

I2C_Config_t Global_I2C_Config[2] = {0};
#define I2C1_Index				0
#define I2C2_Index				1


/* ================================================================
 * @Fn 				- MCAL_I2C_Init
 * @brief 			- Initializes SPIx according to the specified parameters in SPI_Config_t
 * @param [in] 		- I2Cx : where x can be (1..2 depending on device used) to select I2C peripheral
 * @param [in] 		- Config : a pointer to I2C_Config_t structure that contains the configuration information for the specified SPI Module
 * @retval 			- None
 * Note 			- Supported for I2C Standard mode Only
 * 					- You have to configure RCC to select clock for the selected I2C Module
 */
void MCAL_I2C_Init(I2C_t *I2Cx, I2C_Config_t *Config){
	uint16_t TempReg = 0 ,FREQ = 0;
	uint32_t PCLK1 = 800000;
	uint16_t CLK_Result;
	if(I2Cx == I2C1){
		Global_I2C_Config[0] = *Config;
		RCC_I2C1_CLK_EN();
	}
	else if(I2Cx == I2C2){
		Global_I2C_Config[1] = *Config;
		RCC_I2C2_CLK_EN();
	}
	//Set I2C mode
	if(Config->I2C_Mode == I2C_Mode_I2C_Mode){
		//timing initialization


		/*Bits 5:0 FREQ[5:0]: Peripheral clock frequency
The FREQ bits must be configured with the APB clock frequency value (I2C peripheral
connected to APB)*/
		TempReg  = I2Cx->CR2;
		TempReg  &= ~(I2C_CR2_FREQ_Msk);

		//Get PCLCK1
		PCLK1 = MCAL_RCC_Get_PCLK1Freq();

		//Set frequency range
		FREQ = (uint16_t)(PCLK1 / 1000000);

		TempReg  |= FREQ;

		I2Cx->CR2 = TempReg;

		//CLOCK Control
		I2Cx->CR1 &= ~(I2C_CR1_PE);
		TempReg = 0;
		if(Config->I2C_CLk_Speed == I2C_CLk_Speed_50KHz || Config->I2C_CLk_Speed == I2C_CLk_Speed_100KHz ){
			/*Controls the SCL clock in master mode.
Sm mode or SMBus:
T
high = CCR * TPCLK1
Tlow = CCR * TPCLK1
Fm mode:
If DUTY = 0:
T
high = CCR * TPCLK1
Tlow = 2 * CCR * TPCLK1
If DUTY = 1: (to reach 400 kHz)
T
high = 9 * CCR * TPCLK1
Tlow = 16 * CCR * TPCLK1
For instance: in Sm mode, to generate a 100 kHz SCL frequency:
If FREQR = 08, TPCLK1 = 125 ns so CCR must be programmed with 0x28
(0x28 <=> 40d x 125 ns = 5000 ns.)*/
			CLK_Result = (uint16_t)(PCLK1 / (Config->I2C_CLk_Speed<<1));

			TempReg |= CLK_Result;
			I2Cx->CCR = TempReg;
			//Bits 5:0 TRISE[5:0]: Maximum rise time in Fm/Sm mode (Master mode)
			//provide the maximum duration of the SCL feedback loop in master mode
			I2Cx->TRISE = FREQ +1;

		}
		// Set CR1 Register
		TempReg = I2Cx->CR1;
		TempReg |= (uint16_t) (Config->I2C_ACK_CR | Config->I2C_General_Call | Config->I2C_Stretch_Mode | Config->I2C_Mode);
		I2Cx->CR1 |= TempReg;

		// Set OAR2 Register
		TempReg = 0;
		if(Config->I2C_Slave_address.Enable_Dual_Address == 1){
			TempReg = I2C_OAR2_ENDUAL;
			TempReg |= Config->I2C_Slave_address.Slave_Second_Address << I2C_OAR2_ADD2_Pos;
			I2Cx->OAR2 |= TempReg;

		}
		TempReg = 0;
		TempReg |= Config->I2C_Slave_address.Slave_First_Address << 1 ;
		TempReg |= Config->I2C_Slave_address.Slave_Address_Mode;
		I2Cx->OAR1 |= TempReg;

	}
	else{
		/* NOT SUPPORTED */
	}


	//interrupt
	if(Config->P_IRQ_CallBack != NULL){
		//Enable IRQ
		I2Cx->CR2 |= (I2C_CR2_ITERREN);
		I2Cx->CR2 |= (I2C_CR2_ITBUFEN);
		I2Cx->CR2 |= (I2C_CR2_ITEVTEN);

		if(I2Cx == I2C1){
			NVIC_IRQ32_I2C1_ER_IRQ_EN();
			NVIC_IRQ31_I2C1_EV_IRQ_EN();
		}
		else if(I2Cx == I2C2){
			NVIC_IRQ33_I2C2_EV_IRQ_EN();
			NVIC_IRQ34_I2C2_ER_IRQ_EN();
		}
		I2Cx->SR1 =0;
		I2Cx->SR2 =0;

	}
	//Enable Peripheral
	I2Cx->CR1 |= I2C_CR1_PE;
}
/* ================================================================
 * @Fn 				- MCAL_I2C_DeInit
 * @brief 			- Resets Selected I2C Module
 * @param [in] 		- I2Cx : where x can be (1..2 depending on device used) to select I2C peripheral
 * @retval 			- None
 * Note 			- Reset The Module By RCC & Disable NVIC
 */
void MCAL_I2C_DeInit(I2C_t *I2Cx){
	if(I2Cx == I2C1){
		NVIC_IRQ32_I2C1_ER_IRQ_DI();
		NVIC_IRQ31_I2C1_EV_IRQ_DI();
		RCC_I2C1_RESET();
	}
	else if(I2Cx == I2C2){
		NVIC_IRQ33_I2C2_EV_IRQ_DI();
		NVIC_IRQ34_I2C2_ER_IRQ_DI();
		RCC_I2C2_RESET();
	}
}
/* ================================================================
 * @Fn 				- MCAL_I2C_GPIO_Set_Pins
 * @brief 			- Initializes GPIO Pins to be connected with the selected I2C
 * @param [in] 		- I2Cx : where x can be (1..2 depending on device used) to select I2C peripheral
 * @retval 			- None
 * Note 			- Supported for I2C Standard mode Only
 */
void MCAL_I2C_GPIO_Set_Pins(I2C_t *I2Cx){
	GPIO_PIN_CONFIG_T I2C_pin;
	if(I2Cx == I2C1){
		//SCL
		I2C_pin.GPIO_PIN_NUMBER = GPIO_PIN_6;
		I2C_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		I2C_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
		MCAL_GPIO_Init(GPIOB, &I2C_pin);

		//SDA
		I2C_pin.GPIO_PIN_NUMBER = GPIO_PIN_7;
		I2C_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		I2C_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
		MCAL_GPIO_Init(GPIOB, &I2C_pin);
	}
	else if(I2Cx == I2C2){
		//SCL
		I2C_pin.GPIO_PIN_NUMBER = GPIO_PIN_10;
		I2C_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		I2C_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
		MCAL_GPIO_Init(GPIOB, &I2C_pin);

		//SDA
		I2C_pin.GPIO_PIN_NUMBER = GPIO_PIN_11;
		I2C_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		I2C_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
		MCAL_GPIO_Init(GPIOB, &I2C_pin);
	}
}

/* ================================================================ */
/* ======================== Matser with polling =========================== */
/* ================================================================ */

/* ================================================================
 * @Fn 				- MCAL_I2C_Master_Transmit
 * @brief 			- MASTER Send Data with the selected I2C
 * @param [in] 		- I2Cx : where x can be (1..2 depending on device used) to select I2C peripheral
 * @param [in] 		- Device_Address : slave address
 * @param [in] 		- Data : a pointer to the data which will be send
 * @param [in] 		- Data_Length : number of data bytes to be Received
 * @param [in] 		- Stop : send stop bit or not
 * @param [in] 		- Start : send start or repeated start
 * @retval 			- None
 * Note 			- Supported for I2C Standard mode Only
 * Notes: 1- The EV5, EV6, EV9, EV8_1 and EV8_2 events stretch SCL low until the end of the corresponding software sequence.
2- The EV8 software sequence must complete before the end of the current byte transfer. In case EV8 software
sequence can not be managed before the current byte end of transfer, it is recommended to use BTF instead
of TXE with the drawback of slowing the communication
 */
void MCAL_I2C_Master_Transmit(I2C_t *I2Cx,uint16_t Device_Address, uint8_t *Data, uint8_t Data_Length, STOP_Condition Stop, START_Condition Start){
	int i = 0;
	//generate start
	I2C_Generate_Start(I2Cx, Start, Enable);

	//EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address
	while(!(I2C_Get_FlagStatus(I2Cx,SB)));

	//Write Address in DR
	I2C_Send_Address(I2Cx, Device_Address, Transmitter);

	//EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
	while(!(I2C_Get_FlagStatus(I2Cx,ADDR)));

	//EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR.
	while(!(I2C_Get_FlagStatus(I2Cx,TXE)));

	//EV8: TxE=1, shift register not empty, data register empty, cleared by writing DR register
	for (i = 0;i < Data_Length ;i++){
		//writing DR register
		I2Cx->DR = Data[i];
		//EV8_2: TxE=1, BTF = 1, Program Stop request. TxE and BTF are cleared by hardware by the Stop condition
		while(!(I2C_Get_FlagStatus(I2Cx,TXE)));
	}
	//generate Stop
	if(Stop == With_Stop){
		I2C_Stop(I2Cx, Enable);
	}
}
/* ================================================================
 * @Fn 				- MCAL_I2C_Master_Receive
 * @brief 			- MASTER Receive Data with the selected I2C
 * @param [in] 		- I2Cx : where x can be (1..2 depending on device used) to select I2C peripheral
 * @param [in] 		- Device_Address : slave address
 * @param [in] 		- Data : a pointer to the data which will be send
 * @param [in] 		- Data_Length : number of data bytes to be Received
 * @param [in] 		- Stop : send stop bit or not
 * @param [in] 		- Start : send start or repeated start
 * @retval 			- None
 * Note 			- Supported for I2C Standard mode Only
 * Notes: 1- The EV5, EV6, EV9, EV8_1 and EV8_2 events stretch SCL low until the end of the corresponding software sequence.
2- The EV8 software sequence must complete before the end of the current byte transfer. In case EV8 software
sequence can not be managed before the current byte end of transfer, it is recommended to use BTF instead
of TXE with the drawback of slowing the communication
 */
void MCAL_I2C_Master_Receive(I2C_t *I2Cx, uint16_t Device_Address, uint8_t *Data, uint8_t Data_Length, STOP_Condition Stop, START_Condition Start){
	int i = 0;

	uint8_t index = I2Cx == I2C1 ? I2C1_Index : I2C2_Index;
	//generate start
	I2C_Generate_Start(I2Cx, Start, Enable);

	//EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address
	while(!(I2C_Get_FlagStatus(I2Cx,SB)));

	//Write Address in DR
	I2C_Send_Address(I2Cx, Device_Address, Receiver);

	//EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
	while(!(I2C_Get_FlagStatus(I2Cx,ADDR)));

	//Enable Acknowledge
	I2C_ACKConfig(I2Cx, Enable);

	if(Data_Length){

		for (i = 0;i < Data_Length ;i++){

			while(!(I2C_Get_FlagStatus(I2Cx,RXNE)));
			//writing DR register
			Data[i] = I2Cx->DR ;
			Data++;
		}
	}

	//send NACK
	 I2C_ACKConfig(I2Cx, Disable);
		//generate Stop
		if(Stop == With_Stop){
			I2C_Stop(I2Cx, Enable);
		}
	 if(Global_I2C_Config[index].I2C_ACK_CR == I2C_ACK_CR_ACK_Enable){
		 I2C_ACKConfig(I2Cx, Enable);
	 }
	 else{
		 I2C_ACKConfig(I2Cx, Disable);
	 }
}

/* ================================================================ */
/* ======================== Slave Interrupt =========================== */
/* ================================================================ */

/* ================================================================
 * @Fn 				- MCAL_I2C_Slave_Transmit
 * @brief 			- Slave Transmit data to master
 * @param [in] 		- I2Cx : where x can be (1..2 depending on device used) to select I2C peripheral
 * @param [in] 		- pTxBuffer : Pointer to buffer Which holds the Tx data
 * @retval 			- None
 * Note 			- Reset The Module By RCC & Disable NVIC
 */
void MCAL_I2C_Slave_Transmit(I2C_t *I2Cx, uint8_t pTxBuffer){
	I2Cx->DR = pTxBuffer;

}
/* ================================================================
 * @Fn 				- MCAL_I2C_Slave_Receive
 * @brief 			- Slave receive data from master
 * @param [in] 		- I2Cx : where x can be (1..2 depending on device used) to select I2C peripheral
 * @retval 			- Data from DR register
 * Note 			- Reset The Module By RCC & Disable NVIC
 */
uint8_t MCAL_I2C_Slave_Receive(I2C_t *I2Cx){
	return(I2Cx->DR);
}


/* ================================================================ */
/* ======================== Generic =========================== */
/* ================================================================ */

void I2C_Generate_Start(I2C_t *I2Cx,START_Condition Start, Functional_State State){
	if(Start != Repeated_Start){
		while(I2C_Get_FlagStatus(I2Cx, BUS_Busy));
	}
	//write start condition
	if(State != Disable){
		 I2Cx->CR1 |= I2C_CR1_START;
	}
	else{
		 I2Cx->CR1 &=~( I2C_CR1_START);
	}
}
void I2C_Send_Address(I2C_t *I2Cx, uint16_t Device_Address,I2C_Direction Direction){
	//for 7bit address mode only
	if(Direction == Receiver){
		I2Cx->DR = ((Device_Address <<1) | (0x01));
	}
	else if(Direction == Transmitter){
		I2Cx->DR = (Device_Address << 1);
	}
}

void I2C_Stop(I2C_t *I2Cx, Functional_State State){
	if(State == Enable)
	{
		/* Generate a stop condition, Enable stop bit */
		I2Cx->CR1 |= (I2C_CR1_STOP);
	}
	else
	{
		/* Disable the stop condition generation, Disable stop bit */
		I2Cx->CR1 &= ~(I2C_CR1_STOP);
	}
}
void I2C_ACKConfig(I2C_t *I2Cx, Functional_State State){
	if(State == Enable)
	{
		/* Enable Automatic ACK */
		I2Cx->CR1 |= (I2C_CR1_ACK);
	}
	else
	{
		/* Disable Automatic ACK */
		I2Cx->CR1 &= ~(I2C_CR1_ACK);
	}
}

I2C_Flagstatus I2C_Get_FlagStatus(I2C_t *I2Cx, Status Flag){
	 uint32_t flag_1 = 0, flag_2 =0, last_event;
	 I2C_Flagstatus bit_Status = RESET;

	 switch(Flag){
	 case BUS_Busy:
		 /*Bit 1 BUSY: Bus busy
0: No communication on the bus
1: Communication ongoing on the bus
– Set by hardware on detection of SDA or SCL low
– cleared by hardware on detection of a Stop condition.
It indicates a communication in progress on the bus. This information is still updated when
the interface is disabled (PE=0)*/
		 if((I2Cx->SR2) & (I2C_SR2_BUSY)){
			 bit_Status = SET;
		 }
		 else{
			 bit_Status = RESET;
		 }
		 break;
	 case SB:
	 	 /*Bit 0 SB: Start bit (Master mode)
0: No Start condition
1: Start condition generated.
– Set when a Start condition generated.
– Cleared by software by reading the SR1 register followed by writing the DR register, or by
hardware when PE=0*/
	 	 if((I2Cx->SR1) & (I2C_SR1_SB)){
	 		 bit_Status = SET;
	 	 }
	 	 else{
		 	 bit_Status = RESET;
	 	 }
	 	 break;
	 case ADDR:
		 /*Bit 1 ADDR: Address sent (master mode)/matched (slave mode)
This bit is cleared by software reading SR1 register followed reading SR2, or by hardware
when PE=0.
Address matched (Slave)
0: Address mismatched or not received.
1: Received address matched.
– Set by hardware as soon as the received slave address matched with the OAR registers
content or a general call or a SMBus Device Default Address or SMBus Host or SMBus Alert
is recognized. (when enabled depending on configuration).
Note: In slave mode, it is recommended to perform the complete clearing sequence (READ
SR1 then READ SR2) after ADDR is set. Refer to Figure 272.
Address sent (Master)
0: No end of address transmission
1: End of address transmission
– For 10-bit addressing, the bit is set after the ACK of the 2nd byte.
– For 7-bit addressing, the bit is set after the ACK of the byte.
Note: ADDR is not set after a NACK reception*/
	 	 if((I2Cx->SR1) & (I2C_SR1_ADDR)){
	 		 bit_Status = SET;
	 	 }
	 	 else{
		 	 bit_Status = RESET;
	 	 }
	 	 break;
	 case TXE:
		 /*Bit 7 TxE: Data register empty (transmitters)
0: Data register not empty
1: Data register empty
– Set when DR is empty in transmission. TxE is not set during address phase.
– Cleared by software writing to the DR register or by hardware after a start or a stop condition
or when PE=0.*/
	 	 if((I2Cx->SR1) & (I2C_SR1_TXE)){
	 		 bit_Status = SET;
	 	 }
	 	 else{
		 	 bit_Status = RESET;
	 	 }
	 	 break;
	 case RXNE:
		 /*Bit 6 RxNE: Data register not empty (receivers)
0: Data register empty
1: Data register not empty
– Set when data register is not empty in receiver mode. RxNE is not set during address phase.
– Cleared by software reading or writing the DR register or by hardware when PE=0.
RxNE is not set in case of ARLO event*/
	 	 if((I2Cx->SR1) & (I2C_SR1_RXNE)){
	 		 bit_Status = SET;
	 	 }
	 	 else{
		 	 bit_Status = RESET;
	 	 }
	 	 break;
	 case Master_Transmitter_Event:
		 flag_1 = Master_Transmitter_Event & (0x0000FFFF);
		 flag_2 = Master_Transmitter_Event >> 16;

		 last_event = ((I2Cx->SR1 &flag_1) | ((I2Cx->SR2 & flag_2) << 16));

		 if(last_event == Flag){
	 		 bit_Status = SET;
		 }
	 	 else{
		 	 bit_Status = RESET;
	 	 }
	 	 break;
	 default:
		 break;
	 }
	 return bit_Status;
}


/* ================================================================ */
/* ======================== Generic =========================== */
/* ================================================================ */


void I2C1_EV_IRQ_HANDLER(void){
	uint32_t Temp1,Temp2,Temp3;

	Temp1 = (I2C1->CR2 & (I2C_CR2_ITEVTEN));
	Temp2 = (I2C1->CR2 & (I2C_CR2_ITBUFEN));
	Temp3 = (I2C1->SR1 & (I2C_SR1_STOPF));

	if(Temp1 && Temp3){
		I2C1->CR1 |= 0x0000;
		Global_I2C_Config[I2C1_Index].P_IRQ_CallBack(I2C_EV_STOP);
	}

	Temp3 = (I2C1->SR1 & (I2C_SR1_ADDR));
	if(Temp1 && Temp3)
	{
		if(I2C1->SR2 & (I2C_SR2_MSL))
		{

		}
		else
		{
			Global_I2C_Config[I2C1_Index].P_IRQ_CallBack(I2C_EV_ADD_MATCHED);
		}

	}


	Temp3 = (I2C1->SR1 & (I2C_SR1_TXE));
	if(Temp1 && Temp2 && Temp3)
	{
		if(I2C1->SR2 & (I2C_SR2_MSL))
		{
		}
		else
		{

			if(I2C1->SR2 & (I2C_SR2_TRA))
			{
				Global_I2C_Config[I2C1_Index].P_IRQ_CallBack(I2C_EV_DATA_REQ);
			}
		}
	}


	Temp3 = (I2C1->SR1 & (I2C_SR1_RXNE));
	if(Temp1 && Temp2 && Temp3)
	{
		if(I2C1->SR2 & (I2C_SR2_MSL))
		{
		}
		else
		{
			if(!(I2C1->SR2 & (I2C_SR2_TRA)))
			{
				Global_I2C_Config[I2C1_Index].P_IRQ_CallBack(I2C_EV_DATA_RCV);
			}
		}
	}}
void I2C1_ER_IRQ_HANDLER(void){

}

void I2C2_EV_IRQ_HANDLER(void){

}
void I2C2_ER_IRQ_HANDLER(void){

}
