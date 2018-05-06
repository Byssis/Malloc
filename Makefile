all:
	gcc -c rand.c
	gcc -c mylloc_ff.c
	gcc -c malloc_split.c
	gcc -c mylloc_bf.c
	gcc -c mylloc_wf.c
	gcc -o bench_ff.o mylloc_ff.o rand.o benchbuffer.c -lm
	gcc -o bench_bf.o mylloc_bf.o rand.o benchbuffer.c -lm
	gcc -o bench_wf.o mylloc_wf.o rand.o benchbuffer.c -lm
	gcc -o bench_normal.o rand.o benchbuffer.c -lm
	gcc -o bench_split.o malloc_split.o rand.o benchbuffer.c -lm


clean:
	rm *.o

strace:
	strace ./bench_normal.o 2>&1 | grep brk | wc -l
	strace ./bench_ff.o 2>&1 | grep brk | wc -l
	strace ./bench_bf.o 2>&1 | grep brk | wc -l
	strace ./bench_wf.o 2>&1 | grep brk | wc -l
	strace ./bench_split.o 2>&1 | grep brk | wc -l

test:
	./bench_normal.o
	./bench_ff.o
	./bench_bf.o
	./bench_wf.o
	./bench_split.o 
