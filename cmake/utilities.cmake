cmake_minimum_required(VERSION 3.12)

function(append_prefixed_items_to_list prefix out)
	set(items_to_append ${ARGN})
	list(TRANSFORM items_to_append PREPEND ${prefix})

	set(new_out ${${out}})
	list(APPEND new_out ${items_to_append})

	set(${out} ${new_out} PARENT_SCOPE)
endfunction()
