run: compile
	@./a.out 20

compile:
	# -std=gnu++1 for chrono library
	@g++ openmp.c -std=gnu++11 

runp: compilep
	@./a.out 20

compilep:
	@g++ openmp.c -std=gnu++11 -fopenmp

clean:
	rm -rf *.out 