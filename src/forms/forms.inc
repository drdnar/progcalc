; Definition flags
WIDGET_DEF_NO_FLAGS = 0
WIDGET_DEF_CONTAINER = 1

; Macro courtesy of jacobly
macro ?!
	local typeid_counter, child_counter
	typeid_counter = 1
	child_counter = 0

    ;;;;
	;; Declares a widget structure
	;; @param type Name of the widget type, must match its descriptor.
	;;     E.g. a name of Label will reference Label_desc
	;; @param fields List of fields along with their type, e.g. dl someStringPointer.
	;; Additionally, the final entry may be named "children" without any type.
	;; This declares that the widget has a list of children, terminated by end, e.g. end RowList.
	;;;;
	macro defwidget? type*, fields&
		local typeid, args, vtname

		; Increment type ID counter
		typeid_counter = typeid_counter + 1
		typeid := typeid_counter

		; Declare the _vtable pointer for linking
		esc extern _#type#_desc

		args equ type?
		iterate field, fields
			match =children, field
				if % <> %%
					err 'syntax error: defwidget field children should be last'
				end if
				; handled later
			else match size= name, field
				if % <> 1
					args equ args,
				end if
				args equ args name*
			else
				err 'syntax error: defwidget field: ', `field
			end match
		end iterate

		match expand, args
			macro expand
				child_counter = child_counter + 1
				dl _#type#_desc
				iterate field, fields
					match =children, field
						local child_count
						db child_count
						child_counter =: 0

						macro end?.type?
							child_count := child_counter
							restore child_counter
							purge end?.type?
						end macro
					else
						field
					end match
				end iterate
			end macro
		end match
	end macro

	purge ?
end macro

defwidget NullWidget, 
defwidget SingleStyleOverride, db Property, db Data
defwidget FullStyleOverride, db OverrideFont, db Font, db OverrideForeground, db Foreground, db OverrideBackground, db Background, db OverrideHighlight, db Highlight, db OverrideShadow, db Shadow
defwidget RowList, children
defwidget RowItems, db Alignment, db Padding, children
defwidget DialogBox, dl Width, db Height, dl OnLoad, children
defwidget Label, dl Text
defwidget Checkbox, dl Text, dl Variable
defwidget Button, dl Text, dl MinimumWidth, db ID, dl OnPress

; Alignment options for RowItems
ROW_ITEMS_LEFT = 0
ROW_ITEMS_CENTER = 1
ROW_ITEMS_RIGHT = 2
