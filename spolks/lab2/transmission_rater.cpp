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
    _begin = std::chrono::high_resolution_clock::now();
}

double TransmissionRater::time_diff()
{
    auto end = std::chrono::high_resolution_clock::now();    
    auto dur = end - _begin;
    auto sec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() / 1000.0;
    return sec;
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
    return get_rate() / 8000000;
}

void TransmissionRater::print_rate()
{
    std::cout << " * Transmission rate is: " << get_rate();
}
