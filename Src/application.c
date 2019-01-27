#include "stm32f4xx_hal.h"
#include "main.h"
#include "spi.h"
#include "usart.h"
//#include "rtc.h"


#define QUAN_SPI_BYTES 8

#define LIS3DSH_READ							0x80U
#define LIS3DSH_WRITE							0x00U

#define LIS3DSH_MULTIPLE					0x40U
#define LIS3DSH_SINGLE						0x00U

#define LIS3DSH_INFO1							0x0DU
#define LIS3DSH_INFO2							0x0EU
#define LIS3DSH_WHOAMI						0x0FU
#define LIS3DSH_TEMPERATURE				0x0CU

#define LIS3DSH_X_L								0x28U
#define LIS3DSH_X_H								0x29U
#define LIS3DSH_Y_L								0x2AU
#define LIS3DSH_Y_H								0x2BU
#define LIS3DSH_Z_L								0x2CU
#define LIS3DSH_Z_H								0x2DU

#define LIS3DSH_CTRL_REG3					0x23U
#define LIS3DSH_CTRL_REG3_DR_EN			0x80U
#define LIS3DSH_CTRL_REG3_IEA				0x40U
#define LIS3DSH_CTRL_REG3_IEL				0x20U
#define LIS3DSH_CTRL_REG3_INT2_EN		0x10U
#define LIS3DSH_CTRL_REG3_INT1_EN		0x08U
#define LIS3DSH_CTRL_REG3_VFILT			0x04U
#define LIS3DSH_CTRL_REG3_STRT			0x01U

#define LIS3DSH_CTRL_REG4					0x20U
#define LIS3DSH_CTRL_REG4_ODR_SHIFT		4
#define LIS3DSH_CTRL_REG4_ODR0					0
#define LIS3DSH_CTRL_REG4_ODR3_125			1
#define LIS3DSH_CTRL_REG4_ODR6_25				2
#define LIS3DSH_CTRL_REG4_ODR12_5				3
#define LIS3DSH_CTRL_REG4_ODR25					4
#define LIS3DSH_CTRL_REG4_ODR50					5
#define LIS3DSH_CTRL_REG4_ODR100				6
#define LIS3DSH_CTRL_REG4_ODR400				7
#define LIS3DSH_CTRL_REG4_ODR800				8
#define LIS3DSH_CTRL_REG4_ODR1600				9
#define LIS3DSH_CTRL_REG4_BDU				0x08U
#define LIS3DSH_CTRL_REG4_Z_EN			0x04U
#define LIS3DSH_CTRL_REG4_Y_EN			0x02U
#define LIS3DSH_CTRL_REG4_X_EN			0x01U

#define LIS3DSH_CTRL_REG5							0x24U
#define LIS3DSH_CTRL_REG5_BW_SHIFT			6
#define LIS3DSH_CTRL_REG5_BW_800HZ				0x00U
#define LIS3DSH_CTRL_REG5_BW_200HZ				0x01U
#define LIS3DSH_CTRL_REG5_BW_400HZ				0x02U
#define LIS3DSH_CTRL_REG5_BW_50HZ					0x03U
#define LIS3DSH_CTRL_REG5_FSCALE_SHIFT	3
#define LIS3DSH_CTRL_REG5_FSCALE_2G				0x00U
#define LIS3DSH_CTRL_REG5_FSCALE_4G				0x01U
#define LIS3DSH_CTRL_REG5_FSCALE_6G				0x02U
#define LIS3DSH_CTRL_REG5_FSCALE_8G				0x03U
#define LIS3DSH_CTRL_REG5_FSCALE_16G			0x04U
#define LIS3DSH_CTRL_REG5_ST_SHIFT			1
#define LIS3DSH_CTRL_REG5_ST_OFF					0x00U
#define LIS3DSH_CTRL_REG5_SIM_3WIRE				0x01U
#define LIS3DSH_CTRL_REG5_SIM_4WIRE				0x00U

#define LIS3DSH_CTRL_REG6							0x25U
#define LIS3DSH_CTRL_REG6_BOOT					0x80U
#define LIS3DSH_CTRL_REG6_FIFO_EN				0x40U
#define LIS3DSH_CTRL_REG6_WTM_EN				0x20U
#define LIS3DSH_CTRL_REG6_ADD_INC				0x10U
#define LIS3DSH_CTRL_REG6_P1_EMPTY			0x08U
#define LIS3DSH_CTRL_REG6_P1_WTM				0x04U
#define LIS3DSH_CTRL_REG6_P1_OVR				0x02U
#define LIS3DSH_CTRL_REG6_P2_BOOT				0x01U

#define	DUMMY													0xFFU

void SPISend(uint8_t* transmit, uint8_t* receive, uint16_t length);

void Application(void) {
/**
 * \brief		Application - A demonstration of reading the accelerometer.
 * \return	void
 * \param[in]	void - not used
 * \author	Andrei Chichak
 * \date		Mar 28, 2017
 * \details This routine is called from main, just before Cube’s while(1) loop.
 *          To show the use of the SPI bus, we are communicating with the LIS3DSH
 *         accelerometer.
 */

	uint8_t transmitData[QUAN_SPI_BYTES];
	uint8_t receiveData[QUAN_SPI_BYTES];

	int16_t x, y, z, temperature;
	uint8_t xl, xh, yl, yh, zl, zh;
	uint8_t whoami;

/*
 * First we have to configure the accelerometer chip
 */
	transmitData[0] = LIS3DSH_WRITE | LIS3DSH_CTRL_REG6;
	transmitData[1] = LIS3DSH_CTRL_REG6_BOOT | LIS3DSH_CTRL_REG6_ADD_INC;

	SPISend( transmitData, receiveData, 2);

	printf("%x\n", transmitData[0]);

	transmitData[0] = LIS3DSH_WRITE | LIS3DSH_CTRL_REG4;
	transmitData[1] = (LIS3DSH_CTRL_REG4_ODR400 << LIS3DSH_CTRL_REG4_ODR_SHIFT)
			| LIS3DSH_CTRL_REG4_BDU
			| LIS3DSH_CTRL_REG4_Z_EN
			| LIS3DSH_CTRL_REG4_Y_EN
			| LIS3DSH_CTRL_REG4_X_EN;

	SPISend( transmitData, receiveData, 2);


	transmitData[0] = LIS3DSH_WRITE | LIS3DSH_CTRL_REG5;
	transmitData[1] = (LIS3DSH_CTRL_REG5_BW_800HZ << LIS3DSH_CTRL_REG5_BW_SHIFT)
			| (LIS3DSH_CTRL_REG5_FSCALE_2G << LIS3DSH_CTRL_REG5_FSCALE_SHIFT)
			| (LIS3DSH_CTRL_REG5_ST_OFF << LIS3DSH_CTRL_REG5_ST_SHIFT)
			| LIS3DSH_CTRL_REG5_SIM_4WIRE;

	SPISend( transmitData, receiveData, 2);

// who am i read
	  transmitData[0] = (uint8_t) (LIS3DSH_READ |  LIS3DSH_WHOAMI);
	  SPISend( transmitData, receiveData, 2);
	  printf("transdata[0] = %x\n", transmitData[0]);
	  printf("transdata[1] = %x\n", transmitData[1]);
	  printf("who am i before = %x\n", whoami);
	  whoami = receiveData[1];
	  printf("who am i = %x\n", whoami);
	  printf("recvdata[0] = %x\n", receiveData[0]);
	  printf("recvdata[1] = %x\n", receiveData[1]);
	  printf("---xxxx--\n");

	//FOREVER {
	while(1){

	/*
	 * Read the X, Y, and Z, low and high data bytes, and reassemble them
	 * into 16 bit unsigned values. This is the single byte per reading solution
	 * see the blog post for the multi-byte code snippet.
	 */
		  transmitData[1] = (uint8_t) DUMMY;

		  transmitData[0] = (uint8_t) (LIS3DSH_READ |  LIS3DSH_X_L);
		  SPISend( transmitData, receiveData, 2);
		  xl = receiveData[1];
		  //printf("xl=%d\n",xl);


		  transmitData[0] = (uint8_t) (LIS3DSH_READ |  LIS3DSH_X_H);
		  SPISend( transmitData, receiveData, 2);
		  xh = receiveData[1];

		  transmitData[0] = (uint8_t) (LIS3DSH_READ |  LIS3DSH_Y_L);
		  SPISend( transmitData, receiveData, 2);
		  yl = receiveData[1];
		  transmitData[0] = (uint8_t) (LIS3DSH_READ |  LIS3DSH_Y_H);
		  SPISend( transmitData, receiveData, 2);
		  yh = receiveData[1];

		  transmitData[0] = (uint8_t) (LIS3DSH_READ |  LIS3DSH_Z_L);
		  SPISend( transmitData, receiveData, 2);
		  zl = receiveData[1];
		  transmitData[0] = (uint8_t) (LIS3DSH_READ |  LIS3DSH_Z_H);
		  SPISend( transmitData, receiveData, 2);
		  zh = receiveData[1];

		  x = (xh << 8) | xl;
		  y = (yh << 8) | yl;
		  z = (zh << 8) | zl;

		/* Read the temperature from the chip as well. This value is
		 * expressed as a signed 8-bit displacement from 25 degrees celcius.
		 */
		  transmitData[0] = (uint8_t) (LIS3DSH_READ | LIS3DSH_TEMPERATURE);
		  SPISend( transmitData, receiveData, 2);
		  temperature = 25 + (int8_t) receiveData[1];

		/*
		 * print out the X, Y, and Z accelerations, and the temperature.
		 */
		  printf("X %5d     Y %5d       Z %5d    T %5d               \r", x, y, z, temperature);
		  //%5d

		  //char buf[4];
		  int16_t buf[1];
		  //sprintf(buf, "%2d\n", z/1024);
		  //sprintf(buf, "%2d\n", 99);
		  buf[0] = x;
		  HAL_UART_Transmit(&huart2, (uint8_t *)buf, sizeof(buf), 0xFFFF);

		  HAL_Delay(100); //ms

	}
}

void SPISend(uint8_t* transmit, uint8_t* receive, uint16_t length) {
/**
 * \brief		SPISend - Take care of a complete SPI transaction
 * \return		void
 * \param[in]	transmit - char* - a pointer to the transmit buffer
 * \param[out] receive - char* - a pointer to the holding buffer for the recieved data.
 * \param[in] length - uint16_t - the number of bytes to transfer.
 * \author		Andrei Chichak
 * \date		Mar 28, 2017
 * \details  Reasonably straight forward, assert select, send the SPI transaction, deassert select
 */
	HAL_StatusTypeDef status;

	HAL_GPIO_WritePin( CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

	  status = HAL_SPI_TransmitReceive( &hspi1, transmit, receive, length, 100);
	  //printf("status = %d\n", status);

	  HAL_GPIO_WritePin( CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_SET);

	  if (status != HAL_OK) {
		  printf("There is a problem with the transmit %d\r\n", status);
		  while(1);
	  }
}
#if 0
int _read(int file, char *result, size_t len) {
    HAL_StatusTypeDef status;
    int retcode = 0;

    if (len != 0) {
        status = HAL_UART_Receive( &huart3, (uint8_t *) result, len, HAL_MAX_DELAY);

        if (status == HAL_OK) {
            retcode = len;
        } else {
            retcode = -1;
        }
    }
    return( retcode);
}

int _write(int file, char *ptr, int len) {
	HAL_UART_Transmit(&huart3, (uint8_t*) ptr, len, 100);

	return len;
}
#endif
