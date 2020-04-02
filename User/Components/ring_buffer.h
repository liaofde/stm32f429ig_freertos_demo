/*
 * Copyright (c) 2006-2018,
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
  
/* ring buffer */
typedef struct
{
    uint8_t *buffer_ptr;
    uint16_t tail_mirror : 1;
    uint16_t tail_index : 15;
    uint16_t head_mirror : 1;
    uint16_t head_index : 15;
    int16_t buffer_size;
}RINGBUFF_T;

enum ringbuffer_state
{
    RINGBUFFER_EMPTY,
    RINGBUFFER_FULL,
    /* half full is neither full nor empty */
    RINGBUFFER_HALFFULL,
};

/**
 * RingBuffer for DeviceDriver
 *
 * Please note that the ring buffer implementation of RT-Thread
 * has no thread wait or resume feature.
 */
void ringbuffer_init(RINGBUFF_T *rb, uint8_t *pool, int16_t size);
void ringbuffer_reset(RINGBUFF_T *rb);
size_t ringbuffer_put(RINGBUFF_T *rb, const uint8_t *ptr, uint16_t length);
size_t ringbuffer_put_force(RINGBUFF_T *rb, const uint8_t *ptr, uint16_t length);
size_t ringbuffer_putchar(RINGBUFF_T *rb, const uint8_t ch);
size_t ringbuffer_putchar_force(RINGBUFF_T *rb, const uint8_t ch);
size_t ringbuffer_get(RINGBUFF_T *rb, uint8_t *ptr, uint16_t length);
size_t ringbuffer_getchar(RINGBUFF_T *rb, uint8_t *ch);
size_t ringbuffer_data_len(RINGBUFF_T *rb);

#ifdef USING_HEAP
RINGBUFF_T* ringbuffer_create(uint16_t length);
void ringbuffer_destroy(RINGBUFF_T *rb);
#endif

inline uint16_t ringbuffer_get_size(RINGBUFF_T *rb)
{
    //RT_ASSERT(rb != NULL);
    return rb->buffer_size;
}

/** return the size of empty space in rb */
#define ringbuffer_space_len(rb) ((rb)->buffer_size - ringbuffer_data_len(rb))


#ifdef __cplusplus
}
#endif

#endif
