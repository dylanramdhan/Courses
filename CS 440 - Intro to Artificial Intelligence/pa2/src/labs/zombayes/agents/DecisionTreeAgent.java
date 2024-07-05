package src.labs.zombayes.agents;


// SYSTEM IMPORTS
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;


// JAVA PROJECT IMPORTS
import edu.bu.labs.zombayes.agents.SurvivalAgent;
import edu.bu.labs.zombayes.features.quality.Entropy;
import edu.bu.labs.zombayes.features.Features.FeatureType;
import edu.bu.labs.zombayes.linalg.Matrix;
import edu.bu.labs.zombayes.utils.Pair;



public class DecisionTreeAgent
    extends SurvivalAgent
{

    public static class DecisionTree
        extends Object
    {

        public static abstract class Node
            extends Object
        {

            private Matrix X;
            private Matrix y_gt;

            public Node(Matrix X, Matrix y_gt)
            {
                this.X = X;
                this.y_gt = y_gt;
            }

            public final Matrix getX() { return this.X; }
            public final Matrix getY() { return this.y_gt; }

            public int getMajorityClass(Matrix X, Matrix y_gt)
            {
                Pair<Matrix, Matrix> uniqueYGtAndCounts = y_gt.unique();
                Matrix uniqueYGtVals = uniqueYGtAndCounts.getFirst();
                Matrix counts = uniqueYGtAndCounts.getSecond();

                // find the argmax of the counts
                int rowIdxOfMaxCount = -1;
                double maxCount = Double.NEGATIVE_INFINITY;

                for(int rowIdx = 0; rowIdx < counts.getShape().getNumRows(); ++rowIdx)
                {
                    if(counts.get(rowIdx, 0) > maxCount)
                    {
                        rowIdxOfMaxCount = rowIdx;
                        maxCount = counts.get(rowIdx, 0);
                    }
                }

                return (int)uniqueYGtVals.get(rowIdxOfMaxCount, 0);
            }

            public abstract int predict(Matrix x);
            public abstract List<Pair<Matrix, Matrix> > getChildData() throws Exception;

        }

        public static class LeafNode
            extends Node
        {

            private int predictedClass;

            public LeafNode(Matrix X, Matrix y_gt)
            {
                super(X, y_gt);
                this.predictedClass = this.getMajorityClass(X, y_gt);
            }

            @Override
            public int predict(Matrix x)
            {
                // predict the class (an integer)
                return this.predictedClass;
            }

            @Override
            public List<Pair<Matrix, Matrix> > getChildData() throws Exception { return null; }

        }

        public static class InteriorNode
            extends Node
        {

            private int             featureIdx;
            private FeatureType     featureType;

            // when we're processing a discrete feature, it is possible that even though that discrete feature
            // can take on any value in its domain (for example, like 5 values), the data we have may not contain
            // all of those values in it. Therefore, whenever we want to predict a test point, it is possible
            // that the test point has a discrete value that we haven't seen before. When we encounter such scenarios
            // we should predict the majority class (aka assign an "out-of-bounds" leaf node)
            private int             majorityClass;

            private List<Double>    splitValues; 
            private List<Node>      children;
            private Set<Integer>    childColIdxs;
            // if this feature is discrete, then |splitValues| = |children|
            // if this feature is continuous, then p = 1

            public InteriorNode(Matrix X, Matrix y_gt, Set<Integer> availableColIdxs)
            {
                super(X, y_gt);
                this.splitValues = new ArrayList<Double>();
                this.children = new ArrayList<Node>();
                this.majorityClass = this.getMajorityClass(X, y_gt);

                // make a deepcopy of the set that is given to us....we need to potentially remove stuff from this
                // so don't use a shallow copy and risk messing up parent nodes (with a shared shallow copy)!
                this.childColIdxs = new HashSet<Integer>(availableColIdxs);

                // quite a lot happens in this method.
                // this method will figure out which feature (amongst all the ones that we are allowed to see)
                // has the "best" quality (as measured by info gain). It will also populate the field 'this.splitValues'
                // with the correct values for that feature.
                // (side note: this is why this method is being called *after* this.splitValues is initialized)
                this.featureIdx = this.pickBestFeature(X, y_gt, availableColIdxs);
                this.featureType = DecisionTree.FEATURE_HEADER[this.getFeatureIdx()];

                // once we know what feature this node has, we need to remove that feature from our children
                // if that feature is discrete.
                // we made a deepcopy of the set so we're all good to in-place remove here.
                if(this.getFeatureType().equals(FeatureType.DISCRETE))
                {
                    this.getChildColIdxs().remove(this.getFeatureIdx());
                }
            }

            //------------------------ some getters and setters (cause this is java) ------------------------
            public int getFeatureIdx() { return this.featureIdx; }
            public final FeatureType getFeatureType() { return this.featureType; }

            private List<Double> getSplitValues() { return this.splitValues; }
            private List<Node> getChildren() { return this.children; }

            public Set<Integer> getChildColIdxs() { return this.childColIdxs; }
            public int getMajorityClass() { return this.majorityClass; }
            //-----------------------------------------------------------------------------------------------

            // make sure we add children in the correct order when we use this!
            public void addChild(Node n) { this.getChildren().add(n); }


            // TODO: complete me!
            private int pickBestFeature(Matrix X, Matrix y_gt, Set<Integer> availableColIdxs)
            {
                return -1;
            }

            // TODO: complete me!
            private Pair<Double, Matrix> getConditionalEntropy(Matrix X, Matrix y_gt, int colIdx) throws Exception
            {
                return null;
            }

            // TODO: complete me!
            @Override
            public int predict(Matrix x)
            {
                return -1;
            }

            // TODO: complete me!
            @Override
            public List<Pair<Matrix, Matrix> > getChildData() throws Exception
            {
                return null;
            }

        }

        public Node root;
        public static final FeatureType[] FEATURE_HEADER = {FeatureType.CONTINUOUS,
                                                            FeatureType.CONTINUOUS,
                                                            FeatureType.DISCRETE,
                                                            FeatureType.DISCRETE};

        public DecisionTree()
        {
            this.root = null;
        }

        public Node getRoot() { return this.root; }
        private void setRoot(Node n) { this.root = n; }

        // TODO: complete me!
        private Node dfsBuild(Matrix X, Matrix y_gt, Set<Integer> availableColIdxs) throws Exception
        {
            return null;
        }

        public void fit(Matrix X, Matrix y_gt)
        {
            System.out.println("DecisionTree.fit: X.shape=" + X.getShape() + " y_gt.shape=" + y_gt.getShape());
            try
            {
                Set<Integer> allColIdxs = new HashSet<Integer>();
                for(int colIdx = 0; colIdx < X.getShape().getNumCols(); ++colIdx)
                {
                    allColIdxs.add(colIdx);
                }
                this.setRoot(this.dfsBuild(X, y_gt, allColIdxs));
            } catch(Exception e)
            {
                e.printStackTrace();
                System.exit(-1);
            }
        }

        public int predict(Matrix x)
        {
            // class 0 means Human (i.e. not a zombie), class 1 means zombie
            System.out.println("DecisionTree.predict: x=" + x);
            return this.getRoot().predict(x);
        }

    }

    private DecisionTree tree;

    public DecisionTreeAgent(int playerNum, String[] args)
    {
        super(playerNum, args);
        this.tree = new DecisionTree();
    }

    public DecisionTree getTree() { return this.tree; }

    @Override
    public void train(Matrix X, Matrix y_gt)
    {
        System.out.println(X.getShape() + " " + y_gt.getShape());
        this.getTree().fit(X, y_gt);
    }

    @Override
    public int predict(Matrix featureRowVector)
    {
        return this.getTree().predict(featureRowVector);
    }

}
