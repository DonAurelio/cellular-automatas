# FILE
TARGET = openmp
EXT = .c
COMPILER = g++-5

# HABAILABLE COMPILATION FLAGS
# -D DEBUG prin every generation by console
# -D SAVEALL save every generation in a file
# -D SAVEINIT save the initial configuration in a file
# -D SAVELAST save the last configuration in a file
# -D TIME Save average generation calculation time
# -D PARALLEL is 1 the parallel approach taken is OMP Parallel for directive
# -D PARALLEL is 2 the parallel approach taken is Grid per thread
# Sequentila otherwise
# -O3 For compiler optimizations

# DEBUG SETTINGS
# Matrix dimesnions (must be multiple of 4) and number of generations
D_GENERATIONS = 50
D_SIZE = 20
D_FLAGS = -D PARALLEL=1 -D NUM_THREADS=2 -D DEBUG

# EXPERIMENT SETTINGS
# Matrix dimesnions (must be multiple of 4) and number of generations
GENERATIONS = 100
SIZES = 250 500 750 1000 1250 1500
ITERATIONS = 1 2 3
FLAGS = -D PARALLEL=1 -D NUM_THREADS=4 -D TIME -D SAVEINIT -D SAVELAST -O3

# Run in debug mode
debug: clean compiledebug
	@./$(TARGET) $(D_SIZE) $(D_GENERATIONS)

compiledebug:
	@$(COMPILER) $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 -fopenmp $(D_FLAGS)


# Run simulation taking times
experiment: clean compileexp
	
	@for size in $(SIZES); do \
		for i in $(ITERATIONS); do \
			./$(TARGET) $$size $(GENERATIONS); \
		done; \
		echo $$size "done !"; \
	done

compileexp:
	@$(COMPILER) $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 -fopenmp $(FLAGS)

clean:
	rm -f $(TARGET) *.txt *.dat
	@clear