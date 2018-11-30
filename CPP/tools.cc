#include <string>
#include <algorithm>
#include <initializer_list>
#include <unordered_set>
#include <fstream>
#include <cstdio>
#include <memory>
#include <array>
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

bool isSatisfiable() {
    std::string result = exec("minisat problem.cnf");
    exec("rm problem.cnf");
    return result.find("UNSATISFIABLE") == std::string::npos;
}

std::string MakePrintable(int num, int print_width) {
    std::string str_form = std::to_string(num);
    for(int i=0; i < print_width - Len(num); i++) {
        str_form += " ";
    }
    return str_form;
}

void PrintState(std::unordered_set<int> container,
int column, int row) {
    int print_width = Len(row*column);
    std::string dash = "_";
    for(int i=0; i<=print_width; i++) {
        dash += "_";
    }
    std::string space = " ";
    for(int i=0; i<=print_width; i++) {
        space += "_";
    }
    std::string ast = "*";
    for(int i=0; i < print_width - 1; i++) {
        ast += " ";
    }
    for (int i=0; i<row; i++) {
        for(int j=0; j<column; j++) {
            int no = i * column + j + 1;
            if(container.find(no) == container.end()) {
                std::cout << "| " + MakePrintable(no, print_width);
            } else {
                std::cout << "| " + ast;
            }
        }
        std::cout << "|\n";
        for(int i=0; i<column; i++) {
            std::cout << dash;
        }
        std::cout << std::endl;
    }
}

template <class T>
void printVector(std::vector<T> vec) {
  for(T v: vec) {
    std::cout << v << ",";
  }
  std::cout << std::endl;
}
