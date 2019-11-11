/*
 * MyTask.c
 *
 *  Created on: Nov 11, 2019
 *      Author: as_77
 */

#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"
#include "stm32f7xx_hal.h"
#include "MyTask.h"
#include "cmsis_os.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "string.h"



volatile struct netconn *NetConn[2];
extern volatile struct netif gnetif;

STAT_t Eth_INET,Eth_REDy[task_N];

void executeEthCommand(struct netconn *xNetConn,uint8_t* STK_US,uint8_t n );
void executeEthDate(struct netconn *xNetConn,uint8_t* STK_US,uint8_t* date);
uint8_t compareCommand(uint8_t buf[], uint8_t compString[]);
void net_wr(struct netconn *N_Conn,uint8_t* str);

void NetRouting(uint8_t arg){
	 struct netbuf *buf;
		// uint8_t* data1="test\r\n";
		 uint8_t N = arg;
		void *data;
		  u16_t len;
		  int rc1, rc2;
		  uint8_t DEST_PORT=23;
		  	struct netconn *xNetConn = NULL;
		  	uint8_t IP_ADDRESS_REM[4];

		  	  IP_ADDRESS_REM[0] = 192;
		  	  IP_ADDRESS_REM[1] = 168;
		  	  IP_ADDRESS_REM[2] = 0;
		  	  IP_ADDRESS_REM[3] = 103;
		  	  ip4_addr_t remote_ip;
		  	  ip4_addr_t local_ip =gnetif.ip_addr;


		  	  IP4_ADDR(&remote_ip, IP_ADDRESS_REM[0], IP_ADDRESS_REM[1], IP_ADDRESS_REM[2], IP_ADDRESS_REM[3]);

		  	 while(gnetif.ip_addr.addr == 0);
		  	 printf("%d:Local IP address: %d.%d.%d.%d\n\r",N,local_ip.addr&0xFF,(local_ip.addr>>8)&0xFF,(local_ip.addr>>16)&0xFF,(local_ip.addr>>24)&0xFF);
		  	 printf("%d:Remote IP address: %d.%d.%d.%d\n\r",N,remote_ip.addr&0xFF,(remote_ip.addr>>8)&0xFF,(remote_ip.addr>>16)&0xFF,(remote_ip.addr>>24)&0xFF);





		  	while(1){

					 while(rc2!=ERR_OK)
					 {
						 printf("%d:Connect to Remote IP address: %d.%d.%d.%d\n\r",N,remote_ip.addr&0xFF,(remote_ip.addr>>8)&0xFF,(remote_ip.addr>>16)&0xFF,(remote_ip.addr>>24)&0xFF);
						 xNetConn = netconn_new ( NETCONN_TCP );
						 rc1 = netconn_bind ( xNetConn, &local_ip, DEST_PORT );
						 rc2 = netconn_connect ( xNetConn, &remote_ip, DEST_PORT );
						 printf("\r\n connect... \r\n");
							NetConn[N] = xNetConn;
						 if(rc2!=ERR_OK)
						 {
							 netconn_close(xNetConn);
							 netconn_delete(xNetConn);
						 }
					 }

					 printf("\r\n%Remote host connected %d\r\n",N);


					 uint8_t p=0;
		 while (netconn_recv(xNetConn, &buf) == ERR_OK)
		          {

			         do
		            {
		              netbuf_data(buf, &data, &len);
		             if(p<=10){
		              netconn_write(xNetConn, data, len, NETCONN_COPY);
		              p++;
		             }

		             // print(data,0);
		             printf("%d: ",N);   //Log Enabl
		             printf(data);
		              executeEthCommand(xNetConn,data,N);
		            //  executeEthDate(xNetConn,data,NULL);
		           //   ReadSTatEth(xNetConn);
		                        }

		            while (netbuf_next(buf) >= 0);

		            netbuf_delete(buf);
		          }
		 netconn_close(xNetConn);
		           netconn_delete(xNetConn);
	//	           Eth_REDy[N]=incomplit;
	//	           Eth_INET=incomplit;
		       //    P_STOP();

		 printf("\r\%dnRemote host disconnected %d\r\n",N);
		 rc2=ERR_ABRT;
		 //	vTaskDelete(xTaskGetCurrentTaskHandle());
	}

}


void executeEthCommand(struct netconn *xNetConn,uint8_t* STK_US,uint8_t n ){
		if (compareCommand(STK_US, "as-UP-CHT01 login:")==1)
				  {
			         net_wr(xNetConn, "as\r");
				  }

		 if (compareCommand(STK_US, "assword") == 1) {
			            net_wr(xNetConn, "ASD777as\r");
				  }
		 if (compareCommand(STK_US, "as@as-UP-CHT01:~$") == 1) {
			 Eth_REDy[n]=complit;
						  }
		 if (compareCommand(STK_US, "In_ISt") == 1) {
			              Eth_INET=incomplit;
		 				  }
		 if (compareCommand(STK_US, "INET_IS") == 1) {
			 Eth_INET=complit;
				 				  }

         if((compareCommand(STK_US, "MSK") == 1)&&(n==1)) {
        	// net_wr(NetConn[1],"date\r");
        	// print(STK_US,0);
       	 executeEthDate(xNetConn,STK_US,NULL);
         }
}

uint8_t compareCommand(uint8_t buf[], uint8_t compString[]){


	uint8_t* temp=strstr(buf,compString);
	//if(strncmp(temp,compString,strlen(compString))==0){
	if(temp!=NULL){
		return 1;
	}
	else {
		return 0;
	}

}

void executeEthDate(struct netconn *xNetConn,uint8_t* STK_US,uint8_t* date){
	uint8_t mas_date[3];
	uint8_t* str="MSK";
	uint8_t* sep=':';
	uint8_t mas[8];
//	uint8_t mas_date[3];
	uint8_t* istr=strncpy(mas,(strstr(STK_US,str)-9),8);
//	print(istr,0);
//	print("\r\n",0);
	mas_date[0]=atoi(istr);
	mas_date[1]=atoi(istr+3);
	mas_date[2]=atoi(istr+6);
//	print("%02d : %02d : %02d\r\n",3,mas_date[0],mas_date[1],mas_date[2]);
	/*
	RTC_HandleTypeDef hrtc;
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	 HAL_RTC_GetTime(&hrtc, &sTime, FORMAT_BIN);
			 HAL_RTC_GetDate(&hrtc, &sDate, FORMAT_BIN);
			sTime.Hours=10;
			sTime.Minutes=10;
			sTime.Seconds=10;
			HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN);
	 *
	 *
	 */

}

void net_wr(struct netconn *N_Conn,uint8_t* str){
	 netconn_write(N_Conn, str, strlen(str), NETCONN_NOFLAG);
}



void reboot_eth (void){	net_wr(NetConn[0],"sudo reboot\r");}
void poweroff_eth (void){net_wr(NetConn[0],"sudo poweroff\r");}
void prog_eth (void){net_wr(NetConn[0],"./radio.sh\r");}
void prog_pl_eth (void){net_wr(NetConn[0],"./music.sh\r");}
void prog_bud_eth (void){net_wr(NetConn[0],"./bud.sh\r");}
void R_stop_eth (void){net_wr(NetConn[0],"q\r");}
void R_Left_eth (void){net_wr(NetConn[0],"<");}
void R_Right_eth (void){net_wr(NetConn[0],">");}
void R_V_PL_eth (void){net_wr(NetConn[0],"*");}
void R_V_Min_eth (void){net_wr(NetConn[0],"/");}
void Inet_test_eth (void){net_wr(NetConn[1],"./inet.sh\r");	net_wr(NetConn[1],"date\r");}
