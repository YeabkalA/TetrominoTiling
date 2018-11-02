import java.util.*;
import java.io.*;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.Color;

public class CNFCreatorInteractive {
	static class Dimension {
		public Dimension(int w, int h) {
			this.width = w;
			this.height = h;
		}
		public int getWidth() { return this.width; }
		public int getHeight() { return this.height; }
		public int getSize() { return this.width * this.height; }

		private int height;
		private int width;
	}

	public enum Orientation { UP, DOWN, LEFT, RIGHT }

	static class Tetromino {
		public Tetromino(Orientation orientation,
				int startingLoc,
				Dimension dimension) {
			this.start = startingLoc;
			this.orientation = orientation;
			this.gridDimension = dimension;
			this.coveredCells = new int[4];

			int width = dimension.getWidth();

			switch(orientation) {
			case UP:
				coveredCells[0] = startingLoc;
				coveredCells[1] = startingLoc + 1;
				coveredCells[2] = startingLoc + 2;
				coveredCells[3] = startingLoc - width + 1;
				break;
			case LEFT:
				coveredCells[0] = startingLoc;
				coveredCells[1] = startingLoc + width;
				coveredCells[2] = startingLoc + 2*width;
				coveredCells[3] = startingLoc + width - 1;
				break;
			case RIGHT:
				coveredCells[0] = startingLoc;
				coveredCells[1] = startingLoc + width;
				coveredCells[2] = startingLoc + 2*width;
				coveredCells[3] = startingLoc + width + 1;
				break;
			case DOWN:
				coveredCells[0] = startingLoc;
				coveredCells[1] = startingLoc + 1;
				coveredCells[2] = startingLoc + 2;
				coveredCells[3] = startingLoc + width + 1;
				break;
			}
		}

		public Orientation getOrientation() { return this.orientation; }
		public int getIndex() { return cnfIndex; }
		public int getRealIndex() { return realIndex; }
		public int[] getCoveredCells() { return this.coveredCells; }
		public void setIndex(int ind) { cnfIndex = ind; }
		public void setRealIndex(int ind) { realIndex = ind; }


		public boolean isValid(HashSet<Integer> avoidedCells) {
			for(int cell: coveredCells) {
				if(avoidedCells.contains(cell)) { return false; }
			}

			// Check that tetromino lies inside the grid.
			if(start < 1 || start > gridDimension.getSize()) { return false; }


			if (orientation == Orientation.RIGHT) {
				return start % gridDimension.getWidth() != 0
						&& gridDimension.getSize() - start >= 2*gridDimension.getWidth(); 
			} else if (orientation == Orientation.LEFT) {
				return (start - 1) % gridDimension.getWidth() !=0
						&& gridDimension.getSize() - start >= 2*gridDimension.getWidth();
			} else if (orientation == Orientation.UP) {
				return (start > gridDimension.getWidth())
						&& (((start + 1) % gridDimension.getWidth()) != 0)
						&& (start % gridDimension.getWidth() != 0);
			} else {
				return (gridDimension.getSize() - start > gridDimension.getWidth())
						&& (((start + 1) % gridDimension.getWidth()) != 0)
						&& start % gridDimension.getWidth() != 0;
			}
		}

		private int start;
		private Orientation orientation;
		private int[] coveredCells;
		private Dimension gridDimension;
		private int cnfIndex;
		private int realIndex;
	}

	public static class Grid {
		public Grid(Dimension d, HashSet<Integer> avoided) {
			this.dimension = d;
			this.avoidedCells = new HashSet<>(avoided);
			this.drawnTetrominos = new ArrayList<>();
			this.tetrominoIndexMap = new HashMap<>();
			this.tetrominos = new ArrayList<>();
			this.coveredClauses = new ArrayList<>();
			this.onceClauses = new ArrayList<>();
			this.cellMap = new HashMap<>();
			this.satisfiable = false;
			index = 1;
			realIndex = 1;
		}
		public Grid(int w, int h, HashSet<Integer> avoided) {
			this.dimension = new Dimension(w,h);
			this.avoidedCells = new HashSet<>(avoided);
			this.drawnTetrominos = new ArrayList<>();
			this.tetrominoIndexMap = new HashMap<>();
			this.tetrominos = new ArrayList<>();
			this.coveredClauses = new ArrayList<>();
			this.onceClauses = new ArrayList<>();
			this.cellMap = new HashMap<>();
			this.satisfiable = false;
			index = 1;
			realIndex = 1;
		}

		public int getWDim() { return this.dimension.getWidth(); }
		public int getHDim() { return this.dimension.getHeight(); }
		
		public HashSet<Integer> getCoveredCells() { return coveredCells; }

		public boolean isSatisfiable() { return this.satisfiable; }

		public void setFileNames(String prefix) {
			this.cnfFileName = prefix + ".cnf";
			this.outFileName = prefix + ".out";
		}

		public void constructMap() {
			for(Tetromino t: tetrominos) {
				for(int cell: t.coveredCells) {
					if(cell < 1 || cell > this.dimension.getSize()) { continue; }
					if(!cellMap.containsKey(cell)) {
						this.cellMap.put(cell, new ArrayList<Tetromino>());
					}
					this.cellMap.get(cell).add(t);
				}
			}
		}

		public void fillTetrominosList() {
			for(int i = 1; i <= this.dimension.getSize(); i++) {
				if(avoidedCells.contains(i)) { continue; }
				Tetromino t[] = {new Tetromino(Orientation.UP, i, this.dimension),
						new Tetromino(Orientation.DOWN, i, this.dimension),
						new Tetromino(Orientation.LEFT, i, this.dimension),
						new Tetromino(Orientation.RIGHT, i, this.dimension)};
				for (Tetromino tetr: t) {
					tetr.setRealIndex(this.realIndex);
					this.realIndex++;
					if(tetr.isValid(this.avoidedCells)) {
						tetr.setIndex(this.index);
						this.tetrominoIndexMap.put(index, tetr);
						this.index++;
						this.tetrominos.add(tetr);
					}
				}
			}
		}

		public void addAvoidedCell(int index) { this.avoidedCells.add(index); }

		public HashMap<Integer, Tetromino> getTetrominoIndexMap() { return this.tetrominoIndexMap; }

		public ArrayList<Tetromino> getDrawnTetrominos() { return this.drawnTetrominos; }

		public int generateCoveredClauses() {
			int size = 0;
			for(int cell: this.cellMap.keySet()) {
				this.coveredClauses.add("c COVERED" + Integer.toString(cell));
				StringBuilder out = new StringBuilder();
				for(Tetromino t: this.cellMap.get(cell)) {
					out.append(Integer.toString(t.getIndex()) + " ");
				}
				out.append("0");
				this.coveredClauses.add(out.toString());
				size++;
			}
			return size;
		}

		public int generateOnceClauses() {
			int size = 0;
			for(int cell: this.cellMap.keySet()) {
				int listSize = this.cellMap.get(cell).size();
				for(int i = 0; i < listSize - 1; i++) {
					for(int j = i + 1; j < listSize; j++) {
						Tetromino t1 = this.cellMap.get(cell).get(i);
						Tetromino t2 = this.cellMap.get(cell).get(j);
						this.onceClauses.add(Integer.toString(-1 * t1.getIndex()) + 
								" " + Integer.toString(-1 * t2.getIndex())
								+ " 0");
						size++;
					}
				}
			}
			return size;
		}

		public void generateCNFFile(int cnfSize) throws Exception {
			File file = new File(this.cnfFileName);
			PrintWriter writer = new PrintWriter(file);
			writer.println("p cnf " + Integer.toString(this.tetrominos.size())
			+ " " + Integer.toString(cnfSize));
			for(String clause: coveredClauses) { writer.println(clause); }
			for(String clause: onceClauses) { writer.println(clause); }

			writer.close();
		} 

		public void runCNSGeneration() throws Exception {
			this.fillTetrominosList();
			this.constructMap();
			int cnfSize = generateCoveredClauses() + generateOnceClauses();
			generateCNFFile(cnfSize);
		}

		public void checkIfSatisfiable() {
			String[] minisatResult = runMinisat(this.cnfFileName, this.outFileName);
			// Check if case is satisfied.
			if(minisatResult[0].endsWith("UNSATISFIABLE")) {
				System.out.println("UNSATISFIABLE");
			} else {
				this.satisfiable = true;
			}
		}

		public void fillDrawnTetrominos() {
			if(!this.satisfiable) {
				return;
			}
			ArrayList<Integer> solution = new ArrayList<>();
			FileReader f = null;
			BufferedReader b = null;
			String line = "";
			try {
				f = new FileReader(this.outFileName);
				b = new BufferedReader(f);
				while((line = b.readLine()) != null) {
					System.out.println("Read ... " + line);
					if(line.equals("SAT")) continue;
					int[] tempSol = Arrays.stream(line.split(" ")).
							mapToInt(Integer::parseInt).filter(cell -> cell > 0).toArray();
					for(int i = 0; i < tempSol.length; i++) { solution.add(tempSol[i]); }
				}
			} catch(IOException e) {}

			for(int i=0; i < solution.size(); i++) {
				this.drawnTetrominos.add(this.getTetrominoIndexMap().get(solution.get(i)));
			}
		}

		public void run() throws Exception {
			this.runCNSGeneration();
			this.checkIfSatisfiable();
			this.fillDrawnTetrominos();

			coveredCells = new HashSet<>();
			for(Tetromino t: drawnTetrominos) {
				for(int cell: t.getCoveredCells()) {
					coveredCells.add(cell);
				}
			}
			if(!(coveredCells.size() + avoidedCells.size() == dimension.height * dimension.width)) {
				satisfiable = false;
			}

		}

		private HashMap<Integer, ArrayList<Tetromino>> cellMap;
		private ArrayList<Tetromino> drawnTetrominos;
		private Dimension dimension;
		private ArrayList<Tetromino> tetrominos;
		private HashMap<Integer, Tetromino> tetrominoIndexMap;
		private ArrayList<String> coveredClauses;
		private ArrayList<String> onceClauses;
		private HashSet<Integer> avoidedCells;
		private int index;
		private int realIndex;
		private String cnfFileName = "a.cnf";
		private String outFileName = "a.out";
		private boolean satisfiable;
		// generated after a successful case for additional layer of check
		private HashSet<Integer> coveredCells;
	}

	public static class DisplayGrid extends JFrame {
		JPanel mainPanel;
		int width;
		int height;
		HashSet<Integer> selectedCells;
		HashSet<Integer> draggedIndices;

		public static int WIDTH, HEIGHT = 1000;

		public DisplayGrid(int w, int h) {
			width = w;
			height = h;
			selectedCells = new HashSet<>();
			draggedIndices = new HashSet<>();
			setLayout(new BorderLayout());

			mainPanel = new JPanel() {
				public void paintComponent(Graphics g) {
					super.paintComponent(g);

					// Draw vertical lines;
					int widthConst = 1000/w;
					int heightConst = 1000/h;

					System.out.println("Width Const = " + widthConst);
					int current = widthConst;
					for(int i=0; i<w; i++) {
						g.drawLine(current, 0, current, 1000);
						current += widthConst;
					}

					// Draw horizontal lines;

					current = heightConst;
					for(int i=0; i<h; i++) {
						g.drawLine(0, current, 1000, current);
						current += heightConst;
					}

					mainPanel.addMouseListener(new MouseAdapter() {
						public void mouseReleased(MouseEvent e) {
							try {
							Thread.sleep(50);
							} catch(Exception ex) {}
							int[] coordinate = getCoordinateFromClick(
									e.getX(), e.getY(), widthConst, heightConst);
							int x = coordinate[0], y = coordinate[1];
							int clickedIndex = getIndexFromCoordinate(coordinate);
							Graphics a = mainPanel.getGraphics().create();
							if (selectedCells.contains(clickedIndex)) {
								selectedCells.remove(clickedIndex);
								a.setColor(Color.WHITE);
								System.out.println(selectedCells);
							} else {
								selectedCells.add(clickedIndex);
								a.setColor(new Color(200, 100, 150));
								System.out.println(selectedCells);
							}
							a.fillRect(widthConst * (x - 1) + 2, heightConst * (y - 1) + 2,
									widthConst - 5, heightConst - 5);
							Grid grid = new Grid(w, h, selectedCells);
							grid.setFileNames("a");
							try {
								grid.run();
							} catch(Exception ex) {}
							if(grid.isSatisfiable()) {
								new DisplayGrid(grid);
							}
						}
					});
				}
			};

			mainPanel.setPreferredSize(new java.awt.Dimension(1000, 1000));
			add(mainPanel, BorderLayout.CENTER);
			this.pack();
			this.setVisible(true);
		}

		public DisplayGrid(Grid grid) {		
			width = grid.getWDim();
			height = grid.getHDim();
			setLayout(new BorderLayout());

			Color[] colors = new Color[4];
			colors[0] = new Color(202, 222, 0);
			colors[1] = new Color(72, 222, 0);
			colors[2] = new Color(72, 222, 243);
			colors[3] = new Color(250, 0, 50);


			mainPanel = new JPanel() {
				public void paintComponent(Graphics g) {
					super.paintComponent(g);

					// Draw vertical lines;
					int widthConst = 500/width;
					int heightConst = 500/height;

					for(Tetromino t: grid.getDrawnTetrominos()) {
						int[] covered = t.getCoveredCells();
						System.out.println("For " + t.getIndex() + Arrays.toString(covered));
						if(t.getOrientation() == Orientation.LEFT) {
							g.setColor(colors[0]);
						} else if(t.getOrientation() == Orientation.RIGHT) {
							g.setColor(colors[1]);
						} else if(t.getOrientation() == Orientation.UP) {
							g.setColor(colors[2]);
						} else {
							g.setColor(colors[3]);
						}
						for(int cell: covered) {
							int[] coordinate = getCoordinateFromIndex(cell);
							int x = coordinate[0], y = coordinate[1];
							g.fillRect(widthConst * (x - 1) + 2, heightConst * (y - 1) + 2,
									widthConst, heightConst);
						}
					}
				}
			};

			mainPanel.setPreferredSize(new java.awt.Dimension(500, 500));
			add(mainPanel, BorderLayout.CENTER);
			this.pack();
			this.setVisible(true);
		}



		public int[] getCoordinateFromClick(int x, int y, int xConst, int yConst) {
			return new int[]{x/xConst + 1, y/yConst + 1};
		}

		public int getIndexFromCoordinate(int[] coordinate) {
			int x = coordinate[0];
			int y = coordinate[1];

			return (y-1) * this.width + x;
		}

		public int[] getCoordinateFromIndex(int ind) {
			int modWithW = ind % this.width;
			int x = 0, y = 0;
			if(modWithW != 0) {
				x = modWithW;
				y = ind/this.width + 1;
			} else {
				x = this.width;
				y = ind/this.width;
			}
			return new int[] {x, y};
		}
	}

	public static String[] runMinisat(String inFileName, String outFileName) {
		String s = null;
		String command = "./minisat " + inFileName + " " + outFileName;
		StringBuilder output = new StringBuilder();
		StringBuilder error = new StringBuilder();
		try {
			Process p = Runtime.getRuntime().exec(command);
			BufferedReader stdInput = new BufferedReader(
					new InputStreamReader(p.getInputStream()));
			BufferedReader stdError = new BufferedReader(
					new InputStreamReader(p.getErrorStream()));

			while((s = stdInput.readLine()) != null) {
				output.append(s);
			}
			while((s = stdError.readLine()) != null) {
				error.append(s);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		return new String[] {output.toString(), error.toString()};
	}

	// from: https://stackoverflow.com/questions/12548312/find-all-subsets-of-length-k-in-an-array
	public static void getSubsets(ArrayList<Integer> superSet, int k, int idx,
			Set<Integer> current,List<HashSet<Integer>> solution) {
		if (current.size() == k) {
			solution.add(new HashSet<>(current));
			return;
		}
		if (idx == superSet.size()) return;
		Integer x = superSet.get(idx);
		current.add(x);
		getSubsets(superSet, k, idx+1, current, solution);
		current.remove(x);
		getSubsets(superSet, k, idx+1, current, solution);
	}

	// from: https://stackoverflow.com/questions/12548312/find-all-subsets-of-length-k-in-an-array
	public static ArrayList<HashSet<Integer>> getSubsets(ArrayList<Integer> superSet, int k) {
		ArrayList<HashSet<Integer>> res = new ArrayList<>();
		getSubsets(superSet, k, 0, new HashSet<Integer>(), res);
		return res;
	}

	public static void main(String[] args) throws Exception {
		Scanner scan = new Scanner(System.in);

		System.out.println("TETROMIO SAT SOLVER TOOL");

		System.out.println("Enter coordinate (w h) for grid.");
		int[] dimensions = 
				Arrays.stream(scan.nextLine().split(" ")).mapToInt(Integer::parseInt).toArray();
		int w = dimensions[0];
		int h = dimensions[1];

		System.out.println("You can either use a GUI or specify # of monominos allowed...");
		System.out.println("Enter 1 for GUI or 2 otherwise");

		String cmd = scan.nextLine();

		if(cmd.equals("1")) {
			DisplayGrid display = new DisplayGrid(w, h);

			String command = scan.nextLine();
			if (command.equals("gen")) {
				System.out.println("Enter file prefix");
				String filePrefix = scan.nextLine();
				Grid grid = new Grid(new Dimension(w, h),display.selectedCells);
				grid.setFileNames(filePrefix);
				grid.run();
				new DisplayGrid(grid);
			}
		} else {
			System.out.println("Enter the number of monominos you want to allow");
			int m = Integer.parseInt(scan.nextLine());
			int gridSize = w * h;
			ArrayList<Integer> cells = new ArrayList<>();
			for(int i = 1; i <= gridSize; i++) {
				cells.add(i);
			}
			ArrayList<HashSet<Integer>> combos = getSubsets(cells, m);
			ArrayList<Grid> okGrids = new ArrayList<>();
			for(HashSet<Integer> avoided: combos) {
				Grid grid = new Grid(new Dimension(w, h), avoided);
				grid.run();
				if(grid.isSatisfiable()) {
					okGrids.add(grid);
				}
			}
			System.out.println("Finished grid addition");
			for(Grid g: okGrids) {
				new DisplayGrid(g);
			}
		}

	}
}
