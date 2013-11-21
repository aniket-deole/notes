CPP=g++
CFLAGS=-g -rdynamic -O0 -Wall `pkg-config sigc++-2.0 gtk+-3.0 gtkmm-3.0 webkitgtk-3.0 sqlite3 --cflags`

TGT=notify

OBJS= main.o notify.o windowbody.o maintoolbar.o leftpaneview.o

all : ${OBJS}
	${CPP} ${CFLAGS} -o ${TGT} ${OBJS} `pkg-config sigc++-2.0 gtk+-3.0 gtkmm-3.0  webkitgtk-3.0 sqlite3 --libs`

%.o : %.cc
	${CPP} ${CFLAGS} -c $<

.PHONY: clean
clean:
	rm -rf *.o notify

test: all
	./notify
