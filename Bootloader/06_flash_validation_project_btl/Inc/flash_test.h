/*
 * flash_test.h
 *
 *  Created on: 17-May-2026
 *      Author: brajo
 */

#ifndef FLASH_TEST_H_
#define FLASH_TEST_H_

void flash_test_run(void);
void test_flash_erase(void);
void test_flash_program(void);
void test_alignment_error(void);
void test_locked_flash(void);

#endif /* FLASH_TEST_H_ */
