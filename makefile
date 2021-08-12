# ----------------------------
# Set NAME to the program name
# Set ICON to the png icon file name
# Set DESCRIPTION to display within a compatible shell
# Set COMPRESSED to "YES" to create a compressed program
# ----------------------------

NAME        = PROGCALC
COMPRESSED  ?= YES
ARCHIVED    ?= YES
ICON        ?= iconc.png
DESCRIPTION ?= "Programmer's Calculator"
OUTPUT_MAP  ?= YES
#OPT_MODE    ?= -Oz
VERSION_NUMBER = "1.2"
VERSION_DATE = "12 August 2021"

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz -DVERSION_NUMBER=\"$(VERSION_NUMBER)\" -DVERSION_DATE=\"$(VERSION_DATE)\"

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk
