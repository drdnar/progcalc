#ifndef LABEL_H
#define LABEL_H

typedef struct Label_def
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
} Label_def;

typedef struct Label_t
{
    /**
     * Block of data common to all widgets.
     */
    Widget_t GenericData;
} Label_t;

#endif /* LABEL_H */
