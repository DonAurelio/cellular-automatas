TARGET = main
EXT = .c

COMPILER = gcc
CFLAGS = -O3

compile:
	$(COMPILER) $(TARGET)$(EXT) -o $(TARGET) $(CFLAGS)

run: compile
	./$(TARGET)

clean:
	rm -rf $(TARGET) heat.dat

plot_bool:
	@gnuplot -p -e "filename='bool.dat'" ../bool/bool.gnu

plot_heat:
	@gnuplot -p -e "filename='heat.dat'" heat.gnu

