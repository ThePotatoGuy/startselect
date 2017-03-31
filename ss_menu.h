/**
 * Menu Page 
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * For now, this just creates two threads:
 * one thread handles creation of a window with quit button
 * 2nd thread actuallt waits for controller input and displays the information
 */

#ifndef SS_MENU_H
#define SS_MENU_H

void* ss_event_handling(void *thread_data);

/*
 * Runs the menu (more descriptions later)
 */
int ss_menu_run();

// temp
void* ss_open_window(void *thread_data);

#endif

