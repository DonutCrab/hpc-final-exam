STEPS = 10
SIZE_MULTIPLIER = 4
N_PROCESSES = 2

CC = mpicc
CFLAGS = 
LDFLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lpng

TARGET = parallel
all: $(TARGET)

$(TARGET): $(TARGET).c
	@$(CC) $(CFLAGS) $< $(LDFLAGS) -o ./out/$@

run: $(TARGET)
	@mpirun -np ${N_PROCESSES} ./out/$(TARGET) $(STEPS) $(SIZE_MULTIPLIER)

clean:
	rm -f ./out/*
