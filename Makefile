CC=gcc
CFLAGS=-I . -L/System/Library/Frameworks -framework GLUT -framework OpenGL
DEPS = initShader.h lib.h cubes.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: project3.o initShader.o lib.o 
	gcc -o project3 project3.o lib.o initShader.o -I . -L/System/Library/Frameworks -framework GLUT -framework OpenGL

clean:
	rm *.o