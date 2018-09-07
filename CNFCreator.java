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

public class CNFCreator {
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
	public int getIndex() { return cnfInex; }
	public void setIndex(int ind) { cnfInex = ind; }


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
	private int cnfInex;
    }

    public static class Grid {
	public Grid(Dimension d, HashSet<Integer> avoided) {
	    this.dimension = d;
	    this.avoidedCells = new HashSet<>(avoided);
	    this.tetrominoIndexMap = new HashMap<>();
	    this.tetrominos = new ArrayList<>();
	    this.coveredClauses = new ArrayList<>();
	    this.onceClauses = new ArrayList<>();
	    this.cellMap = new HashMap<>();
	    index = 1;
	}
	public Grid(int w, int h, HashSet<Integer> avoided) {
	    this.dimension = new Dimension(w,h);
	    this.avoidedCells = new HashSet<>(avoided);
	    this.tetrominoIndexMap = new HashMap<>();
	    this.tetrominos = new ArrayList<>();
	    this.coveredClauses = new ArrayList<>();
	    this.onceClauses = new ArrayList<>();
	    this.cellMap = new HashMap<>();
	    index = 1;
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

	public void generateCNFFile(int cnfSize, String fileName) throws Exception {
	    File file = new File(fileName);
	    PrintWriter writer = new PrintWriter(file);
	    writer.println("p cnf " + Integer.toString(this.tetrominos.size())
			   + " " + Integer.toString(cnfSize));
	    for(String clause: coveredClauses) { writer.println(clause); }
	    for(String clause: onceClauses) { writer.println(clause); }

	    writer.close();
	}

	public void run(String fileName) throws Exception {
	    this.fillTetrominosList();
	    this.constructMap();
	    int cnfSize = generateCoveredClauses() + generateOnceClauses();
	    generateCNFFile(cnfSize, fileName);
	}

	private HashMap<Integer, ArrayList<Tetromino>> cellMap;
	private Dimension dimension;
	private ArrayList<Tetromino> tetrominos;
	private HashMap<Integer, Tetromino> tetrominoIndexMap;
	private ArrayList<String> coveredClauses;
	private ArrayList<String> onceClauses;
	private HashSet<Integer> avoidedCells;
	private int index;
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

			// Dark Rectangle
			for(int cell: selectedCells) {
			    int[] coordinate = getCoordinateFromIndex(cell);
			    int x = coordinate[0], y = coordinate[1];
			    g.fillRect(widthConst * (x - 1), heightConst * (y - 1), widthConst, heightConst);
			}
			// Draw horizontal lines;

			current = heightConst;
			for(int i=0; i<h; i++) {
			    g.drawLine(0, current, 1000, current);
			    current += heightConst;
			}

			mainPanel.addMouseListener(new MouseAdapter() {
				public void mouseReleased(MouseEvent e) {
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
					a.setColor(new Color(100, 100, 100));
					System.out.println(selectedCells);
				    }
				    a.fillRect(widthConst * (x - 1) + 2, heightConst * (y - 1) + 2,
					       widthConst - 5, heightConst - 5);
				}
			    });
		    }
		};

	    mainPanel.setPreferredSize(new java.awt.Dimension(1000, 1000));
	    add(mainPanel, BorderLayout.CENTER);
	    this.pack();
	    this.setVisible(true);
	}

	public DisplayGrid(String fileName, int[] tetrominoIndices, int w, int h) {
	    Grid grid = new Grid(new Dimension(w,h), new HashSet<Integer>());
	    grid.fillTetrominosList();

	    int[] solution;
	    FileReader f = null;
	    BufferedReader b = null;
	    String line = "";
	    try {
		f = new FileReader(fileName);
		b = new BufferedReader(f);
		while((line = b.readLine()) != null) {
		    if(line.equals("SAT")) continue;
		    int[] tempSol = Arrays.stream(line.split(" ")).mapToInt(Integer::parseInt).toArray();
		    solution = new int[tempSol.length - 1];
		    for(int i = 0; i < solution.length; i++) { solution[i] = tempSol[i]; }
		}
	    } catch(IOException e) {}
	    ArrayList<Tetromino> drawnTetrominos = new ArrayList<>();
	    for(int i=0; i < solution.length; i++) {
		drawnTetrominos.add(grid.getTetrominoIndexMap().get(solution[i]);
	    }

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
	    return new int[] {ind % this.width, ind/width - 1};
	}
    }

    public static void main(String[] args) throws Exception {
	Scanner scan = new Scanner(System.in);
		
	System.out.println("TETROMIO SAT SOLVER TOOL");
	System.out.println("Enter 1 to generate cnf file, 2 to get the display of the solution (if any)");
	int choice = Integer.parseInt(scan.nextLine());

	if (choice == 1) {
	    System.out.println("Enter coordinate (w h) for grid.");
	    int[] dimensions = 
		Arrays.stream(scan.nextLine().split(" ")).mapToInt(Integer::parseInt).toArray();
	    int w = dimensions[0];
	    int h = dimensions[1];

	    DisplayGrid display = new DisplayGrid(w, h);

	    String command = scan.nextLine();
	    if (command.equals("gen")) {
		System.out.println("Enter file name");
		String fileName = scan.nextLine();
		Grid grid = new Grid(new Dimension(w, h),display.selectedCells);
		grid.run(fileName);
		return;
	    }
	}
    }
}
