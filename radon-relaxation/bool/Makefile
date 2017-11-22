TARGET = main
EXT = .c

COMPILER = gcc
CFLAGS = -O3

compile:
	$(COMPILER) $(TARGET)$(EXT) -o $(TARGET) $(CFLAGS)

run: clean compile
	./$(TARGET)

clean:
	rm -rf $(TARGET) matrix.dat

plot:
	@gnuplot -p -e "filename='matrix.dat'" draw.gnu


