//########################################################################//
//----File name:DC3LMC_Unbalance
//----Description:Three-Level Diode-Clamped Matrix Converter under unbalanced input voltages
//----Version:v1.0 
//----Author:Zhao Ziyi,PE lab,School of Information Science and Engineering,CSU
//----Email:304063906@qq.com
//----Release Date:2017-03-31
//########################################################################//
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "BDFIM.h"
#include "compute_duty.h"
#include "BDFM_MMPC.h"
#include "math.h"
#include "C28x_FPU_FastRTS.h"

float buffer0[400]={0},buffer1[400]={0};
float buffer2[400]={0},buffer3[400]={0};
float buffer4[400]={0},buffer5[400]={0};
float buffer6[400]={0};
//float buffer7[400]={0},buffer8[400]={0};
//##########################################################//
//---function:main
//##########################################################//
void main(void)
{
	control_relay(0,OFF);
	control_LED(0,OFF);

	initialization();

	CpuTimer0Regs.TCR.bit.TSS = 0;	//start timer0;

	for(;;)
	{
		//monitor();
	}
}

//##########################################################//
//---function:timer0 ISR
//##########################################################//
interrupt void timer0_isr(void)
{
	static unsigned int light_count = 0;
	static unsigned int buff_count = 0;

	MMPC_duty();

//	compute_duty();

//	buffer0[buff_count]=MMPC_u1d_k;
//	buffer1[buff_count]=MMPC_u1q_k;
//	buffer0[buff_count]=theta_r;
	buffer0[buff_count]=i2_alf;
	buffer1[buff_count]=i2_beta;

//modified by lixuan 1101
//	buffer0[buff_count]=idq_out.d;
//	buffer1[buff_count]=idq_out.q;
//
//	buffer0[buff_count]=gi_num1;
//	buffer1[buff_count]=gi_num2;
//	buffer4[buff_count]=gi_num0;
//	buffer0[buff_count]=MMPC_Error_i2d_k_2_Vector0;
//	buffer1[buff_count]=MMPC_Error_i2q_k_2_Vector0;
//
//
	buffer2[buff_count]=ud_ref_PI;
	buffer3[buff_count]=uq_ref_PI;
	buffer4[buff_count]=wr;
	buffer5[buff_count]=ud_ref_MMPC;
	buffer6[buff_count]=uq_ref_MMPC;

//	buffer5[buff_count]=ud_alf;
//	buffer6[buff_count]=uq_beta;

	if(buff_count>=400)
		buff_count=0;
	else
		buff_count++;

	if(light_count == 0)
	{
		control_LED(2,ON);
		light_count = 1;
	}
	else if(light_count == 500)
	{
		control_LED(2,OFF);
		light_count = 501;
	}
	else if(light_count == 1000)
	{
		light_count = 0;
	}
	else
	{
		light_count++;
	}

	CpuTimer0Regs.TCR.bit.TIF = 1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
