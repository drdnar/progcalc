#ifndef CHECKBOX_H
#define CHECKBOX_H

typedef struct
{
    /**
     * Block of data common to all widget definitions.
     */
    Widget_def GenericData;
    /**
     * ID of font to use for this label.
     */
    uint8_t Font;
    /**
     * Text to display.
     */
    char* Text;
    /**
     * Pointer to variable this checkbox edits.
     */
    bool* Variable;
} Checkbox_def;

typedef struct
{
    /**
     * Block of data common to all widgets.
     */
    Widget_t GenericData;
} Checkbox_t;

#endif /* CHECKBOX_H */
