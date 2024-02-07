/*
 * tc.h
 *
 *  Created on: 29.04.2021
 *      Author: Hare
 */

#ifndef INC_TC_H_
#define INC_TC_H_

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct
{
    uint8_t TouchDetected;
    uint16_t X;
    uint16_t Y;
} TC_StateTypeDef;

#ifdef __cplusplus
}
#endif

#endif /* INC_TC_H_ */
