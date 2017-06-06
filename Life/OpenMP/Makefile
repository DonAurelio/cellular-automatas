# Variable definition
TARGET = sec
EXT = .c
ARGS = 20

run: compile
	@./$(TARGET) $(ARGS)

compile:
	@g++ $(TARGET)$(EXT) -std=gnu++11 

clean:
	rm -rf *.out 