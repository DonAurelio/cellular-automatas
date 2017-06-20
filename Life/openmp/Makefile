# EXPERIMENT SETTINGS
TARGET = openmp
EXT = .c
# Matrix dimesnions (must be multiple of 4) and number of generations
GENERATIONS = 100
SIZES = 250 500 750 1000 1250
ITERATIONS = 1 2 3
PARALLEL_APPROACH = 1

# HABAILABLE COMPILATION FLAGS
# -D DEBUG prin every generation by console
# -D SAVEALL save every generation in a file
# -D SAVEINIT save the initial configuration in a file
# -D SAVELAST save the last configuration in a file
# -D TIME Save average generation calculation time
# -D PARALLEL is 1 the parallel approach taken is cell per thread
# -D PARALLEL is 2 the parallel approach taken is Grid per thread
# -D PARALLEL is 3 the parallel approach taken is OMP Parallel for directive
# Sequentila otherwise

# Run one
run: clean compile
	@./$(TARGET) 20 10

compile:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 

# Run in debug mode
debug: clean compiledebug
	@./$(TARGET) 20 20

compiledebug:
	@g++ $(TARGET)$(EXT) -o $(TARGET) \
	-std=gnu++11 \
	-fopenmp \
	-D DEBUG \
	-D SAVEINIT \
	-D SAVELAST \
	-D TIME \
	-D PARALLEL=$(PARALLEL_APPROACH)

# Run simulation taking times
experiment: clean compileexp
	
	@for size in $(SIZES); do \
		for i in $(ITERATIONS); do \
			./$(TARGET) $$size $(GENERATIONS); \
		done; \
		echo $$size "done !"; \
	done

compileexp:
	@g++ $(TARGET)$(EXT) -o $(TARGET) \
	-std=gnu++11 \
	-fopenmp \
	-D TIME \
	-D PARALLEL=$(PARALLEL_APPROACH)

clean:
	rm -f $(TARGET) *.txt *.dat
	@clear