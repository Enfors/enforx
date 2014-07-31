all: enforx test-enforx

enforx: enforx.l parser.y enforx.h
	bison -d parser.y
	flex enforx.l
	gcc parser.tab.c lex.yy.c enforx.c -lfl -o $@

test-enforx: test-enforx.c enforx.c enforx.h
	gcc -g test-enforx.c enforx.c -o $@

clean:
	rm -f enforx test-enforx
