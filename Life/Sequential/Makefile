TARGET = sequential
EXT = .c
# Matrix dimesnions (must be multiple of 4) and number of generations
ARGS = 20 10

run: compile
	@./$(TARGET) $(ARGS) 

compile:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 -D DEBUG

test: compiletest
	for iteration in 1 2 ; do \
		@./$(TARGET) $(ARGS) >> time.txt ; \
	done

compiletest:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11

clean:
	rm $(TARGET)