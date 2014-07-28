enforx: enforx.l enforx.y
	bison -d enforx.y
	flex enforx.l
	gcc enforx.tab.c lex.yy.c -lfl -o $@
