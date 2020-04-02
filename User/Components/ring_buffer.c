#include "ring_buffer.h"
#include <string.h>

static inline enum ringbuffer_state ringbuffer_status(RINGBUFF_T *rb)
{
    if (rb->tail_index == rb->head_index)
    {
        if (rb->tail_mirror == rb->head_mirror)
            return RINGBUFFER_EMPTY;
        else
            return RINGBUFFER_FULL;
    }
    return RINGBUFFER_HALFFULL;
}

void ringbuffer_init(RINGBUFF_T *rb,
                        uint8_t           *pool,
                        int16_t            size)
{
    //ASSERT(rb != NULL);
    //ASSERT(size > 0);

    /* initialize read and write index */
    rb->tail_mirror = rb->tail_index = 0;
    rb->head_mirror = rb->head_index = 0;

    /* set buffer pool and size */
    rb->buffer_ptr = pool;
    rb->buffer_size = size;
}

/**
 * put a block of data into ring buffer
 */
size_t ringbuffer_put(RINGBUFF_T *rb,
                            const uint8_t     *ptr,
                            uint16_t           length)
{
    uint16_t size;

    //ASSERT(rb != NULL);

    /* whether has enough space */
    size = ringbuffer_space_len(rb);

    /* no space */
    if (size == 0)
        return 0;

    /* drop some data */
    if (size < length)
        length = size;

    if (rb->buffer_size - rb->head_index > length)
    {
        /* tail_index - head_index = empty space */
        memcpy(&rb->buffer_ptr[rb->head_index], ptr, length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->head_index += length;
        return length;
    }

    memcpy(&rb->buffer_ptr[rb->head_index],
           &ptr[0],
           rb->buffer_size - rb->head_index);
    memcpy(&rb->buffer_ptr[0],
           &ptr[rb->buffer_size - rb->head_index],
           length - (rb->buffer_size - rb->head_index));

    /* we are going into the other side of the mirror */
    rb->head_mirror = ~rb->head_mirror;
    rb->head_index = length - (rb->buffer_size - rb->head_index);

    return length;
}

/**
 * put a block of data into ring buffer
 *
 * When the buffer is full, it will discard the old data.
 */
size_t ringbuffer_put_force(RINGBUFF_T *rb,
                            const uint8_t     *ptr,
                            uint16_t           length)
{
    uint16_t space_length;

    //ASSERT(rb != NULL);

    space_length = ringbuffer_space_len(rb);

    if (length > rb->buffer_size)
    {
        ptr = &ptr[length - rb->buffer_size];
        length = rb->buffer_size;
    }

    if (rb->buffer_size - rb->head_index > length)
    {
        /* tail_index - head_index = empty space */
        memcpy(&rb->buffer_ptr[rb->head_index], ptr, length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->head_index += length;

        if (length > space_length)
            rb->tail_index = rb->head_index;

        return length;
    }

    memcpy(&rb->buffer_ptr[rb->head_index],
           &ptr[0],
           rb->buffer_size - rb->head_index);
    memcpy(&rb->buffer_ptr[0],
           &ptr[rb->buffer_size - rb->head_index],
           length - (rb->buffer_size - rb->head_index));

    /* we are going into the other side of the mirror */
    rb->head_mirror = ~rb->head_mirror;
    rb->head_index = length - (rb->buffer_size - rb->head_index);

    if (length > space_length)
    {
        rb->tail_mirror = ~rb->tail_mirror;
        rb->tail_index = rb->head_index;
    }

    return length;
}

/**
 *  get data from ring buffer
 */
size_t ringbuffer_get(RINGBUFF_T *rb,
                            uint8_t           *ptr,
                            uint16_t           length)
{
    size_t size;

    //ASSERT(rb != NULL);

    /* whether has enough data  */
    size = ringbuffer_data_len(rb);

    /* no data */
    if (size == 0)
        return 0;

    /* less data */
    if (size < length)
        length = size;

    if (rb->buffer_size - rb->tail_index > length)
    {
        /* copy all of data */
        memcpy(ptr, &rb->buffer_ptr[rb->tail_index], length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->tail_index += length;
        return length;
    }

    memcpy(&ptr[0],
           &rb->buffer_ptr[rb->tail_index],
           rb->buffer_size - rb->tail_index);
    memcpy(&ptr[rb->buffer_size - rb->tail_index],
           &rb->buffer_ptr[0],
           length - (rb->buffer_size - rb->tail_index));

    /* we are going into the other side of the mirror */
    rb->tail_mirror = ~rb->tail_mirror;
    rb->tail_index = length - (rb->buffer_size - rb->tail_index);

    return length;
}

/**
 * put a character into ring buffer
 */
size_t ringbuffer_putchar(RINGBUFF_T *rb, const uint8_t ch)
{
    //ASSERT(rb != NULL);

    /* whether has enough space */
    if (!ringbuffer_space_len(rb))
        return 0;

    rb->buffer_ptr[rb->head_index] = ch;

    /* flip mirror */
    if (rb->head_index == rb->buffer_size-1)
    {
        rb->head_mirror = ~rb->head_mirror;
        rb->head_index = 0;
    }
    else
    {
        rb->head_index++;
    }

    return 1;
}

/**
 * put a character into ring buffer
 *
 * When the buffer is full, it will discard one old data.
 */
size_t ringbuffer_putchar_force(RINGBUFF_T *rb, const uint8_t ch)
{
    enum ringbuffer_state old_state;

    //ASSERT(rb != NULL);

    old_state = ringbuffer_status(rb);

    rb->buffer_ptr[rb->head_index] = ch;

    /* flip mirror */
    if (rb->head_index == rb->buffer_size-1)
    {
        rb->head_mirror = ~rb->head_mirror;
        rb->head_index = 0;
        if (old_state == RINGBUFFER_FULL)
        {
            rb->tail_mirror = ~rb->tail_mirror;
            rb->tail_index = rb->head_index;
        }
    }
    else
    {
        rb->head_index++;
        if (old_state == RINGBUFFER_FULL)
            rb->tail_index = rb->head_index;
    }

    return 1;
}

/**
 * get a character from a ringbuffer
 */
size_t ringbuffer_getchar(RINGBUFF_T *rb, uint8_t *ch)
{
    //ASSERT(rb != NULL);

    /* ringbuffer is empty */
    if (!ringbuffer_data_len(rb))
        return 0;

    /* put character */
    *ch = rb->buffer_ptr[rb->tail_index];

    if (rb->tail_index == rb->buffer_size-1)
    {
        rb->tail_mirror = ~rb->tail_mirror;
        rb->tail_index = 0;
    }
    else
    {
        rb->tail_index++;
    }

    return 1;
}

/** 
 * get the size of data in rb 
 */
size_t ringbuffer_data_len(RINGBUFF_T *rb)
{
    switch (ringbuffer_status(rb))
    {
    case RINGBUFFER_EMPTY:
        return 0;
    case RINGBUFFER_FULL:
        return rb->buffer_size;
    case RINGBUFFER_HALFFULL:
    default:
        if (rb->head_index > rb->tail_index)
            return rb->head_index - rb->tail_index;
        else
            return rb->buffer_size - (rb->tail_index - rb->head_index);
    };
}

/** 
 * query index of the specified character  
 * -1 query is fail, 0~ buffsize-1:query is success
 */
int ringbuffer_query_index_specified_character(RINGBUFF_T *rb, uint8_t ch)
{
    //ASSERT(rb != NULL);
    int ret = -1;
    int index = rb->tail_index;
    int len = ringbuffer_data_len(rb);
  /* ringbuffer is empty */    
    if (!len)
      return -1;

    uint8_t *pt = rb->buffer_ptr;
    for(int i=0; i< len; i++)
    {
      if(pt[index] == ch)
      {
        ret = index;
        break;
      }
      ++index;
      index  %= rb->buffer_size;
    }
    
    return ret;
}

/** 
 * empty the rb 
 */
void ringbuffer_reset(RINGBUFF_T *rb)
{
    //ASSERT(rb != NULL);

    rb->tail_mirror = 0;
    rb->tail_index = 0;
    rb->head_mirror = 0;
    rb->head_index = 0;
}
