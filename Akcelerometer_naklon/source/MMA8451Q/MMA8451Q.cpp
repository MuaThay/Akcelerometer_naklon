/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MMA8451Q.h"
#include "fsl_i2c.h"
#include "board.h"

#define REG_WHO_AM_I      	0x0D 		//	VRACIA ID NEJAKEHO OBVODU

#define REG_OUT_X_MSB     	0x01
#define REG_OUT_Y_MSB     	0x03
#define REG_OUT_Z_MSB     	0x05

#define UINT14_MAX        16383

MMA8451Q::MMA8451Q(unsigned char addr) : m_addr(addr) {
    // activate the peripheral

	I2C_MasterGetDefaultConfig(&masterConfig);
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetBusClkFreq());//I2C0_CLK_SRC
}

MMA8451Q::~MMA8451Q() { }

uint8_t MMA8451Q::getWhoAmI() {
    uint8_t who_am_i = 0;
    readRegs(REG_WHO_AM_I, &who_am_i, 1);
    return who_am_i;
}

float MMA8451Q::getAccX() {
    return (float(getAccAxis(REG_OUT_X_MSB))/4096.0);
}

float MMA8451Q::getAccY() {
    return (float(getAccAxis(REG_OUT_Y_MSB))/4096.0);
}

float MMA8451Q::getAccZ() {
    return (float(getAccAxis(REG_OUT_Z_MSB))/4096.0);
}

void MMA8451Q::getAccAllAxis(float * res) {
    res[0] = getAccX();
    res[1] = getAccY();
    res[2] = getAccZ();
}

int16_t MMA8451Q::getAccAxis(uint8_t addr) {
    int16_t acc;
    uint8_t res[2];
    readRegs(addr, res, 2);

    acc = (res[0] << 6) | (res[1] >> 2);
    if (acc > UINT14_MAX/2)
        acc -= UINT14_MAX;

    return acc;
}

void MMA8451Q::readRegs(int addr, uint8_t * data, int len) { //I2C

	i2c_master_transfer_t masterXfer;
	memset(&masterXfer,0,sizeof(masterXfer));

	masterXfer.slaveAddress = m_addr;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = addr;
	masterXfer.subaddressSize = 1;
	masterXfer.data = data;
	masterXfer.dataSize = len;

	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
}

void MMA8451Q::writeRegs(uint8_t * data, int len) { //I2C
  	i2c_master_transfer_t masterXfer;
	memset(&masterXfer,0,sizeof(masterXfer));

	masterXfer.slaveAddress = m_addr;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = data[0];
	masterXfer.subaddressSize = 1;
	masterXfer.data = &data[1];
	masterXfer.dataSize = len-1;

	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
}
