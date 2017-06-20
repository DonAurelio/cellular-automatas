TARGET = sequential
EXT = .c
# Matrix dimesnions (must be multiple of 4) and number of generations
GENERATIONS = 100
SIZES = 400 800 1200 1600 2000
ITERATIONS = 1 2 3

# Run one
run: clean compile
	@./$(TARGET) 20 10 > time.txt

compile:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 

# Run in debug mode
debug: clean compiledebug
	@./$(TARGET) 20 20

# DEBUG 0 to print every generation
compiledebug:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 -D DEBUG -D SAVEALL -D SAVEINIT

# Run simulation taking times
experiment: clean compileexp
	
	@for size in $(SIZES); do \
		for i in $(ITERATIONS); do \
			./$(TARGET) $$size $(GENERATIONS) >> $$size.txt; \
		done; \
		echo $$size "done !"; \
	done

compileexp:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11

clean:
	rm -f $(TARGET) *.txt *.dat
	@clear