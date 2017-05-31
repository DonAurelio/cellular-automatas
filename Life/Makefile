run: compile
	@./a.out 20

compile:
	# -std=gnu++1 for chrono library
	@g++ sequential.c -std=gnu++11 

runp: compilep
	@./a.out 20

compilep:
	@g++ sequential.c -std=gnu++11 -fopenmp

clean:
	rm -rf *.out 