TARGET = sequential
EXT = .c
# Matrix dimesnions (must be multiple of 4) and number of generations
GENERATIONS = 100
# ITERATIONS = 1 2 3 4 5 6 7 8 9 10
SIZES = 400 800 1200 1600 2000
ITERATIONS = 1 2 3

run: clean compile
	@./$(TARGET) $(ARGS) 

compile:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 -D DEBUG

test: clean compiletest
	
	@rm -f time.txt
	@for size in $(SIZES); do \
		for i in $(ITERATIONS); do \
			./$(TARGET) $$size $(GENERATIONS) >> $$size.txt; \
		done; \
		echo $$size "done !"; \
	done

compiletest:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11

clean:
	rm -f $(TARGET) *.txt
	@clear