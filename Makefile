all: enforx test-enforx

enforx: enforx.l enforx.y enforx.h
	bison -d enforx.y
	flex enforx.l
	gcc enforx.tab.c lex.yy.c -lfl -o $@

test-enforx: test-enforx.c enforx.c enforx.h
	gcc test-enforx.c enforx.c -o $@
