/**
  ******************************************************************************
  * @file    circular_buffer.h
  * @author  Lukasz Garlicki
  * @brief   Header file of circular buffer initialization, update and
  *            software implementation of Moving Average filter
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_CIRCULAR_BUFFER_H_
#define INC_CIRCULAR_BUFFER_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>

/* Defines ------------------------------------------------------------------*/
/* Define used for setting the maximum amount of entries in circular buffer array */
#define CIRCULAR_BUFFER_MAX_SIZE 8

/**
 * @brief  Circular buffer structure for floating point data.
 */
typedef struct
{
    float data[CIRCULAR_BUFFER_MAX_SIZE];  /*!< Storage array for up to 8 data entries of float type;
                                             incrementing the amount of data makes up for higher precision
                                             of filtering later on*/

    uint16_t front;                        /*!< Index of the youngest added
                                             value to the circular buffer,
                                             wrapping itself back to 0 after meeting the max size */

    uint16_t size;                         /*!< Size of the circular buffer, equal to size of data array */

    uint8_t count;                         /*!< Additional variable for holding the amount of entries
                                                curretly placed in circular buffer; used for filter
                                                function to properly adjust filtering nominator
                                                when there isn't sufficient enough
                                                measurements count (due to measuring delay) */
} circular_buffer_t;


/**
  * @brief  Initializes the buffer init, setting proper size, front and count values,
  * as well as initializing the data array with zeroes.
  * @param  pBuffer Pointer to the buffer meant for initialization
  */
void Circular_Buffer_Init(volatile circular_buffer_t * pBuffer);
/**
  * @brief  Updates the desired buffer with new data and handles the wrapping part of circular buffer
  * @param pBuffer Pointer to the buffer meant for update
  * @param new_data Data value to throw into circular buffer
  */
void Circular_Buffer_Update(volatile circular_buffer_t * pBuffer, float new_data);
/**
  * @brief  Applies the Moving Average filter algorithm to array of data
  * @param  pArray Pointer to the array meant for provide data to filter
  * @param  count Number of entries meant to be filtered
  * @retval Value of Moving Average filter algorithm on provided data
  */
float Filter_Average(volatile float * pArray, uint16_t count);

#endif /* INC_CIRCULAR_BUFFER_H_ */
