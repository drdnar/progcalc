#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>
#include <fontlibc.h>
#include "bigint.h"
#include "settings.h"
#include "style.h"
#include "printbigint.h"

/* 0x12345678901234567890123456789012 */
BigInt_t n1 = { 0x12, 0x90, 0x78, 0x56, 0x34, 0x12, 0x90, 0x78, 0x56, 0x34, 0x12, 0x90, 0x78, 0x56, 0x34, 0x12, };
/* 0x7890123456789012 */
BigInt_t n2 = { 0x12, 0x90, 0x78, 0x56, 0x34, 0x12, 0x90, 0x78, 0, 0, 0, 0, 0, 0, 0, 0, };


/* Main Function */
void main(void) {
    Style_Initialize();
    Settings_Init();
    
    Settings_ChangeDisplayBits(SHOW_128);

    fontlib_DrawString("TESTING");
    fontlib_Newline();
    Format_PrintBin(&n1);
    Format_PrintHex(&n1);
    Format_PrintDec(&n2);
    /*fontlib_Newline();*/
    
    /* Pause */
    while (!os_GetCSC());
    Style_Finalize();
}
