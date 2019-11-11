/*
 * MyTask.h
 *
 *  Created on: Nov 11, 2019
 *      Author: as_77
 */

#ifndef INC_MYTASK_H_
#define INC_MYTASK_H_

#define task_N 2

typedef enum
{
	incomplit,
	complit,
	start
}STAT_t;


void NetRouting(uint8_t arg);
//void executeEthCommand(struct netconn *xNetConn,uint8_t* STK_US,uint8_t n );

#endif /* INC_MYTASK_H_ */
