#include <avr/interrupt.h>
#include <avr/io.h>
#include <compat/twi.h>
#define F_CPU  1000000UL
#include <util/delay.h>
#include <stdint.h>


void ma1(int);//  get data from i2c
void ma2(int);// processes the data

int regn=1233;

#define MAX_TRIES 50
#define adress_ 0x60
#define EEPROM_ID    0xA0        // I2C 24AA128 EEPROM Device Identifier
#define EEPROM_ADDR  0x60        // I2C 24AA128 EEPROM Device Address
#define I2C_START 0
#define I2C_DATA  1
#define I2C_STOP  2
 
 int temp[12]={2,1,23,12,0,88,23,0,7,24,63,12};//variable for temprature
 int pres[12]={3,4,37,34,99,0,0,34,24,7,49,76};//variable for pressure


// get data 
 long flag0=7U;//  main variable for  counting in inactive mode
 int flag1=1;  // count for the no of months
 
 
 //transmit data
 int flag2=0;  //  8 limit var
 unsigned char arr_dat[12];// stores the buffer in transmit
 unsigned char prot[8];   // stores the adc values
 unsigned char prot1[8];   // stores the adc values
 unsigned char prot2[8];   // stores the adc values
 unsigned char prot3[8];   // stores the adc values
int counter=0;
int counter1=0;
int counter2=0;
int counter3=0;
int c_f=0;
 int flag3=1; //time out counter

  int state0=0;
  int state1=0;
  int state2=0;
  
  

#define I2C_SPEED 200e3
#define START 0x08
#define REP_START 0x10
#define SLA_W_ACK 0x18
#define SLA_W_NACK 0x20
#define DATA_W_ACK 0x28
#define DATA_W_NACK 0x30
#define SLA_R_ACK 0x40
#define SLA_R_NACK 0x48
#define DATA_R_ACK 0x50
#define DATA_R_NACK 0x58
#define ARB_LOST 0x38
#define CPU_CLOCK    1105920L

#define FALSE     0
#define TRUE    (!FALSE)

void i2c_init ()
{
    TWSR=0x01;                            //prescaler 4
    TWBR=4;        //200kHz
    //(MCU_CLK/I2C_SPEED-16)/8;        //set bitrate, max 800kbit with this prescaler setting
}

unsigned char i2c_receive (unsigned char adress, unsigned char subadress, unsigned char* data)
{    
    unsigned char error;
    
    TWCR=0xA4;                            //send start condition
    while (!(TWCR&0x80));                    //wait for int_flag
    error=((TWSR&0xF8)!=START);
    
    if (!error)
    {    TWDR=(adress<<1);            //load slave adress
        TWCR=0x84;                        //clear int_flag to start transmission
        while (!(TWCR&0x80));                //wait for int_flag
        error=((TWSR&0xF8)!=SLA_W_ACK);
    }
    
    if (!error)
    {    TWDR=subadress;
        TWCR=0x84;                        //clear int_flag to start transmission
        while (!(TWCR&0x80));                //wait for int_flag
        error=((TWSR&0xF8)!=DATA_W_ACK);
    }

    TWCR=0xA4;                            //send start condition
    while (!(TWCR&0x80));                    //wait for int_flag
    error=((TWSR&0xF8)!=REP_START);

    if (!error)
    {    TWDR=(adress<<1)|0x01;            //load slave adress + read bit
        TWCR=0x84;                        //clear int_flag to start transmission
        while (!(TWCR&0x80));                //wait for int_flag
        error=((TWSR&0xF8)!=SLA_R_ACK);
    }
    
    if (!error)
    {    TWCR=0x84;                        //clear int_flag to start transmission
        while (!(TWCR&0x80));                //wait for int_flag
        *data=TWDR;
    }

    TWCR=0x94;                        //generate stop condition

    return error;
}

////////////main///////////////

int main(void)
{

DDRD=0b00010000;
//TWI

/* Initial TWI Peripheral */
 TWSR = 0x00;   // Select Prescaler of 1

 // SCL frequency = 11059200 / (16 + 2 * 48 * 1) = 98.743 khz
 TWBR = 0x30;   // 48 Decimal

 // Read the EEPROM ID
uint8_t dev_address=0;              // Start at Address 0



//main loop
  while(1){
             //loop body


			 
 unsigned char fg=0b00010000;
//////////////////////////////////////time base ///////////////////////////////////
out:
{
 while(((fg&PIND)!=0b00010000)&&(state0==0))
   {

    _delay_us(995);
    if(flag0>0)
     {
       flag0--;
      }
     else
      {
        flag0=7;

         //get data from i2c
		 ma1(flag1);
		 //end data
        if(flag1<12)
              {
                 flag1++;
               }
         else
               {
                  flag1=1;
               }
			   goto out;
        }
   }
 }
   // end of out!

 


///////////////////////////////////////////////////////////////////////////////////////////
// broadcast


_delay_ms(1);
state0=0;
state1=0;
state2=0;

flag0=7;
flag1=1;

_delay_ms(15);
int y=0;
int k=1;
for(y=1;y<100;y++)
{
PORTD^=(0x01);
_delay_ms(5);


}
                int i_val=0;
unsigned char buf1,buf0;    
            _delay_ms(100);
            for(i_val=0;i_val<12;i_val++)
                {
				int j_val=0x00;
				int a=pres[i_val];
				int b=temp[i_val];

				for(j_val=0x00;j_val<0x10;j_val++)
				{
                    buf0=0x01&a;
					buf1=0x01&b;
					a=a>>1;
					b=b>>1;
					// set all the timing for  t and 2*t
                 if((buf0==1)&&(buf1==1))
					{// for case 1,1--(temp,pres)=(l ,h)

					PORTD^=0x03;_delay_us(499);
					PORTD^=0x02;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x00;_delay_us(499);

					PORTD^=0x03;_delay_us(499);
					PORTD^=0x02;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x00;_delay_us(499);

					PORTD^=0x03;_delay_us(499);
					PORTD^=0x02;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x00;_delay_us(499);

					PORTD^=0x03;_delay_us(499);
					PORTD^=0x02;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x00;_delay_us(499);

					PORTD^=0x03;_delay_us(499);
					PORTD^=0x02;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x00;_delay_us(493);
					}
					else if((buf0==0)&&(buf1==1))
                    {
					//for case  0,1

					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);

					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);

					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);

					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);

					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(493);

                     }
					else if((buf0==1)&&(buf1==0))
                    {
					//for case   1, 0

					PORTD^=0x02;_delay_us(998);
					PORTD^=0x02;_delay_us(998);

				   	PORTD^=0x02;_delay_us(998);
					PORTD^=0x02;_delay_us(998);

			       	PORTD^=0x02;_delay_us(998);
					PORTD^=0x02;_delay_us(998);

					PORTD^=0x02;_delay_us(998);
					PORTD^=0x02;_delay_us(998);

		            PORTD^=0x02;_delay_us(998);
					PORTD^=0x02;_delay_us(998);
                    }
					//case 0 ,0
                    else
					{
					PORTD^=0x00;_delay_us(9997);
                    }
				}
               }



			   ////////////////////next part
			   for(i_val=0;i_val<12;i_val++)
                {
				int j_val=0x00;
				int a=temp[i_val];
				int b=pres[i_val];

				for(j_val=0x00;j_val<0x10;j_val++)
				{
                    buf0=0x01&a;
					buf1=0x01&b;
					a=a>>1;
					b=b>>1;
					// set all the timing for  t and 2*t
                 if((buf0==1)&&(buf1==1))
					{// for case 1,1--(temp,pres)=(l ,h)

					PORTD^=0x03;_delay_us(499);
					PORTD^=0x02;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x00;_delay_us(499);

					PORTD^=0x03;_delay_us(499);
					PORTD^=0x02;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x00;_delay_us(499);

					PORTD^=0x03;_delay_us(499);
					PORTD^=0x02;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x00;_delay_us(499);

					PORTD^=0x03;_delay_us(499);
					PORTD^=0x02;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x00;_delay_us(499);

					PORTD^=0x03;_delay_us(499);
					PORTD^=0x02;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x00;_delay_us(493);
					}
					else if((buf0==0)&&(buf1==1))
                    {
					//for case  0,1

					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);

					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);

					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);

					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);

					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(499);
					PORTD^=0x01;_delay_us(493);

                     }
					else if((buf0==1)&&(buf1==0))
                    {
					//for case   1, 0

					PORTD^=0x02;_delay_us(998);
					PORTD^=0x02;_delay_us(998);

				   	PORTD^=0x02;_delay_us(998);
					PORTD^=0x02;_delay_us(998);

			       	PORTD^=0x02;_delay_us(998);
					PORTD^=0x02;_delay_us(998);

					PORTD^=0x02;_delay_us(998);
					PORTD^=0x02;_delay_us(998);

		            PORTD^=0x02;_delay_us(998);
					PORTD^=0x02;_delay_us(998);
                    }
					//case 0 ,0
                    else
					{
					PORTD^=0x00;_delay_us(9997);
                    }
				}
               }

			   for(y=1;y<100;y++)
{
PORTD^=(0x01);
_delay_ms(5);


}

/////// no idea //////////////////

          }
}

///end of main//////
// /////////////////read frm i2c///////////////
void ma1(int fl)
{
PORTD^=0x01;
PORTD^=0x01;
PORTD^=0x01;
     ma2(fl);
						//calcuations
 }



// //////////////////comppute the vals///////////////////
void ma2(int fl1)
{


//calcuations!!!
PORTD^=0x11;
PORTD^=0x11;


}
