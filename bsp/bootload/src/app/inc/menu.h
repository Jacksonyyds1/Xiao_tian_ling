 /*
 * menu.h
 *
 *  Created on: 2022��10��19��
 *      Author: lwp edit
 */
#ifndef __MENU_H
#define __MENU_H

/*
********************************************************************************
********************************************************************************
*/

#define FILE_NAME_LENGTH        (256)
#define FILE_SIZE_LENGTH        (16)

typedef void (*pFunction)(void);
extern uint8_t FileName[FILE_NAME_LENGTH];
/*
********************************************************************************
********************************************************************************
*/
void Main_Menu(void);

#endif /* __MENU_H */

