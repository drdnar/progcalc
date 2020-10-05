; Macro courtesy of jacobly
macro ?!
	local typeid_counter, child_counter
	typeid_counter = 1
	child_counter = 0

	macro defwidget? type*, fields&
		local typeid, args

		typeid_counter = typeid_counter + 1
		typeid := typeid_counter

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

				db typeid
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

defwidget RowList, children
defwidget RowItems, db Alignment, children
defwidget Label, db Font, dl Text
defwidget Checkbox, db Font, dl Text, dl Variable
