#include "BDFM_MMPC.h"
#include "math.h"

float  icd_li=0.0, icq_li=0.0;
float i2_alf=0;
float i2_beta=0;

int Num_test = 0;
float wr_MMPC=0.0;
float ud_ref_MMPC=0;
float uq_ref_MMPC=0;

float gm = 123355995955.0;

float gx=0.222222998445;
float gy=0.6565454545554;
float gxy=0;

//modified by lixuan 1101
#define MMPC_Npc 0.7

/*******************the Parameter define-start  for model in 50us*************************/
#define G11_MMPC 0.99908735
float G12_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G13_MMPC -0.00092417992
float G14_MMPC = 0.0;//见data_in_MMPC()中的定义


float G21_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G22_MMPC  0.99908735
float G23_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G24_MMPC -0.00092417992


#define G31_MMPC -0.00084180392
float G32_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G33_MMPC 0.99868955
float G34_MMPC = 0.0;//见data_in_MMPC()中的定义


float G41_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G42_MMPC  -0.00084180392
float G43_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G44_MMPC 0.99868955


#define H11_MMPC 0.0022615636
#define H12_MMPC 0.0
#define H13_MMPC 0.0020859966
#define H14_MMPC 0.0

#define H21_MMPC 0.0
#define H22_MMPC 0.0022615636
#define H23_MMPC 0.0
#define H24_MMPC 0.0020859966

#define H31_MMPC 0.0020859966
#define H32_MMPC 0.0
#define H33_MMPC 0.0029578553
#define H34_MMPC 0.0

#define H41_MMPC 0.0
#define H42_MMPC 0.0020859966
#define H43_MMPC 0.0
#define H44_MMPC 0.0029578553
/*******************the Parameter define-end*************************/

/***************for 250us control********************/
float MMPC_250us_i1d_k_1 = 0.0;
float MMPC_250us_i1q_k_1 = 0.0;
float MMPC_250us_i2d_k_1 = 0.0;
float MMPC_250us_i2q_k_1 = 0.0;

float MMPC_250us_u1d_k_1 = 0.0;
float MMPC_250us_u1q_k_1 = 0.0;

float MMPC_250us_u2d_k = 0.0;
float MMPC_250us_u2q_k = 0.0;

/*******************the Parameter define-start**for 250us control************************/
#define G11_250us_MMPC 0.99543673
float G12_250us_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G13_250us_MMPC  -0.0046208996
float G14_250us_MMPC = 0.0;//见data_in_MMPC()中的定义


float G21_250us_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G22_250us_MMPC  0.99543673
float G23_250us_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G24_250us_MMPC -0.0046208996


#define G31_250us_MMPC -0.0042090196
float G32_250us_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G33_250us_MMPC 0.99344776
float G34_250us_MMPC = 0.0;//见data_in_MMPC()中的定义


float G41_250us_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G42_250us_MMPC  -0.0042090196
float G43_250us_MMPC = 0.0;//见data_in_MMPC()中的定义
#define G44_250us_MMPC  0.99344776


#define H11_250us_MMPC 0.011307818
#define H12_250us_MMPC 0.0
#define H13_250us_MMPC 0.010429983
#define H14_250us_MMPC 0.0

#define H21_250us_MMPC 0.0
#define H22_250us_MMPC 0.011307818
#define H23_250us_MMPC 0.0
#define H24_250us_MMPC 0.010429983

#define H31_250us_MMPC 0.010429983
#define H32_250us_MMPC 0.0
#define H33_250us_MMPC 0.014789276
#define H34_250us_MMPC 0.0

#define H41_250us_MMPC 0.0
#define H42_250us_MMPC 0.010429983
#define H43_250us_MMPC 0.0
#define H44_250us_MMPC 0.014789276
/*******************the Parameter define-end*************************/
/*******************************************/

/*******************the state variables at the time of  k define-start*************************/
float MMPC_i1d_k = 0.0;
float MMPC_i1q_k = 0.0;
float MMPC_i2d_k = 0.0;
float MMPC_i2q_k = 0.0;

float MMPC_u1d_k = 0.0;
float MMPC_u1q_k = 0.0;
float MMPC_u2d_k = 0.0;
float MMPC_u2q_k = 0.0;
static float MMPC_udc = 0.0;
/*******************the state variables at the time of  k define-end*************************/ 


/*******************the state variables at the time of  k+1 define-start*************************/
float MMPC_i1d_k_1 = 0.0;
float MMPC_i1q_k_1 = 0.0;
float MMPC_i2d_k_1 = 0.0;
float MMPC_i2q_k_1 = 0.0;

float MMPC_u1d_k_1 = 0.0;
float MMPC_u1q_k_1 = 0.0;
float MMPC_u2d_k_1[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
float MMPC_u2q_k_1[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
/*******************the state variables at the time of  k+1 define-end*************************/

int veci[6][2] = {{1,2},{2,3},{3,4},{4,5},{5,6},{6,1}};
float temp_MMPC=0.0;
float MMPC_theta_C=0.0;


/*******************the reference value at the time of  k+2 define-start*************************/
float MMPC_i2d_ref=0.0;
float MMPC_i2q_ref=0.0;
/*******************the reference value at the time of  k+2 define-end*************************/


/*******************cost function variable define-start*************************/
float gi0=0.0;
float gi1=0.0;
float gi2=0.0;
float gi=0.0;
float gimin=0.0;
float gitemp=0.0;
float Duty_Vector1=0.0;
float Duty_Vector2=0.0;
int Num_Vector1=0;
int Num_Vector2=0;
float gi_num1=0;
float gi_num2=0;
float gi_num0=0;
int MMPC_i=0;
float MMPC_Error_i2d_k_2_Vector0 = 0.0;
float MMPC_Error_i2q_k_2_Vector0 = 0.0;
float MMPC_Error_i2d_k_2_Vector1 = 0.0;
float MMPC_Error_i2q_k_2_Vector1 = 0.0;
float MMPC_Error_i2d_k_2_Vector2 = 0.0;
float MMPC_Error_i2q_k_2_Vector2 = 0.0;
float MMPC_u2d_k_1_optimal = 0.0;
float MMPC_u2q_k_1_optimal = 0.0;
/*******************cost function variable define-end*************************/ 
float	cos_a_MMPC = 0.0;
float	cos_b_MMPC = 0.0;
float	cos_c_MMPC = 0.0;

float	sin_a_MMPC = 0.0;
float	sin_b_MMPC = 0.0;
float	sin_c_MMPC = 0.0;
//for test
float test_gi[6]={0.0,0.0,0.0,0.0,0.0,0.0};

void data_in_MMPC()
{


	MMPC_i1d_k = idq_in.d;
	MMPC_i1q_k = idq_in.q;
	MMPC_i2d_k = idq_out.d;
	MMPC_i2q_k = idq_out.q;

	MMPC_u1d_k = -vp_in_dq.q;
	MMPC_u1q_k = vp_in_dq.d;
	MMPC_theta_C = field_oriented_theta;
	MMPC_udc = dc_voltage;

//modified by lixuan 1101
//	MMPC_i2d_ref= id_ref;
//	MMPC_i2q_ref= iq_ref;

	MMPC_i2d_ref= id_ref;
	MMPC_i2q_ref= iq_ref;

	wr_MMPC= wr;




    /*********the state variables at the time of  k sample-end***************/


    /*********the state variables at the time of  k sample-start for test***************/

//	MMPC_u2d_k = 100;
//	MMPC_u2q_k = 100;
//	wr_MMPC = 80;
//	MMPC_i1d_k = 13;
//	MMPC_i1q_k = 8;
//
//	MMPC_i2d_k = 6;
//	MMPC_i2q_k = 9;
//
//	MMPC_u1d_k = 0;
//	MMPC_u1q_k = 311;
//	MMPC_theta_C = 3.14/6;
//	MMPC_udc = 600;
//	MMPC_i2d_ref= 20;
//	MMPC_i2q_ref= -10;
}

void MMPC_parameter_50us()

{
    /*********the state variables at the time of  k sample-end  for test***************/


    /*********parameter calculation-start***************/

	G12_MMPC = 0.00037223174 * wr_MMPC +  0.015707963;
	G14_MMPC = -0.00040356046 * wr_MMPC;
	G21_MMPC = 	-G12_MMPC;
	G23_MMPC =  -G14_MMPC;

	G32_MMPC = 0.00052780894 * wr_MMPC;
	G34_MMPC = 0.015707963 - 0.00057223174 * wr_MMPC;
	G41_MMPC = 	-G32_MMPC;
	G43_MMPC =  -G34_MMPC;
}

void MMPC_parameter_250us()
{
	/*********parameter calculation-start***************/
	G12_250us_MMPC = 0.0018611587 * wr_MMPC +  0.078539816;
	G14_250us_MMPC = - 0.0020178023 * wr_MMPC;
	G21_250us_MMPC = 	-G12_250us_MMPC;
	G23_250us_MMPC =  -G14_250us_MMPC;

	G32_250us_MMPC = 0.0026390447 * wr_MMPC;
	G34_250us_MMPC = 0.078539816 - 0.0028611587 * wr_MMPC;
	G41_250us_MMPC = 	-G32_250us_MMPC;
	G43_250us_MMPC =  -G34_250us_MMPC;

//modified by lixuan 1101
//	temp_MMPC =  0.6666666667 * MMPC_udc;
    temp_MMPC =  0.6666666667 * MMPC_udc;

	cos_a_MMPC = cos(MMPC_theta_C);
	cos_b_MMPC = cos(MMPC_theta_C - 2.094395102393195);
	cos_c_MMPC = cos(MMPC_theta_C + 2.094395102393195);

	sin_a_MMPC = sin(MMPC_theta_C);
	sin_b_MMPC = sin(MMPC_theta_C - 2.094395102393195);
	sin_c_MMPC = sin(MMPC_theta_C + 2.094395102393195);

    //The d-axis,q-axis of Vector1 = [1 0 0];
    MMPC_u2d_k_1[1] = temp_MMPC * ( cos_a_MMPC   );
	MMPC_u2q_k_1[1] = temp_MMPC * ( -sin_a_MMPC  );
    //The d-axis,q-axis of Vector2 =[1 1 0];
    MMPC_u2d_k_1[2] = temp_MMPC * (cos_a_MMPC + cos_b_MMPC );
	MMPC_u2q_k_1[2] = temp_MMPC * ( -sin_a_MMPC - sin_b_MMPC  );
    //The d-axis,q-axis of Vector3 =[0 1 0];
    MMPC_u2d_k_1[3] = temp_MMPC * (   cos_b_MMPC );
	MMPC_u2q_k_1[3] = temp_MMPC * (  -sin_b_MMPC  );
    //The d-axis,q-axis of Vector4 = [0 1 1];
    MMPC_u2d_k_1[4] = temp_MMPC * (  cos_b_MMPC + cos_c_MMPC);
	MMPC_u2q_k_1[4] = temp_MMPC * (  -sin_b_MMPC - sin_c_MMPC );
    //The d-axis,q-axis of Vector5 = [0 0 1];
    MMPC_u2d_k_1[5] = temp_MMPC * (  cos_c_MMPC );
	MMPC_u2q_k_1[5] = temp_MMPC * ( - sin_c_MMPC );
    //The d-axis,q-axis of Vector6 =  [1 0 1];
	MMPC_u2d_k_1[6] = temp_MMPC * ( cos_a_MMPC  + cos_c_MMPC );
	MMPC_u2q_k_1[6] = temp_MMPC * ( -sin_a_MMPC - sin_c_MMPC );

   /*********parameter calculation-end***************/
}


void Model_Validate_50us()
{

	/*******************the state variables at the time of  k+1 -start*************************/

	//modified by lixuan 1101
//	MMPC_u2d_k = ud_ref;
//	MMPC_u2q_k = uq_ref;
	MMPC_u2d_k = ud_ref;
	MMPC_u2q_k = uq_ref;



	temp_MMPC = G11_MMPC * MMPC_i1d_k + G12_MMPC * MMPC_i1q_k + G13_MMPC * MMPC_i2d_k + G14_MMPC * MMPC_i2q_k; 
    MMPC_i1d_k_1 = temp_MMPC + H11_MMPC * MMPC_u1d_k  + H13_MMPC * MMPC_u2d_k; 

	temp_MMPC = G21_MMPC * MMPC_i1d_k + G22_MMPC * MMPC_i1q_k + G23_MMPC * MMPC_i2d_k + G24_MMPC * MMPC_i2q_k; 
    MMPC_i1q_k_1 = temp_MMPC + H22_MMPC * MMPC_u1q_k  + H24_MMPC * MMPC_u2q_k;
    
    temp_MMPC = G31_MMPC * MMPC_i1d_k + G32_MMPC * MMPC_i1q_k + G33_MMPC * MMPC_i2d_k + G34_MMPC * MMPC_i2q_k; 
    MMPC_i2d_k_1 = temp_MMPC + H31_MMPC * MMPC_u1d_k  + H33_MMPC * MMPC_u2d_k; 

	temp_MMPC = G41_MMPC * MMPC_i1d_k + G42_MMPC * MMPC_i1q_k + G43_MMPC * MMPC_i2d_k + G44_MMPC * MMPC_i2q_k; 
    MMPC_i2q_k_1 = temp_MMPC + H42_MMPC * MMPC_u1q_k  + H44_MMPC * MMPC_u2q_k; 
    
	MMPC_u1d_k_1 = MMPC_u1d_k; 
	MMPC_u1q_k_1 = MMPC_u1q_k; 
	/*******************the state variables at the time of  k+1 -end*************************/ 

}



void MMPC_Control_Method()
{
    //采用预测控制的情况下，当前的控制侧d轴和q轴电压对应值。

	//modified by lixuan 0709
//	MMPC_250us_u2d_k = ud_ref_MMPC;   //_r2
//	MMPC_250us_u2q_k = uq_ref_MMPC;   //_r2
//
	MMPC_250us_u2d_k = ud_ref;   //_r2
	MMPC_250us_u2q_k = uq_ref;
//	MMPC_u2d_k = MMPC_u2d_k_1_optimal;
//	MMPC_u2q_k = MMPC_u2q_k_1_optimal;

	/*******************the state variables at the time of  k+1 -start*************************/
//	temp_MMPC = G11_250us_MMPC * MMPC_i1d_k + G12_250us_MMPC * MMPC_i1q_k + G13_250us_MMPC * MMPC_i2d_k + G14_250us_MMPC * MMPC_i2q_k;
//    MMPC_250us_i1d_k_1 = temp_MMPC + H11_250us_MMPC * MMPC_u1d_k  + H13_250us_MMPC * MMPC_250us_u2d_k;
//
//	temp_MMPC = G21_250us_MMPC * MMPC_i1d_k + G22_250us_MMPC * MMPC_i1q_k + G23_250us_MMPC * MMPC_i2d_k + G24_250us_MMPC * MMPC_i2q_k;
//    MMPC_250us_i1q_k_1 = temp_MMPC + H22_250us_MMPC * MMPC_u1q_k  + H24_250us_MMPC * MMPC_250us_u2q_k;
//
//    temp_MMPC = G31_250us_MMPC * MMPC_i1d_k + G32_250us_MMPC * MMPC_i1q_k + G33_250us_MMPC * MMPC_i2d_k + G34_250us_MMPC * MMPC_i2q_k;
//    MMPC_250us_i2d_k_1 = temp_MMPC + H31_250us_MMPC * MMPC_u1d_k  + H33_250us_MMPC * MMPC_250us_u2d_k;
//
//	temp_MMPC = G41_250us_MMPC * MMPC_i1d_k + G42_250us_MMPC * MMPC_i1q_k + G43_250us_MMPC * MMPC_i2d_k + G44_250us_MMPC * MMPC_i2q_k;
//    MMPC_250us_i2q_k_1 = temp_MMPC + H42_250us_MMPC * MMPC_u1q_k  + H44_250us_MMPC * MMPC_250us_u2q_k;
//
    MMPC_250us_i1d_k_1=MMPC_i1d_k;
    MMPC_250us_i1q_k_1=MMPC_i1q_k;
   	MMPC_250us_i2d_k_1=MMPC_i2d_k;
   	MMPC_250us_i2q_k_1=MMPC_i2q_k;

	MMPC_250us_u1d_k_1 = MMPC_u1d_k;
	MMPC_250us_u1q_k_1 = MMPC_u1q_k;
	/*******************the state variables at the time of  k+1 -end*************************/


	gimin = 10000000000000000000.0;
    


	//Prediction for the zero voltage vector
	temp_MMPC = G31_250us_MMPC * MMPC_250us_i1d_k_1 + G32_250us_MMPC * MMPC_250us_i1q_k_1 + G33_250us_MMPC * MMPC_250us_i2d_k_1 + G34_250us_MMPC * MMPC_250us_i2q_k_1 + H31_250us_MMPC * MMPC_250us_u1d_k_1;
    MMPC_Error_i2d_k_2_Vector0 = temp_MMPC - MMPC_i2d_ref;

	temp_MMPC = G41_250us_MMPC * MMPC_250us_i1d_k_1 + G42_250us_MMPC * MMPC_250us_i1q_k_1 + G43_250us_MMPC * MMPC_250us_i2d_k_1 + G44_250us_MMPC * MMPC_250us_i2q_k_1 + H42_250us_MMPC * MMPC_250us_u1q_k_1 ;
    MMPC_Error_i2q_k_2_Vector0 = temp_MMPC - MMPC_i2q_ref;

    gi0 = MMPC_Error_i2d_k_2_Vector0 * MMPC_Error_i2d_k_2_Vector0 + MMPC_Error_i2q_k_2_Vector0 * MMPC_Error_i2q_k_2_Vector0;

	for(MMPC_i=0;MMPC_i<6;MMPC_i++)
    {

		//First Vector Prediction
		MMPC_Error_i2d_k_2_Vector1 = MMPC_Error_i2d_k_2_Vector0  + H33_MMPC * MMPC_u2d_k_1[veci[MMPC_i][0]];
		MMPC_Error_i2q_k_2_Vector1 = MMPC_Error_i2q_k_2_Vector0  + H44_MMPC * MMPC_u2q_k_1[veci[MMPC_i][0]];
//		temp_MMPC= (MMPC_Error_i2d_k_2_Vector1+ MMPC_i2d_ref)*(MMPC_Error_i2d_k_2_Vector1+ MMPC_i2d_ref)+(MMPC_Error_i2q_k_2_Vector1+MMPC_i2q_ref)*(MMPC_Error_i2q_k_2_Vector1+MMPC_i2q_ref);
//		if(temp_MMPC>140)
//			gi1=100000000;
//		else
			gi1 = (MMPC_Error_i2d_k_2_Vector1 * MMPC_Error_i2d_k_2_Vector1 + MMPC_Error_i2q_k_2_Vector1 * MMPC_Error_i2q_k_2_Vector1);



		//Second Vector Prediction
		MMPC_Error_i2d_k_2_Vector2 = MMPC_Error_i2d_k_2_Vector0  + H33_MMPC * MMPC_u2d_k_1[veci[MMPC_i][1]];
		MMPC_Error_i2q_k_2_Vector2 = MMPC_Error_i2q_k_2_Vector0  + H44_MMPC * MMPC_u2q_k_1[veci[MMPC_i][1]];
//		temp_MMPC= (MMPC_Error_i2d_k_2_Vector1+ MMPC_i2d_ref)*(MMPC_Error_i2d_k_2_Vector1+ MMPC_i2d_ref)+(MMPC_Error_i2q_k_2_Vector1+MMPC_i2q_ref)*(MMPC_Error_i2q_k_2_Vector1+MMPC_i2q_ref);
//		if(temp_MMPC>140)
//			gi2=100000000;
//		else
			gi2 = (MMPC_Error_i2d_k_2_Vector2 * MMPC_Error_i2d_k_2_Vector2 + MMPC_Error_i2q_k_2_Vector2 * MMPC_Error_i2q_k_2_Vector2);



		//Cost Function Calculation
		gitemp=1/(gi1 * gi2 + gi0 * gi2 + gi0 * gi1);
		gi = gi0 * gi1 * gi2 * gitemp;



		if(gi<gimin)
		{
			gi_num1=gi1;
			gi_num2=gi2;
			gi_num0=gi0;


			gimin  = gi;
			Duty_Vector1=gi0*gi2*gitemp;
			Duty_Vector2=gi0*gi1*gitemp;
			Num_Vector1 = veci[MMPC_i][0];
			Num_Vector2 = veci[MMPC_i][1];
		}

		test_gi[MMPC_i]=gi; // for test
	}

	//output voltage vector
    MMPC_u2d_k_1_optimal = Duty_Vector1 * MMPC_u2d_k_1[Num_Vector1] + Duty_Vector2 * MMPC_u2d_k_1[Num_Vector2];
	MMPC_u2q_k_1_optimal = Duty_Vector1 * MMPC_u2q_k_1[Num_Vector1] + Duty_Vector2 * MMPC_u2q_k_1[Num_Vector2];

}
void motor_model_simulation()
{
	 /********************************电机模型预测---电机转速和角度预测*************************************/
    	wr_ref=62.83;//机械角速度=转速/60*2pi

        theta_r =4*wr*ts*0.2+theta_rb;
		if(theta_r>6.283185307){	theta_r=theta_r-6.283185307;}
		if(theta_r<0.0){	theta_r=theta_r+6.283185307;}
		theta_rb = theta_r;


		toque_li=(1/kkk_te)*MMPC_i2q_k;
		wr=(toque_li-TL)/J1*ts*0.2+wrb;
		wrb=wr;
     	/*************************************************-end*************************/

}

//dq dq_alfbeta(float a,float b)
//{
//	dq dq1;
//	dq1.d = cos(field_oriented_theta)*a-sin(field_oriented_theta)*b;
//	dq1.q = sin(field_oriented_theta)*a+cos(field_oriented_theta)*b;
//	return dq1;
//}

void MMPC_duty()//占空比计算
{

/*************************************************/
/*  modified by hanbing dan*/
	Num_test ++;
	if(Num_test==5)
		Num_test = 0;

/******************************************** *****/

	 speed_control_para_init();  //控制器参数初始化
	 data_transform();    //通过电机模型得到采样值
	 compute_angle();     //通过电机模型得到角度的采样值

    if(Num_test == 0)
     {
    	 compute_toque_ref();  //转速环
    	 compute_id_iq_ref();  //控制目标给定
     }



 /*********start *********/
 // 电机预测控制算法用数据输入   其中包含电流期望值idref和iqref的给定，因此一定要放在速度环的后面
	 data_in_MMPC();
 /*********end *********/

//	 /********************************预测预测---电机控制*************************************/
//	     if(Num_test == 0)
//	     {
//	    	 MMPC_parameter_250us();
//	    	 MMPC_Control_Method();  //预测控制算法
//
//	    	 compute_ud_uq_ref();  //PI控制算法   把上面两行注释，这一行打开就是PI控制算法
//	//
//	    	 icd_li=MMPC_i2d_k_1;//rli 观测250us的电流
//	    	 icq_li=MMPC_i2q_k_1;
//	     }
//	   	/*************************************************-end*************************/

/********************************电机模型预测---定子功率和控制绕组电流预测*************************************/
	 MMPC_parameter_50us();
	 Model_Validate_50us();
   	/*************************************************-end*************************/


 /********************************预测预测---电机控制*************************************/
     if(Num_test == 0)
     {
    	 MMPC_parameter_250us();
    	 MMPC_Control_Method();  //预测控制算法

    	 compute_ud_uq_ref();  //PI控制算法   把上面两行注释，这一行打开就是PI控制算法
//
    	 icd_li=MMPC_i2d_k_1;//rli 观测250us的电流
    	 icq_li=MMPC_i2q_k_1;
     }
   	/*************************************************-end*************************/
     if(Num_test == 4)
     {
       ud_ref_MMPC=MMPC_u2d_k_1_optimal; //预测控制算法的输出赋值
	   uq_ref_MMPC=MMPC_u2q_k_1_optimal;//预测控制算法的输出赋

//	   ud_ref=MMPC_u2d_k_1_optimal; //预测控制算法的输出赋值
//	   uq_ref=MMPC_u2q_k_1_optimal;//预测控制算法的输出赋值
	   ud_ref=ud_ref_PI; //PI控制算法的输出赋值  把上面两行注释，这两行打开就是PI控制算法
	   uq_ref=uq_ref_PI; //PI控制算法的输出赋值
     }
     motor_model_simulation();

     i2_alf = cos(field_oriented_theta)*idq_out.d-sin(field_oriented_theta)*idq_out.q;
     i2_beta = sin(field_oriented_theta)*idq_out.d+cos(field_oriented_theta)*idq_out.q;
     gxy=gx*gy;

}



