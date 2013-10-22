#ifndef _STARTCF_H_
#define _STARTCF_H_

/*
 * This file is part of BaS_gcc.
 *
 * BaS_gcc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BaS_gcc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BaS_gcc.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: 26.02.2013
 *      Author: Markus Fröschle
 */

#define cf_stack

#define DIP_SWITCH		(*(volatile uint8_t *)(&_MBAR[0xA2C]))
#define DIP_SWITCHa		__MBAR + 0xA2C

#define sca_page_ID 	6

#endif /* _STARTCF_H_ */
