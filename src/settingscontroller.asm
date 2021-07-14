	section	.text
	assume	adl = 1

include 'widgets.inc'

    extern _HandleOKCancel
    extern _LoadSettings
    extern _TemporarySettings

DisplayBits = 0
PrimaryBase = 1
SecondaryBase = 2
AlwaysShowHex = 3
AlwaysShowDec = 4
AlwaysShowOct = 5
AlwaysShowBin = 6
StatusBarEnabled = 7

strOK:
	db	"OK", 0
strCancel:
	db	"Cancel", 0


public _SettingsDialog
_SettingsDialog:

DialogBox 260, 180, _LoadSettings
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
			Checkbox strShowStatusBar, _TemporarySettings + StatusBarEnabled
		end RowItems
	end RowList
	RowList
		RowItems ROW_ITEMS_RIGHT, 6
			Button strOK, 80, 1, _HandleOKCancel
			Button strCancel, 80, 2, _HandleOKCancel
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
