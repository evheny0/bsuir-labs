#include "transmission_rater.h"

TransmissionRater::TransmissionRater()
{
    _offset = 0;
    _transfered = 0;
    start_measure();
}

TransmissionRater::TransmissionRater(int offset)
{
    _offset = offset;
    _transfered = 0;
    start_measure();
}

TransmissionRater::~TransmissionRater()
{
    
}

void TransmissionRater::start_measure()
{
    time(&_begin);
}

double TransmissionRater::time_diff()
{
    time(&_end);
    return difftime(_end, _begin);
}

void TransmissionRater::add_bytes(long long int bytes)
{
    _transfered += bytes;
}

double TransmissionRater::get_rate()
{
    return _transfered / time_diff();
}

double TransmissionRater::get_rate_MBs()
{
    return get_rate() / 800000;
}

void TransmissionRater::print_rate()
{
    std::cout << " * Transmission rate is: " << get_rate();
}
