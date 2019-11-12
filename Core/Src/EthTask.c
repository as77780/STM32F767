/*
 * EthTask.c
 *
 *  Created on: Nov 11, 2019
 *      Author: as_77
 */

#include "EthTask.h"
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "lwip.h"

#include "stdint.h"
#include "string.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"

char str1[60];
char str_buf[1000]={'\0'};
osThreadId TaskStringOutHandle;
osMailQId strout_Queue;
osThreadId defaultTaskHandle;


typedef struct struct_out_t {
  uint32_t tick_count;
  uint16_t y_pos;
  char str[60];
} struct_out;
typedef struct struct_conn_t {
  uint32_t conn;
  uint32_t buf;
} struct_conn;
struct_conn conn01;
#define MAIL_SIZE (uint32_t) 5

void StartDefaultTask(void const * argument);
void TaskStringOut(void const * argument);


void StartTasks(){
	 osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1280);
	  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);


	  osThreadDef(tskstrout, TaskStringOut, osPriorityBelowNormal, 0, 1280);
	  TaskStringOutHandle = osThreadCreate(osThread(tskstrout), NULL);


	  osMailQDef(stroutqueue, MAIL_SIZE, struct_out);
	  strout_Queue = osMailCreate(osMailQ(stroutqueue), NULL);
}

void TaskStringOut(void const * argument)
{
	osEvent event;
	struct_out *qstruct;
  for(;;)
  {
  	event = osMailGet(strout_Queue, osWaitForever);
    if (event.status == osEventMail)
    {
    	printf("Enter");
			qstruct = event.value.p;
			printf("%s", qstruct->str);

		//	TFT_DisplayString(50, qstruct->y_pos, (uint8_t *)str1, LEFT_MODE);
    }
  }
}
//---------------------------------------------------------------
static void send_thread(void *arg)
{
  struct_conn *arg_conn;
  struct_out *qstruct;
  struct netconn *conn;
  err_t sent_err;
  arg_conn = (struct_conn*) arg;
  conn = (void*)arg_conn->conn;
  uint32_t syscnt = 0;
  char buf[15] = {};
  for(;;)
  {
  	syscnt = osKernelSysTick();
  	if(syscnt>50000)
  	{
  	  netconn_close(conn);
  	  netconn_delete(conn);
  	  qstruct = osMailAlloc(strout_Queue, osWaitForever);
  	  qstruct->y_pos = 160;
  	  strcpy(qstruct->str,"Connection was closed!");
  	  osMailPut(strout_Queue, qstruct);
  	  osMailFree(strout_Queue, qstruct);
  	  osDelay(2);
  	  break;
  	}
  	sprintf(buf,"%lu\r\n",syscnt);
  	sent_err = netconn_write(conn, (void *) buf, strlen(buf), NETCONN_COPY);
  	if(sent_err == ERR_OK)
  	{
  	  qstruct = osMailAlloc(strout_Queue, osWaitForever);
  	  qstruct->y_pos = 60;
  	  buf[strlen(buf)-2]=0;
  	  strcpy(qstruct->str,buf);
  	  osMailPut(strout_Queue, qstruct);
  	  osMailFree(strout_Queue, qstruct);
  	}
    osDelay(1000);
  }
}
//---------------------------------------------------------------
static void recv_thread(void *arg)
{
  struct_conn *arg_conn;
  err_t recv_err;
  struct netconn *conn;
  struct netbuf *inbuf;
  struct_out *qstruct;
  uint8_t* buf;
  u16_t buflen;
  arg_conn = (struct_conn*) arg;
  conn = (void*)arg_conn->conn;
  for(;;)
  {
  	recv_err = netconn_recv(conn, &inbuf);
  	if (recv_err == ERR_OK)
  	{
  	  if (netconn_err(conn) == ERR_OK)
  	  {
  	    netbuf_data(inbuf, (void**)&buf, &buflen);
  	    if(buflen>1)
  	    {
  	      qstruct = osMailAlloc(strout_Queue, osWaitForever);
  	      qstruct->y_pos = 100;
  	      strncpy(str_buf,(char*)buf,buflen);
  	      str_buf[buflen-1]=0;
  	      sprintf(qstruct->str,"%-20s", str_buf);
  	      osMailPut(strout_Queue, qstruct);
  	      osMailFree(strout_Queue, qstruct);
  	    }
  	    netbuf_delete(inbuf);
  	  }
  	}
  }
}
//---------------------------------------------------------------
/* USER CODE END 4 */

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();

  /* USER CODE BEGIN 5 */
  struct netconn *conn;
  err_t err;
  ip_addr_t ServerIPaddr;
  IP4_ADDR(&ServerIPaddr, 192, 168, 0, 103);
//  TFT_SetTextColor(LCD_COLOR_BLUE);
  conn = netconn_new(NETCONN_TCP);
  if(conn!=NULL)
  {
    err = netconn_bind(conn, NULL, 23);
    if (err == ERR_OK)
    {
     // err = netconn_connect(conn, &ServerIPaddr, 5444);
     // if (err == ERR_OK)
      {
        conn01.conn = conn;
        sys_thread_new("send_thread", send_thread, (void*)&conn01, DEFAULT_THREAD_STACKSIZE, osPriorityNormal );
        sys_thread_new("recv_thread", recv_thread, (void*)&conn01, DEFAULT_THREAD_STACKSIZE, osPriorityNormal );
      }
    }
    else
    {
      netconn_delete(conn);
    }
  }
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}
