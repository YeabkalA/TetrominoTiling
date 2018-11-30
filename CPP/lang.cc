#include <iostream>
#include <vector>
#include <unordered_set>

class Query {
    public:
    Query(int r, int c) : row(r), column(c), max_no(r*c)
    , print_width(Len(r*c)), space(" ") {}

    void Execute(std::string order) {
        if(order == "Print") {
            PrintState(container, column, row);
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
                RemoveFromContainer(val);
            } else {
                std::cout << "Invalid query..." << std::endl;
            }
        }
    }
    void AddToContainer(int num) {
        container.insert(num);
    }

    void RemoveFromContainer(int num) {
        container.erase(num);
    }

    void Print() {
        for(auto n: container) {
            std::cout << n << std::endl;
        }
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
