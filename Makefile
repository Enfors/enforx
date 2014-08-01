CFLAGS = -g

all: enforx test-enforx

enforx: lexer.l parser.y enforx.c enforx.h
	bison -d parser.y
	flex lexer.l
	gcc $(CFLAGS) parser.tab.c lex.yy.c enforx.c -lfl -o $@

test-enforx: test-enforx.c enforx.c enforx.h
	gcc $(CFLAGS) test-enforx.c enforx.c -o $@

clean:
	rm -f enforx test-enforx
