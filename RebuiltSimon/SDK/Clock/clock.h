#ifndef CLOCK_H_
#define CLOCK_H_

typedef struct rsclock_t {
	double start_time;
	double elapsed;
} rsclock_t;

void clock_update(rsclock_t* clock);
void clock_start(rsclock_t* clock);
void clock_stop(rsclock_t* clock);

#endif /* CLOCK_H_ */