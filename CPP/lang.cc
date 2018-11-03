#include <iostream>
#include <vector>
#include <unordered_set>

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
class Query {
    public:
    Query(int r, int c) : row(r), column(c), max_no(r*c)
    , print_width(Len(r*c)), space(" ") {}

    void Execute(std::string order) {
        if(order == "Print") {
            PrintState();
            return;
        }
        int val = -1;
        bool isNumber = NumericCheck(order, &val);
        if(isNumber) { // 'order' is Numeric.
            AddToContainer(val);
        } else {
            if(order.back() == 'R') {
                for(int i=row*(val-1) + 1; i<=column*val; i++) {
                    AddToContainer(i);
                }
            } else if(order.back() == 'C') {
                for(int i=val; i<=column * (row-1) + val; i+=column) {
                    AddToContainer(i);
                }
            } else if(order.back() == 'A') {
                RemoveToContainer(val);
            } else {
                std::cout << "Invalid query..." << std::endl;
            }
        }
    }
    void AddToContainer(int num) {
        container.insert(num);
    }

    void RemoveToContainer(int num) {
        container.erase(num);
    }

    void Print() {
        for(auto n: container) {
            std::cout << n << std::endl;
        }
    }

    std::string MakePrintable(int num) {
        std::string str_form = std::to_string(num);
        for(int i=0; i < print_width - Len(num); i++) {
            str_form += " ";
        }
        return str_form;
    }

    void PrintState() {
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
                    std::cout << "| " + MakePrintable(no);
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

    void PrintQueryState() {
        std::cout << "pw=" << print_width << std::endl;

    }

    void AcceptQueries() {
        while(true) {
            std::string order;
            std::cin >> order;
            if(order == "End") return;
            Execute(order);
        }
    }

    std::unordered_set<int> Container() { return container; }
    private:
    std::unordered_set<int> container;
    int row;
    int column;
    int max_no;
    int print_width;
    std::string space;
};