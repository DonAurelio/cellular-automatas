# Variable definition
TARGET = openmp
EXT = .c
ARGS = 20 100

run: compile
	@./$(TARGET) $(ARGS)

compile:
	@g++ $(TARGET)$(EXT) -std=gnu++11 -fopenmp

clean:
	rm -rf *.out 