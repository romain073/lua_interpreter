parser: lex.yy.c interpreter.tab.o main.cc
	g++ -Wall -g -std=c++11 -DYYDEBUG=1 -oint interpreter.tab.o lex.yy.c main.cc
interpreter.tab.o: interpreter.tab.cc headers/*.cpp
	g++ -Wall -g -std=c++11 -c interpreter.tab.cc headers/*.cpp
interpreter.tab.cc: interpreter.yy
	bison -v interpreter.yy
lex.yy.c: interpreter.ll interpreter.tab.cc
	flex interpreter.ll
clean:
	rm -f interpreter.tab.* lex.yy.c* parser