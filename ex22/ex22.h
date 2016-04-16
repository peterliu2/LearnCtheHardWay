/**
 * @file ex22.h
 * @brief ex22 header file
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-05
 */

#ifndef _ex22_h_
#define _ex22_h_

/* makes THE_SIZE in ex22.c available to other .c files */
extern int THE_SIZE;

/* gets and sets an internal static variable in ex22.c */
int get_age(void);
void set_age(int age);

/* update a static variable that's inside update_ratio */
double update_ratio(double ratio);

void print_size(void);

#endif /* _ex22_h_ */
