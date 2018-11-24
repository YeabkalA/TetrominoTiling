#include <string>
#include <algorithm>
#include <initializer_list>
#include <fstream>
#include <cstdio>
#include <memory>
#include <stdexcept>


bool NumericCheck(std::string q, int* ret) {
    char* convered;
    int val = strtol(q.c_str(), &convered, 10);
    *ret = val;
    return *convered == 0;
}

int Len(int num) {
    int size = 0;
    if(num == 0) return 1;
    while(num != 0) {
        num/=10;
        size++;
    }
    return size;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}