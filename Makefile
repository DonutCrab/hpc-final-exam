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

benchmark: ${TARGET}
	@echo ",4,5,6,7,8,9,10,11,12,13,14,16" > ./out/benchmark.csv

# 1 thread
	mpirun -np 1 ./out/$(TARGET) 100 4 b
	mpirun -np 1 ./out/$(TARGET) 100 5 b
	mpirun -np 1 ./out/$(TARGET) 100 6 b
	mpirun -np 1 ./out/$(TARGET) 100 7 b
	mpirun -np 1 ./out/$(TARGET) 100 8 b
	mpirun -np 1 ./out/$(TARGET) 100 9 b
	mpirun -np 1 ./out/$(TARGET) 100 10 b
	mpirun -np 1 ./out/$(TARGET) 100 12 b
	mpirun -np 1 ./out/$(TARGET) 100 13 b
	mpirun -np 1 ./out/$(TARGET) 100 14 b
	mpirun -np 1 ./out/$(TARGET) 100 15 b
	mpirun -np 1 ./out/$(TARGET) 100 16 b

# 2 threads
	@echo "" >> ./out/benchmark.csv

	mpirun -np 2 ./out/$(TARGET) 100 4 b
	mpirun -np 2 ./out/$(TARGET) 100 5 b
	mpirun -np 2 ./out/$(TARGET) 100 6 b
	mpirun -np 2 ./out/$(TARGET) 100 7 b
	mpirun -np 2 ./out/$(TARGET) 100 8 b
	mpirun -np 2 ./out/$(TARGET) 100 9 b
	mpirun -np 2 ./out/$(TARGET) 100 10 b
	mpirun -np 2 ./out/$(TARGET) 100 12 b
	mpirun -np 2 ./out/$(TARGET) 100 13 b
	mpirun -np 2 ./out/$(TARGET) 100 14 b
	mpirun -np 2 ./out/$(TARGET) 100 15 b
	mpirun -np 2 ./out/$(TARGET) 100 16 b

# 3 threads
	@echo "" >> ./out/benchmark.csv

	mpirun -np 3 ./out/$(TARGET) 100 4 b
	mpirun -np 3 ./out/$(TARGET) 100 5 b
	mpirun -np 3 ./out/$(TARGET) 100 6 b
	mpirun -np 3 ./out/$(TARGET) 100 7 b
	mpirun -np 3 ./out/$(TARGET) 100 8 b
	mpirun -np 3 ./out/$(TARGET) 100 9 b
	mpirun -np 3 ./out/$(TARGET) 100 10 b
	mpirun -np 3 ./out/$(TARGET) 100 12 b
	mpirun -np 3 ./out/$(TARGET) 100 13 b
	mpirun -np 3 ./out/$(TARGET) 100 14 b
	mpirun -np 3 ./out/$(TARGET) 100 15 b
	mpirun -np 3 ./out/$(TARGET) 100 16 b

# 4 threads
	@echo "" >> ./out/benchmark.csv

	mpirun -np 4 ./out/$(TARGET) 100 4 b
	mpirun -np 4 ./out/$(TARGET) 100 5 b
	mpirun -np 4 ./out/$(TARGET) 100 6 b
	mpirun -np 4 ./out/$(TARGET) 100 7 b
	mpirun -np 4 ./out/$(TARGET) 100 8 b
	mpirun -np 4 ./out/$(TARGET) 100 9 b
	mpirun -np 4 ./out/$(TARGET) 100 10 b
	mpirun -np 4 ./out/$(TARGET) 100 12 b
	mpirun -np 4 ./out/$(TARGET) 100 13 b
	mpirun -np 4 ./out/$(TARGET) 100 14 b
	mpirun -np 4 ./out/$(TARGET) 100 15 b
	mpirun -np 4 ./out/$(TARGET) 100 16 b

# 5 threads
	@echo "" >> ./out/benchmark.csv

	mpirun -np 5 ./out/$(TARGET) 100 4 b
	mpirun -np 5 ./out/$(TARGET) 100 5 b
	mpirun -np 5 ./out/$(TARGET) 100 6 b
	mpirun -np 5 ./out/$(TARGET) 100 7 b
	mpirun -np 5 ./out/$(TARGET) 100 8 b
	mpirun -np 5 ./out/$(TARGET) 100 9 b
	mpirun -np 5 ./out/$(TARGET) 100 10 b
	mpirun -np 5 ./out/$(TARGET) 100 12 b
	mpirun -np 5 ./out/$(TARGET) 100 13 b
	mpirun -np 5 ./out/$(TARGET) 100 14 b
	mpirun -np 5 ./out/$(TARGET) 100 15 b
	mpirun -np 5 ./out/$(TARGET) 100 16 b

# 6 threads
	@echo "" >> ./out/benchmark.csv

	mpirun -np 6 ./out/$(TARGET) 100 4 b
	mpirun -np 6 ./out/$(TARGET) 100 5 b
	mpirun -np 6 ./out/$(TARGET) 100 6 b
	mpirun -np 6 ./out/$(TARGET) 100 7 b
	mpirun -np 6 ./out/$(TARGET) 100 8 b
	mpirun -np 6 ./out/$(TARGET) 100 9 b
	mpirun -np 6 ./out/$(TARGET) 100 10 b
	mpirun -np 6 ./out/$(TARGET) 100 12 b
	mpirun -np 6 ./out/$(TARGET) 100 13 b
	mpirun -np 6 ./out/$(TARGET) 100 14 b
	mpirun -np 6 ./out/$(TARGET) 100 15 b
	mpirun -np 6 ./out/$(TARGET) 100 16 b

# 7 threads
	@echo "" >> ./out/benchmark.csv

	mpirun -np 7 ./out/$(TARGET) 100 4 b
	mpirun -np 7 ./out/$(TARGET) 100 5 b
	mpirun -np 7 ./out/$(TARGET) 100 6 b
	mpirun -np 7 ./out/$(TARGET) 100 7 b
	mpirun -np 7 ./out/$(TARGET) 100 8 b
	mpirun -np 7 ./out/$(TARGET) 100 9 b
	mpirun -np 7 ./out/$(TARGET) 100 10 b
	mpirun -np 7 ./out/$(TARGET) 100 12 b
	mpirun -np 7 ./out/$(TARGET) 100 13 b
	mpirun -np 7 ./out/$(TARGET) 100 14 b
	mpirun -np 7 ./out/$(TARGET) 100 15 b
	mpirun -np 7 ./out/$(TARGET) 100 16 b

# 8 threads
	@echo "" >> ./out/benchmark.csv

	mpirun -np 8 ./out/$(TARGET) 100 4 b
	mpirun -np 8 ./out/$(TARGET) 100 5 b
	mpirun -np 8 ./out/$(TARGET) 100 6 b
	mpirun -np 8 ./out/$(TARGET) 100 7 b
	mpirun -np 8 ./out/$(TARGET) 100 8 b
	mpirun -np 8 ./out/$(TARGET) 100 9 b
	mpirun -np 8 ./out/$(TARGET) 100 10 b
	mpirun -np 8 ./out/$(TARGET) 100 12 b
	mpirun -np 8 ./out/$(TARGET) 100 13 b
	mpirun -np 8 ./out/$(TARGET) 100 14 b
	mpirun -np 8 ./out/$(TARGET) 100 15 b
	mpirun -np 8 ./out/$(TARGET) 100 16 b

clean:
	rm -f ./out/*
