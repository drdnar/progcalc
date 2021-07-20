	section	.text
	assume	adl = 1

include 'widgets.inc'

    extern _HandleOKCancel
    extern _LoadSettings
    extern _TemporarySettings
	extern _BaseLongNames
	extern _DisplayBitsNames
	extern _LoadErrorDialogMessage
	extern _CloseErrorDialog

DisplayBits = 0
PrimaryBase = 1
SecondaryBase = 2
AlwaysShowHex = 3
AlwaysShowDec = 4
AlwaysShowOct = 5
AlwaysShowBin = 6
StatusBarEnabled = 7

	public _SettingsDialog
	public _ErrorDialog
_SettingsDialog:
	DialogBox 260, 180, _LoadSettings
		RowList
			RowItems ROW_ITEMS_CENTER, 1
				Label strSettings
			end RowItems
		end RowList
		RowList
			RowItems ROW_ITEMS_LEFT, 3
				Label strPrimaryBase
				EnumSelector _BaseLongNames, 1, _TemporarySettings + PrimaryBase
			end RowItems
			RowItems ROW_ITEMS_LEFT, 3
				Label strSecondaryBase
				EnumSelector _BaseLongNames, 1, _TemporarySettings + SecondaryBase
			end RowItems
			RowItems ROW_ITEMS_LEFT, 3
				Label strDisplayBits
				EnumSelector _DisplayBitsNames, 1, _TemporarySettings + DisplayBits
			end RowItems
			RowItems ROW_ITEMS_LEFT, 3
				Checkbox strShowStatusBar, _TemporarySettings + StatusBarEnabled
			end RowItems
		end RowList
		RowList
			RowItems ROW_ITEMS_RIGHT, 6
				Button strOK, 80, 1, _HandleOKCancel
				Button strCancel, 80, 0, _HandleOKCancel
			end RowItems
		end RowList
	end DialogBox
	NullWidget

_ErrorDialog:
	DialogBox 240, 120, _LoadErrorDialogMessage
		RowList
			RowItems ROW_ITEMS_CENTER, 1
				Label strInvalidSettings
			end RowItems
		end RowList
		RowList
			Label strInvalidSettings
		end RowList
		RowList
			RowItems ROW_ITEMS_RIGHT, 6
				Button strOK, 80, 1, _CloseErrorDialog
			end RowItems
		end RowList
	end DialogBox
	NullWidget

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
strOK:
	db	"OK", 0
strCancel:
	db	"Cancel", 0
strInvalidSettings:
	db	"INVALID SETTINGS", 0