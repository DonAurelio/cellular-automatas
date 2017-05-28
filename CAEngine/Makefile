run: compile
	@./a.out

compile:
	@python template_engine.py
	@g++ MatrixLifeCA.parsed.c -std=gnu++11 # -std=gnu++1 for chrono library

clean:
	rm -rf a.out MatrixLifeCA.parsed.c