runsc: compilesc
	@./sctca.out
compilesc:
	@g++ sctca.c -std=gnu++11 -o sctca.out

runmc: compilemc
	@./mctca.out

compilemc:
	@g++ mctca.c -std=gnu++11 -o mctca.out

clean:
	@rm -rf *.out

