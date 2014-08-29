.PHONY: tags

all:
	g++ core.cc -o core -lpthread -lboost_program_options

tags:
	ctags --c++-kinds=+p --fields=+iaS --extra=+q --language-force=C++ -R .

clean:
	rm -f tags
	rm -f *.o
	rm -f core
