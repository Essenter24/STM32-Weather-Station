/**
  ******************************************************************************
  * @file    circular_buffer.c
  * @author  Lukasz Garlicki
  * @brief   Implementation of functions handling circular buffer initialization,
  * update and Moving Average filter algorithm
  *
  * This module allows for initializing the desired circular buffer, alongside
  * providing a way to update the meant buffer with new data, as well as with provided
  * Moving Average filter function.
  *
  ******************************************************************************
  */
#include "circular_buffer.h"

void Circular_Buffer_Init(volatile circular_buffer_t * p_buffer)
{
    if (p_buffer == NULL)
    {
        return;
    }
    p_buffer->front = 0;
    p_buffer->size = CIRCULAR_BUFFER_MAX_SIZE;
    p_buffer->count = 0;
    for (uint16_t i = 0; i < p_buffer->size; i++)
    {
        p_buffer->data[i] = 0.0f;
    }
}

void Circular_Buffer_Update(volatile circular_buffer_t * p_buffer, float new_data)
{
    if (p_buffer == NULL || p_buffer->size == 0)
    {
        return;
    }
    p_buffer->data[p_buffer->front] = new_data;
    p_buffer->front++;
    p_buffer->front = p_buffer->front % p_buffer->size;

    /* To provide the current amount of data entries in buffer to the filter function, due to
     * desired delay in measurements. When the buffer hits max amount of entries, the count is never changed
     * again due to buffer never having less than the max amount.
     */
    if (p_buffer->count < p_buffer->size)
    {
        p_buffer->count++;
    }
}
float Filter_Average(volatile float * p_array, uint16_t count)
{
    if (count == 0)
    {
        return 0.0f;
    }
    float sum = 0.0f;
    for (uint16_t i = 0; i < count; i++)
    {
        sum += p_array[i];
    }

    return (sum / count);
}
