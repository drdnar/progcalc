#ifndef UI_H
#define UI_H

/**
 * Exits the program, cleaning up stuff.
 */
void ExitClean(void); /* I don't really have a better idea where to put this declaration. */

#include <tice.h>
#include <graphx.h>

typedef uint8_t Key_t;

#define sk_2nd_Modifier 128
#define sk_2nd_Down         (sk_2nd_Modifier | sk_Down      )
#define sk_2nd_Left         (sk_2nd_Modifier | sk_Left      )
#define sk_2nd_Right        (sk_2nd_Modifier | sk_Right     )
#define sk_2nd_Up           (sk_2nd_Modifier | sk_Up        )
#define sk_2nd_Enter        (sk_2nd_Modifier | sk_Enter     )
#define sk_2nd_Clear        (sk_2nd_Modifier | sk_Clear     )
#define sk_2nd_Alpha        (sk_2nd_Modifier | sk_Alpha     )
#define sk_2nd_Add          (sk_2nd_Modifier | sk_Add       )
#define sk_2nd_Sub          (sk_2nd_Modifier | sk_Sub       )
#define sk_2nd_Mul          (sk_2nd_Modifier | sk_Mul       )
#define sk_2nd_Div          (sk_2nd_Modifier | sk_Div       )
#define sk_2nd_Graph        (sk_2nd_Modifier | sk_Graph     )
#define sk_2nd_Trace        (sk_2nd_Modifier | sk_Trace     )
#define sk_2nd_Zoom         (sk_2nd_Modifier | sk_Zoom      )
#define sk_2nd_Window       (sk_2nd_Modifier | sk_Window    )
#define sk_2nd_Yequ         (sk_2nd_Modifier | sk_Yequ      )
#define sk_Quit             (sk_2nd_Modifier | sk_Mode      ) /**< 2nd + Mode */
#define sk_Ins              (sk_2nd_Modifier | sk_Del       ) /**< 2nd + Del */
#define sk_Recall           (sk_2nd_Modifier | sk_Store     ) /**< 2nd + Sto-> */
#define sk_2nd_Ln           (sk_2nd_Modifier | sk_Ln        )
#define sk_2nd_Log          (sk_2nd_Modifier | sk_Log       )
#define sk_2nd_Square       (sk_2nd_Modifier | sk_Square    )
#define sk_2nd_Recip        (sk_2nd_Modifier | sk_Recip     )
#define sk_2nd_Math         (sk_2nd_Modifier | sk_Math      )
#define sk_2nd_0            (sk_2nd_Modifier | sk_0         )
#define sk_2nd_1            (sk_2nd_Modifier | sk_1         )
#define sk_2nd_4            (sk_2nd_Modifier | sk_4         )
#define sk_2nd_7            (sk_2nd_Modifier | sk_7         )
#define sk_2nd_2            (sk_2nd_Modifier | sk_2         )
#define sk_2nd_5            (sk_2nd_Modifier | sk_5         )
#define sk_2nd_8            (sk_2nd_Modifier | sk_8         )
#define sk_2nd_3            (sk_2nd_Modifier | sk_3         )
#define sk_2nd_6            (sk_2nd_Modifier | sk_6         )
#define sk_2nd_9            (sk_2nd_Modifier | sk_9         )
#define sk_2nd_Comma        (sk_2nd_Modifier | sk_Comma     )
#define sk_2nd_Sin          (sk_2nd_Modifier | sk_Sin       )
#define sk_2nd_Apps         (sk_2nd_Modifier | sk_Apps      )
#define sk_2nd_GraphVar     (sk_2nd_Modifier | sk_GraphVar  )
#define sk_2nd_DecPnt       (sk_2nd_Modifier | sk_DecPnt    )
#define sk_2nd_LParen       (sk_2nd_Modifier | sk_LParen    )
#define sk_2nd_Cos          (sk_2nd_Modifier | sk_Cos       )
#define sk_2nd_Prgm         (sk_2nd_Modifier | sk_Prgm      )
#define sk_2nd_Stat         (sk_2nd_Modifier | sk_Stat      )
#define sk_2nd_Chs          (sk_2nd_Modifier | sk_Chs       )
#define sk_2nd_RParen       (sk_2nd_Modifier | sk_RParen    )
#define sk_2nd_Tan          (sk_2nd_Modifier | sk_Tan       )
#define sk_2nd_Vars         (sk_2nd_Modifier | sk_Vars      )
#define sk_2nd_Power        (sk_2nd_Modifier | sk_Power     )


/**
 * How many seconds to wait before dimming the screen.
 */
#define APD_DIM_TIME 60
/**
 * How many seconds to wait before quitting so the OS can do its APD thing.
 */
#define APD_QUIT_TIME 120

/**
 * Checks whether the device is an 83 Premium CE with the different key layout.
 * @return Hopefully returns true if the device is an 83 Premium CE.
 */
bool Is83Premium(void);

/**
 * Waits for the user to press any key (except ON).
 * @note This runs an APD timer and may terminate the program.
 * @return The key code of the key pressed.
 */
sk_key_t GetCSC_APD(void);

/**
 * Wraps GetCSC, translating 2nd keypresses.
 * The arrow 2nd indicator will appear at CursorLocation.
 */
Key_t GetKey(void);


void Ui_Initialize(void);


void Ui_Finalize(void);


#endif /* UI_H */
