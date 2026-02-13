#include <windows.h>
#include <timeapi.h>
#include "clock.h"

static double clock_frequency;
static UINT min_period;
static LARGE_INTEGER start_time;

static void clock_setup(void) {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	clock_frequency = 1.0 / (double)frequency.QuadPart;
	QueryPerformanceCounter(&start_time);

	TIMECAPS tc;
	timeGetDevCaps(&tc, sizeof(tc));
	min_period = tc.wPeriodMin;
}

static double get_absolute_time(void) {
	if (!clock_frequency) {
		clock_setup();
	}

	LARGE_INTEGER now_time;
	QueryPerformanceCounter(&now_time);
	return (double)now_time.QuadPart * clock_frequency;
}

void clock_update(rsclock_t* clock) {
	if (clock->start_time != 0) {
		clock->elapsed = get_absolute_time() - clock->start_time;
	}
}

void clock_start(rsclock_t* clock) {
	clock->start_time = get_absolute_time();
	clock->elapsed = 0;
}

void clock_stop(rsclock_t* clock) {
	clock->start_time = 0;
}
