
/*
  Representation of a grid.
*/
class Grid {
public:
  Grid(Dimension d, std::vector<int>* avoided = nullptr)
  : dimension(d), index(1) {
    if(avoided != nullptr) {
      for(int cell: *avoided) { avoidedCells.push_back(cell); }
    }
}

Grid(int w, int h, std::vector<int>* avoided = nullptr)
: dimension(Dimension(w,h)), index(1) {
  if (avoided != nullptr) { 
    for(int cell: *avoided) { avoidedCells.push_back(cell); }
  }
}

  // Map each valid cell to all the tetrominos that cover it.
void ConstructMap() {
  for (auto t: tetrominos) {
    for (int cell: t.coveredCells) {
      if (cell < 1 || cell > dimension.Size()) {
        continue;
      }
      if(cellMap.find(cell) == cellMap.end()) {
        cellMap.insert(std::pair<int, std::vector<Tetromino> >(cell, std::vector<Tetromino>()));
      }
      cellMap[cell].push_back(t);
    }
  }
}

  // Populate list of all possibly used tetrominos.
void FillTetrominosList() {
  for (int i = 1; i <= dimension.Size(); i++) {
    if (std::find(avoidedCells.begin(), avoidedCells.end(), i) != avoidedCells.end()) {
      continue;
    }
    Tetromino t[] = {Tetromino(UP, i, dimension),
     Tetromino(DOWN, i, dimension),
     Tetromino(LEFT, i, dimension),
     Tetromino(RIGHT, i, dimension)};
     for(Tetromino tetr: t) {
       if (tetr.IsValid(&avoidedCells)) {
         tetr.SetIndex(index++);
         tetrominos.push_back(tetr);
       }
     }
   }
 }

 void AddAvoidedCell(int ind) { avoidedCells.push_back(ind); }

 void GenerateCoveredClauses(int* size) {
  for (auto it=cellMap.begin(); it!=cellMap.end(); ++it) {
    coveredClauses.push_back("c 'COVERED'" + std::to_string(it->first));
    auto tetrs = it->second;
    std::string out;
    for (auto t: tetrs) {
      out = out + std::to_string(t.CNFIndex()) + " ";
    }
    out = out + " 0";
    coveredClauses.push_back(out);
    *size = *size + 1;
  }
}

void GenerateOnceClauses(int* size) {
  int count = 0;
  for (auto it=cellMap.begin(); it!=cellMap.end(); ++it) {
    auto tetrs = it->second;
    for(int i = 0; i < tetrs.size() -1; i++) {
      for(int j = i + 1; j < tetrs.size(); j++) {
        auto t1 = tetrs[i];
        auto t2 = tetrs[j];
        onceClauses.push_back(std::to_string(-1 * t1.CNFIndex()) + " "
          + std::to_string(-1 * t2.CNFIndex()) + " 0");
        *size = *size + 1;
      }
    }
  }
}

void GenereateCNFFile(int size, const std::string& fileName) {
  std::ofstream cnf_file;
  cnf_file.open(fileName);

  std::string first_line = "p cnf " + std::to_string(tetrominos.size())
  + " " + std::to_string(size);

  cnf_file << first_line << std::endl;
  for (const std::string& clause: coveredClauses) {
    cnf_file << clause << std::endl;
  }
  for (const std::string& clause: onceClauses) {
    cnf_file << clause << std::endl;
  }
  cnf_file.close();
}

void Run(const std::string& file_name = "problem.cnf") {
  FillTetrominosList();
  ConstructMap();
  int size = 0;
  GenerateCoveredClauses(&size);
  GenerateOnceClauses(&size);
  GenereateCNFFile(size, file_name);
}

void Print() {
  for(auto i = cellMap.begin(); i != cellMap.end(); i++) {
    std::cout << i->first << "-->\n";
    auto list = cellMap[i->first];
    for (Tetromino t: list) {
     t.Print();
   }
   std::cout << "\n**************\n";
 }
}

private:
  // Map of each cell to all tetrominos covering it.
  std::map<int, std::vector<Tetromino> > cellMap;
  // Dimension of the grid.
  Dimension dimension;
  // List of all possible tetrominos for the grid.
  std::vector<Tetromino> tetrominos;
  // Covered Clauses.
  std::vector<std::string> coveredClauses;
  // Once Clauses.
  std::vector<std::string> onceClauses;
  // Index (cnf index) of a tetromino to be added.
  // Constantly updated as new tetrominos are added
  // since the cnf index of a tetromino is unique.
  int index;
  // List of all avoided cells in map construction.
  // Allows usage of different shapes besides rectangles.
  std::vector<int> avoidedCells;
};