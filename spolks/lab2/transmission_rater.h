#ifndef TRANSMISSION_RATER_H
#define TRANSMISSION_RATER_H

#include <ctime>
#include <iostream>
#include <stdio.h>


class TransmissionRater {
    long long int _offset;
    long long int _transfered;
    time_t _begin, _end;

  public:
    TransmissionRater();
    TransmissionRater(int offset);
    ~TransmissionRater();
    void start_measure();
    void add_bytes(long long int bytes);
    void print_rate();
    double get_rate();
    double get_rate_MBs();
    double time_diff();
};

#endif // TRANSMISSION_RATER_H
