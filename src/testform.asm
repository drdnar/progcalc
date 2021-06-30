	section	.text
	assume	adl = 1

include 'forms/forms.inc'

defwidget RPN_UI

public _RPN_UI_Dialog
_RPN_UI_Dialog:
	RPN_UI




teststr:
    db  "Test label", 0

rowItemsTestStr1:
	db	"Item 1", 0
rowItemsTestStr2:
	db	"Item 2", 0
someTitle:
	db	"RowList title", 0
someCheckboxText:
	db	"Checkbox", 0

public _TestLabel
_TestLabel:
Label FONT_LARGE_PROP, teststr


; Temporary test checkbox
public _TestCheckbox
	extern	_someBool
_TestCheckbox:
Checkbox FONT_LARGE_PROP, someCheckboxText, _someBool
; END TEMPORARY THINGY

public _TestRowList
_TestRowList:
RowList
	Label FONT_LARGE_PROP, someTitle
end RowList

public _TestRowItems
_TestRowItems:
RowItems ROW_ITEMS_LEFT, 5
	Label FONT_LARGE_PROP, rowItemsTestStr1
	Label FONT_LARGE_PROP, rowItemsTestStr2
end RowItems

;	extern	_someBool
;	Checkbox FONT_LARGE_PROP, someCheckboxText, _someBool
;end RowList
