TARGET = sequential
EXT = .c
# Matrix dimesnions (must be multiple of 4) and number of generations
ARGS = 20 50

run: compile
	@./$(TARGET) $(ARGS)

compile:
	@g++ $(TARGET)$(EXT) -o $(TARGET) -std=gnu++11 

clean:
	rm $(TARGET)