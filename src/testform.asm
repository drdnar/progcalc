	section	.text
	assume	adl = 1

include 'forms/forms.inc'


; Font IDs matching style.h
FONT_LARGE_PROP = 0
FONT_LARGE_MONO = 1
FONT_SMALL_PROP = 2
FONT_SMALL_PROP_BOLD = 3
FONT_SMALL_PROP_ALIGNED = 4

defwidget RPN_UI ,


public _RPN_UI_Dialog
_RPN_UI_Dialog:
	RPN_UI



public _SettingsDialog
_SettingsDialog:
RowList
	RowItems ROW_ITEMS_CENTER, 1
		Label strSettings
	end RowItems
	RowItems ROW_ITEMS_LEFT, 3
		Checkbox someCheckboxText, _someBool
	end RowItems
end RowList

strSettings:
	db	"SETTINGS", 0




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
Label teststr


; Temporary test checkbox
public _TestCheckbox
	extern	_someBool
_TestCheckbox:
Checkbox someCheckboxText, _someBool
; END TEMPORARY THINGY

public _TestRowList
_TestRowList:
RowList
	Label someTitle
end RowList

public _TestRowItems
_TestRowItems:
RowItems ROW_ITEMS_LEFT, 5
	Label rowItemsTestStr1
	Label rowItemsTestStr2
end RowItems

;	extern	_someBool
;	Checkbox FONT_LARGE_PROP, someCheckboxText, _someBool
;end RowList
