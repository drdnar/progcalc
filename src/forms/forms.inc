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
	;; @param type Name of the widget type prefix, must match the name in C, e.g. Label
	;; @param defflags Should be WIDGET_DEF_CONTAINER if the Widget is a Container.
	;; @param fields List of fields along with their type, e.g. dl someStringPointer.
	;; Additionally, the final entry may be named "children" without any type.
	;; This declares that the widget has a list of children, terminated by end, e.g. end RowList.
	;;;;
	macro defwidget? type*, userflags:0, fields&
		local typeid, args, vtname, defflags
		defflags = userflags

		; Increment type ID counter
		typeid_counter = typeid_counter + 1
		typeid := typeid_counter

		; Declare the _vtable pointer for linking
		esc extern _#type#_vtable

		args equ type?
		iterate field, fields
			match =children, field
				if % <> %%
					err 'syntax error: defwidget field children should be last'
				else
					defflags = (defflags or WIDGET_DEF_CONTAINER)
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

				db typeid
				db defflags
				dl _#type#_vtable
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

; These must be declared in the same order as in forms.h so that the type IDs match.
defwidget RowList, , children
defwidget RowItems, , db Alignment, children
defwidget Label, , db Font, dl Text
defwidget Checkbox, , db Font, dl Text, dl Variable

; Font IDs matching style.h
FONT_LARGE_PROP = 0
FONT_LARGE_MONO = 1
FONT_SMALL_PROP = 2
FONT_SMALL_PROP_BOLD = 3
FONT_SMALL_PROP_ALIGNED = 4

; Alignment options for RowItems
ROW_ITEMS_LEFT = 0
ROW_ITEMS_RIGHT = 1
