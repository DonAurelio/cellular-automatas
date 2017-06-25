# FILE
TARGET = openacc
EXT = .c
COMPILER = pgc++

# HABAILABLE FLAGS
# -D SAVEINIT save the initial configuration in a file
# -D SAVELAST save the last configuration in a file
# -D TIME Save average generation calculation time
# -O3 For compiler optimizations
# -std=c++11 to get habailable the chrono library
# -acc OpenAcc Oprimizations
# -ta=tesla:cc30 Target architecture (NVDIA) and compute capability 
# -ta=tesla:cc30,time All of the above plus give a time execution feedback
# -Minfo=accel OpenAcc code transformation feedback

# DEBUG SETTINGS
D_GENERATIONS = 100
D_SIZE = 1024
D_FLAGS = -D TIME -D SAVEINIT -D SAVELAST
D_CFLAGS = -std=c++11 -O3 -acc -ta=tesla:cc30,time -Minfo=accel
# D_CFLAGS = -std=c++11 -O3 -acc -ta=nvidia,time -Minfo=accel

# EXPERIMENT SETTINGS
GENERATIONS = 100
SIZES = 250 500 750 1000 1250 1500
ITERATIONS = 1 2 3
FLAGS = -D TIME -D SAVEINIT -D SAVELAST 
CFLAGS = -std=c++11 -O3 -acc -ta=tesla:cc30,time -Minfo=accel
# CFLAGS = -std=c++11 -O3 -acc -ta=nvidia,time -Minfo=accel

# Run in debug mode
debug: clean compiledebug
	@./$(TARGET)

compiledebug:
	@$(COMPILER) $(TARGET)$(EXT) -o $(TARGET) \
	-D SIZE=$(D_SIZE) \
	-D GENERATIONS=$(D_GENERATIONS) \
	$(D_CFLAGS) \
	$(D_FLAGS) 

# Run experiment taking times
experiment: clean
	
	@for size in $(SIZES); do \
		for i in $(ITERATIONS); do \
			@$(COMPILER) $(TARGET)$(EXT) -o $(TARGET) \
				-D SIZE=$(D_SIZE) \
				-D GENERATIONS=$(D_GENERATIONS) \
				$(CFLAGS) \
				$(FLAGS)
			./$(TARGET) $$size $(GENERATIONS); \
		done; \
		echo $$size "done !"; \
	done

clean:
	@rm -f $(TARGET) *.txt *.dat
	@clear


nvidia_device_info:
	@pgaccelinfo

# This feature do not work in the pgicontainer 
# it need the nvidia-settings that comes with 
# the nvidia driver that is not present into the container.
nvidia_device_top:
	@watch -n 0,1 ./tools/gpumonitor.sh

# This feature do not work properly
intel_device_top:
	@sudo intel_gpu_top
