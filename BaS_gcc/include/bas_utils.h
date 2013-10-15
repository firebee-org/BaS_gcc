/*
 * bas_utils.h
 *
 *  Created on: 08.08.2013
 *      Author: froesm1
 */

#ifndef _BAS_UTILS_H_
#define _BAS_UTILS_H_

#define CLEAR_BIT(p,bit)        p &= ~(bit)
#define CLEAR_BIT_NO(p,nr)      CLEAR_BIT(p, (1 << (nr)))

#endif /* _BAS_UTILS_H_ */
