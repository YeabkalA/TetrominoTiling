import java.util.*;

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


	public boolean isValid(ArrayList<Integer> avoidedCells) {
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

    static class Grid {
	public Grid(Dimension d, ArrayList<Integer> avoided) {
	    this.dimension = d;
	    this.avoidedCells = new ArrayList(avoided);
	}
	public Grid(int w, int h, ArrayList<Integer> avoided) {
	    this.dimension = new Dimension(w,h);
	    this.avoidedCells = new ArrayList(avoided);
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
		for(Tetromino t1: this.cellMap.get(cell)) {
		    for(Tetromino t2: this.cellMap.get(cell)) {
			this.onceClauses.add(Integer.toString(-1 * t1.getIndex()) + 
					     " " + Integer.toString(-1 * t2.getIndex()));
			size++;
		    }
		}
	    }
	    return size;
	}

	private HashMap<Integer, ArrayList<Tetromino>> cellMap;
	private Dimension dimension;
	private ArrayList<Tetromino> tetrominos;
	private ArrayList<String> coveredClauses;
	private ArrayList<String> onceClauses;
	private ArrayList<Integer> avoidedCells;
	private int index;
    }

    public static void main(String[] args) {
	System.out.println("Working!");
    }
}
