#ifndef __ERROR_CHK_H
#define __ERROR_CHK_H

#include <stm8s.h>

#define MAIN_FAN_OPEN_ERROR_THR                 0x00CC          /* LOW 1V */
#define MAIN_FAN_SHORT_ERROR_THR                0x0399          /* up 4.5V */

#define GLASS_ERROR_THR                         0x0066           /* LOW 0.5V */
#define GLASS_LAMP_TYPE1_THRL                   0x00A3          /* up 0.8V */
#define GLASS_LAMP_TYPE1_THRH                   0x01D7          /* LOW 2.3V */
#define GLASS_LAMP_TYPE2_THRL                   0x030A          /* up 3.8V */
#define GLASS_LAMP_TYPE2_THRH                   0x0370          /* LOW 4.3V */
#define GLASS_LAMP_TYPE3_THR                    0x03C2          /* up 4.7V */

#define ONE_LAMP                                1
#define TWO_LAMP                                2
#define THREE_LAMP                              3

void ERROR_CHK_Fun(void);
void MainFan_Error_Chk_Fun(void);
void Glass_Error_Chk_Fun(void);

#endif