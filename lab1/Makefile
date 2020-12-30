EXE=input_generator

all: build

build: inputs.c
	gcc -o ${EXE} inputs.c

clean:
	find . -maxdepth 1 -type f -name "*.csv" -delete 
	rm -rf ${EXE} courses
