CC=gcc
CFLAGS=-Os
DEPS=utf8encode.h utf8decode.h parse_codepoint.h handle_ferr.h 
OBJ=utf8encode.o utf8decode.o parse_codepoint.o handle_ferr.o

all: codepoint2utf8 codepoint-list codepoint-range utf8encode-test utf8file utf8parse 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

codepoint2utf8: codepoint2utf8.o parse_codepoint.o utf8encode.o
	$(CC) -o codepoint2utf8 codepoint2utf8.o parse_codepoint.o utf8encode.o $(CFLAGS)

codepoint-list: codepoint-list.o parse_codepoint.o utf8encode.o
	$(CC) -o codepoint-list codepoint-list.o parse_codepoint.o utf8encode.o $(CFLAGS)

codepoint-range: codepoint-range.o parse_codepoint.o utf8encode.o
	$(CC) -o codepoint-range codepoint-range.o parse_codepoint.o utf8encode.o $(CFLAGS)

utf8encode-test: utf8encode-test.o utf8encode.o
	$(CC) -o utf8encode-test utf8encode-test.o utf8encode.o $(CFLAGS)

utf8file: utf8file.o handle_ferr.o utf8decode.o
	$(CC) -o utf8file utf8file.o handle_ferr.o utf8decode.o $(CFLAGS)

utf8parse: utf8parse.o utf8decode.o
	$(CC) -o utf8parse utf8parse.o utf8decode.o $(CFLAGS)
