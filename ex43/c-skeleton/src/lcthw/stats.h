/**
 * @file stats.h
 * @brief A Simple Statistics Engine header
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-23
 */

#ifndef _stats_h_
#define _stats_h_

typedef struct Stats {
	double sum;
	double sumsq;
	unsigned long n;
	double min;
	double max;
} Stats;

Stats *Stats_recreate(double sum, double sumsq, unsigned long n, double min, double max);
Stats *Stats_create(void);
double Stats_mean(Stats *st);
double Stats_stddev(Stats *st);
void Stats_sample(Stats *st, double s);
void Stats_dump(Stats *st);

#endif /* _stats_h_ */
