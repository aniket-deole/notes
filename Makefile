CPP=g++
CFLAGS=-g -rdynamic -O0 -Wall `pkg-config sigc++-2.0 gtk+-3.0 gtkmm-3.0 gdkmm-3.0 webkitgtk-3.0 sqlite3 uuid --cflags`

TGT=notes

OBJS= main.o notify.o windowbody.o maintoolbar.o leftpaneview.o notelistpaneview.o notepaneview.o databasemanager.o notedata.cc

all : ${OBJS}
	${CPP} ${CFLAGS} -o ${TGT} ${OBJS} `pkg-config sigc++-2.0 gtk+-3.0 gtkmm-3.0 webkitgtk-3.0 sqlite3 uuid --libs`

%.o : %.cc
	${CPP} ${CFLAGS} -c $<

.PHONY: clean
clean:
	rm -rf *.o notes

test: all
	./notes

install: all
	sudo cp notes /usr/bin/notes