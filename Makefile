parser: lex.yy.c interpreter.tab.o main.cc
	g++ -g -std=c++11 -DYYDEBUG=1 -oparser interpreter.tab.o lex.yy.c main.cc
interpreter.tab.o: interpreter.tab.cc headers/*.h
	g++ -g -std=c++11 -c interpreter.tab.cc headers/*.h
interpreter.tab.cc: interpreter.yy
	bison -v interpreter.yy
lex.yy.c: interpreter.ll interpreter.tab.cc
	flex interpreter.ll
clean:
	rm -f interpreter.tab.* lex.yy.c* parser