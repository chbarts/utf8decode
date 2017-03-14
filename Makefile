CC=gcc
CFLAGS=-O3 -march=native -flto
DEPS=utf8encode.h utf8decode.h parse_codepoint.h handle_ferr.h 

all: codepoint2utf8 codepoint-list codepoint-range utf8encode-test utf8file utf8parse isutf8 rot32768

codepoint2utf8: codepoint2utf8.c parse_codepoint.c utf8encode.c
	$(CC) -o codepoint2utf8 codepoint2utf8.c parse_codepoint.c utf8encode.c $(CFLAGS)

codepoint-list: codepoint-list.c parse_codepoint.c utf8encode.c
	$(CC) -o codepoint-list codepoint-list.c parse_codepoint.c utf8encode.c $(CFLAGS)

codepoint-range: codepoint-range.c parse_codepoint.c utf8encode.c
	$(CC) -o codepoint-range codepoint-range.c parse_codepoint.c utf8encode.c $(CFLAGS)

utf8encode-test: utf8encode-test.c utf8encode.c
	$(CC) -o utf8encode-test utf8encode-test.c utf8encode.c $(CFLAGS)

utf8file: utf8file.c handle_ferr.c utf8decode.c
	$(CC) -o utf8file utf8file.c handle_ferr.c utf8decode.c $(CFLAGS)

utf8parse: utf8parse.c utf8decode.c
	$(CC) -o utf8parse utf8parse.c utf8decode.c $(CFLAGS)

isutf8: isutf8.c handle_ferr.c utf8decode.c
	$(CC) -o isutf8 isutf8.c handle_ferr.c utf8decode.c $(CFLAGS)

rot32768: rot32768.c handle_ferr.c utf8decode.c utf8encode.c
	$(CC) -o rot32768 rot32768.c handle_ferr.c utf8decode.c utf8encode.c $(CFLAGS)

clean:
	rm codepoint2utf8 codepoint-list codepoint-range utf8encode-test utf8file utf8parse isutf8 rot32768
