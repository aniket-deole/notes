CPP=g++
CFLAGS=-g -rdynamic -O0 -Wall `pkg-config sigc++-2.0 gtk+-3.0 gtkmm-3.0 gdkmm-3.0 webkitgtk-3.0 sqlite3 uuid --cflags`-Ilib/ -Ilib/thrift/ -lssl -lcrypto -lpthread

TGT=notes
OBJS= main.o notify.o windowbody.o maintoolbar.o leftpaneview.o notelistpaneview.o notepaneview.o databasemanager.o notedata.o

all : ${OBJS}
ifdef WITHPDF
		@echo "Making with pdf"
		${CPP} ${CFLAGS} -o ${TGT} ${OBJS} `pkg-config sigc++-2.0 gtk+-3.0 gtkmm-3.0 webkitgtk-3.0 sqlite3 uuid --libs` -Ilib/ -Ilib/thrift/ -Iwithpdf/ -lssl -lcrypto -lpthread -lwkhtmltox
else
		@echo "Making without pdf"
		${CPP} ${CFLAGS} -o ${TGT} ${OBJS} `pkg-config sigc++-2.0 gtk+-3.0 gtkmm-3.0 webkitgtk-3.0 sqlite3 uuid --libs` -Ilib/ -Ilib/thrift/ -Inormal/ -lssl -lcrypto -lpthread 
endif

%.o : %.cc
ifdef WITHPDF
	${CPP} ${CFLAGS} -Iwithpdf/ -c $<
else
	${CPP} ${CFLAGS} -Inormal/ -c $<
endif

.PHONY: clean
clean:
	rm -rf *.o notes

test: all
	./notes

install: all
	cp notes /usr/bin/notes
