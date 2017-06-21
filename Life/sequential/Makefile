# FILE
TARGET = sequential
EXT = .c

# HABAILABLE FLAGS
# -D DEBUG prin every generation by console
# -D SAVEALL save every generation in a file
# -D SAVEINIT save the initial configuration in a file
# -D SAVELAST save the last configuration in a file
# -D TIME Save average generation calculation time

# DEBUG SETTINGS
# Matrix dimesnions (must be multiple of 4) and number of generations
D_GENERATIONS = 50
D_SIZE = 20
D_FLAGS = -D DEBUG

# EXPERIMENT SETTINGS
# Matrix dimesnions (must be multiple of 4) and number of generations
GENERATIONS = 100
SIZES = 250 500 750 1000 1250 1500
ITERATIONS = 1 2 3
FLAGS = -D TIME

# Run in debug mode
debug: clean compiledebug
	@./$(TARGET) $(D_SIZE) $(D_GENERATIONS)

compiledebug:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 $(D_FLAGS)

# Run experiment taking times
experiment: clean compileexp
	
	@for size in $(SIZES); do \
		for i in $(ITERATIONS); do \
			./$(TARGET) $$size $(GENERATIONS); \
		done; \
		echo $$size "done !"; \
	done

compileexp:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 $(FLAGS)

clean:
	rm -f $(TARGET) *.txt *.dat
	@clear