CPP=g++
CFLAGS=-g -rdynamic -O0 -Wall `pkg-config sigc++-2.0 gtk+-3.0 gtkmm-3.0 gdkmm-3.0 webkitgtk-3.0 sqlite3 uuid --cflags`-Ilib/ -Ilib/thrift/ -lssl -lcrypto -lpthread

TGT=notes

OBJS= main.o notify.o windowbody.o maintoolbar.o leftpaneview.o notelistpaneview.o notepaneview.o databasemanager.o notedata.o

all : ${OBJS}
	${CPP} ${CFLAGS} -o ${TGT} ${OBJS} `pkg-config sigc++-2.0 gtk+-3.0 gtkmm-3.0 webkitgtk-3.0 sqlite3 uuid --libs` -Ilib/ -Ilib/thrift/ -lssl -lcrypto -lpthread

%.o : %.cc
	${CPP} ${CFLAGS} -c $<

.PHONY: clean
clean:
	rm -rf *.o notes

test: all
	./notes

install: all
	cp notes /usr/bin/notes
