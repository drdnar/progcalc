# ----------------------------
# Set NAME to the program name
# Set ICON to the png icon file name
# Set DESCRIPTION to display within a compatible shell
# Set COMPRESSED to "YES" to create a compressed program
# ----------------------------

NAME        ?= PROGCALC
COMPRESSED  ?= NO
ARCHIVED    ?= NO
ICON        ?= iconc.png
DESCRIPTION ?= "Programmer's Calculator"
OUTPUT_MAP  ?= YES
#OPT_MODE    ?= -Oz

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk
