/*
 * bbuff.h
 *
 *  Created on: 
 *      Author: Ali Sharifi
 */

#ifndef BBUFF_H_
#define BBUFF_H_

#define BUFFER_SIZE 10

void bbuff_init(void);
void bbuff_blocking_insert(void* item);
void* bbuff_blocking_extract(void);
_Bool bbuff_is_empty(void);


#endif /* BBUFF_H_ */
