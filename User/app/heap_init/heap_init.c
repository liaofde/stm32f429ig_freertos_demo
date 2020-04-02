#include "heap_init.h"
#include "freertos.h"
#include "sdram_bsp.h"

void heap_init(void)
{
  //static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
  HeapRegion_t xHeapRegions[] =
  {
    //{ ( uint8_t * ) ucHeap, configTOTAL_HEAP_SIZE },
    { ( uint8_t * ) SDRAM_BANK_ADDR, 1024*1024*32/sizeof(int) },
    { NULL, 0 }                                          //<< Terminates the array.
  };

  vPortDefineHeapRegions( xHeapRegions );  //<< Pass the array into vPortDefineHeapRegions().
}
