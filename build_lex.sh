rm -rf generated
mkdir "generated"
bison -d commands.y
mv ./commands.tab.c ./generated
mv ./commands.tab.h ./generated
flex -o ./generated/lex.yy.c names.l
bison -d -v commands.y -o generated/commands.c
cc -c ./generated/lex.yy.c -o ./generated/lex.yy.o
c++ ./generated/lex.yy.o ./generated/commands.c -o ./generated/commands

