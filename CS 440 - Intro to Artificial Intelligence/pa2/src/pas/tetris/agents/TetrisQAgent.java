package src.pas.tetris.agents;


// SYSTEM IMPORTS
import java.util.Iterator;
import java.util.List;
import java.util.Random;


// JAVA PROJECT IMPORTS
import edu.bu.tetris.agents.QAgent;
import edu.bu.tetris.agents.TrainerAgent.GameCounter;
import edu.bu.tetris.game.Board;
import edu.bu.tetris.game.Game.GameView;
import edu.bu.tetris.game.minos.Mino;
import edu.bu.tetris.linalg.Matrix;
import edu.bu.tetris.nn.Model;
import edu.bu.tetris.nn.LossFunction;
import edu.bu.tetris.nn.Optimizer;
import edu.bu.tetris.nn.models.Sequential;
import edu.bu.tetris.nn.layers.Dense; // fully connected layer
import edu.bu.tetris.nn.layers.ReLU;  // some activations (below too)
import edu.bu.tetris.nn.layers.Tanh;
import edu.bu.tetris.training.data.Dataset;
import edu.bu.tetris.utils.Pair;


public class TetrisQAgent
    extends QAgent
{

    public static final double EXPLORATION_PROB = 0.1;

    private Random random;

    public TetrisQAgent(String name)
    {
        super(name);
        this.random = new Random(12345); // optional to have a seed
    }

    public Random getRandom() { return this.random; }

    @Override
    public Model initQFunction()
    {
        // build a single-hidden-layer feedforward network
        // this example will create a 3-layer neural network (1 hidden layer)
        // in this example, the input to the neural network is the
        // image of the board unrolled into a giant vector
        final int inputDim = Board.NUM_COLS + 10; // Dimension of the input layer based on board features
        final int hiddenDim1 = 64; // Number of neurons in the first hidden layer
        final int hiddenDim2 = 32; // Number of neurons in the second hidden layer
        final int outDim = 1; // Single output for Q-value

        Sequential qFunction = new Sequential();
        qFunction.add(new Dense(inputDim, hiddenDim1));
        qFunction.add(new Tanh());
        qFunction.add(new Dense(hiddenDim1, hiddenDim2));
        qFunction.add(new ReLU());
        qFunction.add(new Dense(hiddenDim2, outDim));

        return qFunction;
    }

    /**
        This function is for you to figure out what your features
        are. This should end up being a single row-vector, and the
        dimensions should be what your qfunction is expecting.
        One thing we can do is get the grayscale image
        where squares in the image are 0.0 if unoccupied, 0.5 if
        there is a "background" square (i.e. that square is occupied
        but it is not the current piece being placed), and 1.0 for
        any squares that the current piece is being considered for.
        
        We can then flatten this image to get a row-vector, but we
        can do more than this! Try to be creative: how can you measure the
        "state" of the game without relying on the pixels? If you were given
        a tetris game midway through play, what properties would you look for?
     */
    @Override
    public Matrix getQFunctionInput(final GameView game,
                                    final Mino potentialAction)
    {
        Matrix features = Matrix.zeros(1, Board.NUM_COLS + 10);
        try
        {
            Matrix board = game.getGrayscaleImage(potentialAction);

            int[] heights = new int[Board.NUM_COLS];
            int totalHoles = calculateHeightsAndHoles(board, heights);
            int completeLines = countCompletedLines(board);

            fillTypeFeatures(features, potentialAction);

            int bumpy = calculateBumpiness(heights);
            fillFeatureVector(features, heights, totalHoles, completeLines, bumpy);

        } catch(Exception e)
        {
            e.printStackTrace();
            System.exit(-1);
        }
        return features;
    }

    /**
     * Calculates the heights of each column on the board and the total number of holes.
     * @param board The current game board matrix.
     * @param heights Array to fill with each column's height.
     * @return Total number of holes on the board.
     */
    private int calculateHeightsAndHoles(Matrix board, int[] heights) {
        int totalHoles = 0;
        for (int col = 0; col < Board.NUM_COLS; col++) {
            int colHeight = 0;
            boolean holeStarted = false;
            boolean checkedHeight = false;
            int colHoles = 0;
            for (int row = 0; row < Board.NUM_ROWS; row++) {
                if (board.get(row, col) == 0.5 || board.get(row, col) == 1.0) {
                    if (!checkedHeight) {
                        colHeight = Board.NUM_ROWS - row;
                        checkedHeight = true;
                    }
                    holeStarted = true;
                } else if (holeStarted) {
                    colHoles++;
                }
            }
            heights[col] = colHeight;
            totalHoles += colHoles;
        }
        return totalHoles;
    }

    /**
     * Calculates the bumpiness of the board, defined as the sum of absolute differences between heights of adjacent columns.
     * @param heights Array of heights for each column.
     * @return The bumpiness value.
     */
    private int calculateBumpiness(int[] heights) {
        int bumpy = 0;
        for (int i = 1; i < heights.length; i++) {
            int diff = Math.abs(heights[i] - heights[i-1]);
            bumpy += diff;
        }
        return bumpy;
    }

    /**
     * Fills in the type-specific features into the feature matrix based on the Mino type.
     * @param features The feature matrix to be filled.
     * @param potentialAction The Mino being considered for placement.
     */
    private void fillTypeFeatures(Matrix features, Mino potentialAction) {
        Matrix typeVector = fillType(potentialAction);
        for (int i = 0; i < 7; i++) {
            int index = Board.NUM_COLS + 2 + i;
            features.set(0, index, typeVector.get(0, i));
        }
    }

    /**
     * Populates the feature vector with calculated game state values.
     * @param features The feature matrix to be filled.
     * @param heights Array of column heights.
     * @param totalHoles Total number of holes on the board.
     * @param completeLines Number of complete lines on the board.
     * @param bumpy Bumpiness of the board.
     */
    private void fillFeatureVector(Matrix features, int[] heights, int totalHoles, int completeLines, int bumpy) {
        for (int i = 0; i < Board.NUM_COLS; i++) {
            features.set(0, i, heights[i]);
        }
        features.set(0, Board.NUM_COLS, totalHoles);
        features.set(0, Board.NUM_COLS + 1, completeLines);
        features.set(0, Board.NUM_COLS + 9, bumpy);
    }

    /**
     * Generates a matrix representing the type of Mino.
     * @param mino The Mino whose type is to be encoded.
     * @return A matrix where each type of Mino is represented as a different unit vector.
     */
    private Matrix fillType(Mino mino) {
        Mino.MinoType type = mino.getType();
        Matrix typesMatrix = Matrix.zeros(1, 7);
        switch (type) {
            case I: typesMatrix.set(0, 0, 1); break;
            case J: typesMatrix.set(0, 1, 1); break;
            case L: typesMatrix.set(0, 2, 1); break;
            case O: typesMatrix.set(0, 3, 1); break;
            case S: typesMatrix.set(0, 4, 1); break;
            case T: typesMatrix.set(0, 5, 1); break;
            case Z: typesMatrix.set(0, 6, 1); break;
        }
        return typesMatrix;
    }
    
    /**
     * Counts the number of complete lines on the board.
     * @param m The board matrix.
     * @return The number of fully occupied rows in the matrix.
     */
    private int countCompletedLines(Matrix m) {
        int count = 0;
        for (int row = 0 ; row < m.getShape().getNumRows() ; row++) {
            boolean fullyOccupied = true;
            for (int col = 0 ; col < m.getShape().getNumCols() ; col++) {
                if (m.get(row, col) == 0.0) {
                    fullyOccupied = false;
                }
            }
            if (fullyOccupied) { count++; }
        }
        return count;
    }

    /**
     * This method is used to decide if we should follow our current policy
     * (i.e. our q-function), or if we should ignore it and take a random action
     * (i.e. explore).
     *
     * Remember, as the q-function learns, it will start to predict the same "good" actions
     * over and over again. This can prevent us from discovering new, potentially even
     * better states, which we want to do! So, sometimes we should ignore our policy
     * and explore to gain novel experiences.
     *
     * The current implementation chooses to ignore the current policy around 5% of the time.
     * While this strategy is easy to implement, it often doesn't perform well and is
     * really sensitive to the EXPLORATION_PROB. I would recommend devising your own
     * strategy here.
     */
    @Override
    public boolean shouldExplore(final GameView game,
                                 final GameCounter gameCounter)
    {
        // Dynamically adjust exploration probability based on game phases
        return random.nextDouble() < Math.max(0.01, EXPLORATION_PROB - (gameCounter.getNumPhases() * 0.00001));
    }

    /**
     * This method is a counterpart to the "shouldExplore" method. Whenever we decide
     * that we should ignore our policy, we now have to actually choose an action.
     *
     * You should come up with a way of choosing an action so that the model gets
     * to experience something new. The current implemention just chooses a random
     * option, which in practice doesn't work as well as a more guided strategy.
     * I would recommend devising your own strategy here.
     */
    @Override
    public Mino getExplorationMove(final GameView game)
    {
        int randIdx = this.getRandom().nextInt(game.getFinalMinoPositions().size());
        List<Mino> possibleMoves = game.getFinalMinoPositions();
        int index = 0;
        int maxCount = 0;

        for (int i = 0 ; i < possibleMoves.size() ; i++) {
            try {
                Matrix grayScaleBoard = game.getGrayscaleImage(possibleMoves.get(i));
                int count = countCompletedLines(grayScaleBoard);
                if (count > maxCount) {
                    index = i;
                    maxCount = count;
                }
            } catch (Exception e) { e.printStackTrace(); }
        }

        if (maxCount == 0) {index = randIdx;}

        return possibleMoves.get(index);
    }

    /**
     * This method is called by the TrainerAgent after we have played enough training games.
     * In between the training section and the evaluation section of a phase, we need to use
     * the exprience we've collected (from the training games) to improve the q-function.
     *
     * You don't really need to change this method unless you want to. All that happens
     * is that we will use the experiences currently stored in the replay buffer to update
     * our model. Updates (i.e. gradient descent updates) will be applied per minibatch
     * (i.e. a subset of the entire dataset) rather than in a vanilla gradient descent manner
     * (i.e. all at once)...this often works better and is an active area of research.
     *
     * Each pass through the data is called an epoch, and we will perform "numUpdates" amount
     * of epochs in between the training and eval sections of each phase.
     */
    @Override
    public void trainQFunction(Dataset dataset,
                               LossFunction lossFunction,
                               Optimizer optimizer,
                               long numUpdates)
    {
        for(int i = 0; i < numUpdates; ++i)
        {
            dataset.shuffle();
            Iterator<Pair<Matrix, Matrix> > batchIterator = dataset.iterator();

            while(batchIterator.hasNext())
            {
                Pair<Matrix, Matrix> batch = batchIterator.next();

                try
                {
                    Matrix YHat = this.getQFunction().forward(batch.getFirst());

                    optimizer.reset();
                    this.getQFunction().backwards(batch.getFirst(),
                                                  lossFunction.backwards(YHat, batch.getSecond()));
                    optimizer.step();
                } catch(Exception e)
                {
                    e.printStackTrace();
                    System.exit(-1);
                }
            }
        }
    }

    /**
     * This method is where you will devise your own reward signal. Remember, the larger
     * the number, the more "pleasurable" it is to the model, and the smaller the number,
     * the more "painful" to the model.
     *
     * This is where you get to tell the model how "good" or "bad" the game is.
     * Since you earn points in this game, the reward should probably be influenced by the
     * points, however this is not all. In fact, just using the points earned this turn
     * is a **terrible** reward function, because earning points is hard!!
     *
     * I would recommend you to consider other ways of measuring "good"ness and "bad"ness
     * of the game. For instance, the higher the stack of minos gets....generally the worse
     * (unless you have a long hole waiting for an I-block). When you design a reward
     * signal that is less sparse, you should see your model optimize this reward over time.
     */
    @Override
    public double getReward(final GameView game)
    {
        Board board = game.getBoard();
        int completedLines = calculateCompletedLines(board);
        double heightPenalty = calculateHeightPenalty(board);
        int totalHoles = calculateTotalHoles(board);

        double scoreComponent = game.getScoreThisTurn() * 1000;
        double linesComponent = completedLines * 50;
        double holesPenalty = totalHoles;
        double heightPenaltyComponent = heightPenalty * 0.25;

        return scoreComponent + linesComponent - holesPenalty - heightPenaltyComponent;
    }

    /**
     * Calculates the number of completely filled rows in the board, known as completed lines in Tetris.
     * A line is considered complete if every column in that row is occupied.
     * @param board The current state of the game board.
     * @return The number of fully completed lines on the board.
     */
    private int calculateCompletedLines(Board board) {
        int completedLines = 0;
        for (int row = 0; row < board.NUM_ROWS; row++) {
            boolean isComplete = true;
            for (int col = 0; col < board.NUM_COLS; col++) {
                if (!board.isCoordinateOccupied(col, row)) {
                    isComplete = false;
                    break;
                }
            }
            if (isComplete) {
                completedLines++;
            }
        }
        return completedLines;
    }

    /**
     * Calculates the height penalty for the Tetris board. The penalty is defined as the sum of 
     * the heights of the columns. Height for each column is considered as the number of filled cells 
     * from the top of the board to the first occupied cell encountered when moving downwards.
     * This encourages the agent to keep the pile low.
     * @param board The current state of the game board.
     * @return The cumulative penalty based on the heights of all columns.
     */
    private double calculateHeightPenalty(Board board) {
        double penalty = 0;
        for (int col = 0; col < board.NUM_COLS; col++) {
            for (int row = 0; row < board.NUM_ROWS; row++) {
                if (board.isCoordinateOccupied(col, row)) {
                    penalty += (board.NUM_ROWS - row);
                    break;
                }
            }
        }
        return penalty;
    }
    
    /**
     * Calculates the total number of holes in the Tetris board. A hole is defined as an empty space 
     * where there is at least one block above it in the same column. This metric is critical as holes 
     * can complicate future placements of Tetris pieces.
     * @param board The current state of the game board.
     * @return The total number of holes on the board.
     */
    private int calculateTotalHoles(Board board) {
        int totalHoles = 0;
        for (int col = 0; col < board.NUM_COLS; col++) {
            boolean lineExists = false;
            boolean holeFound = false;
            for (int row = 0; row < board.NUM_ROWS; row++) {
                if (board.isCoordinateOccupied(col, row)) {
                    lineExists = true;
                }
            }

            for (int rowScan = 0; rowScan < board.NUM_ROWS; rowScan++) {
                if (lineExists && !board.isCoordinateOccupied(col, rowScan)) {
                    holeFound = true;
                }
            }
            if (holeFound) {totalHoles++;}
        }
        return totalHoles;
    }

}
