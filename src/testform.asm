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


strOK:
	db	"OK", 0
strCancel:
	db	"Cancel", 0


public _SettingsDialog
_SettingsDialog:

DialogBox 260, 180
	RowList
		RowItems ROW_ITEMS_CENTER, 1
			Label strSettings
		end RowItems
	end RowList
	RowList
		RowItems ROW_ITEMS_LEFT, 3
			Label strDisplayBits
		end RowItems
		RowItems ROW_ITEMS_LEFT, 3
			Label strPrimaryBase
		end RowItems
		RowItems ROW_ITEMS_LEFT, 3
			Label strSecondaryBase
		end RowItems
		RowItems ROW_ITEMS_LEFT, 3
			Checkbox strShowStatusBar, _someBool
		end RowItems
		RowItems ROW_ITEMS_LEFT, 3
			Checkbox strShowStatusBar, _someBool
		end RowItems
		RowItems ROW_ITEMS_LEFT, 3
			Checkbox strShowStatusBar, _someBool
		end RowItems
	end RowList
	RowList
		RowItems ROW_ITEMS_RIGHT, 6
			Button strOK, 80, 1, 0
			Button strCancel, 80, 2, 0
			;Label strOK
			;Label strCancel
		end RowItems
	end RowList
end DialogBox

strSettings:
	db	"SETTINGS", 0
strDisplayBits:
	db	"Bits to display:", 0
strPrimaryBase:
	db	"Primary base:", 0
strSecondaryBase:
	db	"Secondary base:", 0
strShowStatusBar:
	db	"Show status bar", 0




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
