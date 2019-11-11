/*
 * MyTask.h
 *
 *  Created on: Nov 11, 2019
 *      Author: as_77
 */

#ifndef INC_MYTASK_H_
#define INC_MYTASK_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define task_N 2

typedef enum
{
	incomplit,
	complit,
	start
}STAT_t;


void NetRouting(uint8_t arg);

void reboot_eth (void);
void poweroff_eth (void);
void prog_eth (void);
void prog_pl_eth (void);
void prog_bud_eth (void);
void R_stop_eth (void);
void R_Left_eth (void);
void R_Right_eth (void);
void R_V_PL_eth (void);
void R_V_Min_eth (void);
void Inet_test_eth (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INC_MYTASK_H_ */
