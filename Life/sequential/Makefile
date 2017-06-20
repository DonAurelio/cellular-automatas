TARGET = sequential
EXT = .c
# Matrix dimesnions (must be multiple of 4) and number of generations
GENERATIONS = 100
# SIZES = 400 800 1200 1600 2000
SIZES = 250 500 750 1000 1250
ITERATIONS = 1 2 3

# Run one
run: clean compile
	@./$(TARGET) 20 10

compile:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 

# Run in debug mode
debug: clean compiledebug
	@./$(TARGET) 1200 100

# HABAILABLE FLAGS
# -D DEBUG prin every generation by console
# -D SAVEALL save every generation in a file
# -D SAVEINIT save the initial configuration in a file
# -D TIME Save average generation calculation time
compiledebug:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11  -D TIME

# Run simulation taking times
experiment: clean compileexp
	
	@for size in $(SIZES); do \
		for i in $(ITERATIONS); do \
			./$(TARGET) $$size $(GENERATIONS) \
		done; \
		echo $$size "done !"; \
	done

compileexp:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 -D TIME

clean:
	rm -f $(TARGET) *.txt *.dat
	@clear