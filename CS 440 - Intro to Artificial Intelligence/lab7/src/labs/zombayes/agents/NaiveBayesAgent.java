package src.labs.zombayes.agents;


// SYSTEM IMPORTS
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;


// JAVA PROJECT IMPORTS
import edu.bu.labs.zombayes.agents.SurvivalAgent;
import edu.bu.labs.zombayes.features.Features.FeatureType;
import edu.bu.labs.zombayes.linalg.Matrix;
import edu.bu.labs.zombayes.utils.Pair;



public class NaiveBayesAgent
    extends SurvivalAgent
{

    public static class NaiveBayes
        extends Object
    {

        public static final FeatureType[] FEATURE_HEADER = {FeatureType.CONTINUOUS,
                                                            FeatureType.CONTINUOUS,
                                                            FeatureType.DISCRETE,
                                                            FeatureType.DISCRETE};

        private Map<Integer, Map<Integer, Double>> featureProbabilities;
        private Map<Integer, Double> classProbabilities;

        // TODO: complete me!
        public NaiveBayes()
        {
            featureProbabilities = new HashMap<>();
            classProbabilities = new HashMap<>();
        }

        // TODO: complete me!
        public void fit(Matrix X, Matrix y_gt)
        {
            // Calculate class probabilities
            calculateClassProbabilities(y_gt);

            // Calculate feature probabilities
            calculateFeatureProbabilities(X, y_gt);
        }

        private void calculateClassProbabilities(Matrix y_gt) {
            // Count the occurrences of each class
            Map<Integer, Integer> classCounts = new HashMap<>();
            for (int i = 0; i < y_gt.getShape().getNumRows(); i++) {
                int classLabel = (int) y_gt.get(i, 0);
                classCounts.put(classLabel, classCounts.getOrDefault(classLabel, 0) + 1);
            }
    
            // Calculate probabilities for each class
            int totalSamples = y_gt.getShape().getNumRows();
            for (Map.Entry<Integer, Integer> entry : classCounts.entrySet()) {
                int classLabel = entry.getKey();
                int count = entry.getValue();
                double probability = (double) count / totalSamples;
                classProbabilities.put(classLabel, probability);
            }
        }
    
        private void calculateFeatureProbabilities(Matrix X, Matrix y_gt) {
            // For each class label
            for (int classLabel : classProbabilities.keySet()) {
                // Filter data for current class label
                List<Matrix> classSamples = new ArrayList<>();
                for (int i = 0; i < y_gt.getShape().getNumRows(); i++) {
                    if ((int) y_gt.get(i, 0) == classLabel) {
                        classSamples.add(X.getRow(i));
                    }
                }
    
                // For each feature
                Map<Integer, Double> featureProbs = new HashMap<>();
                for (int featureIndex = 0; featureIndex < X.getShape().getNumCols(); featureIndex++) {
                    // Calculate probability distribution for each feature
                    if (FEATURE_HEADER[featureIndex] == FeatureType.CONTINUOUS) {
                        // For continuous features (using Gaussian distribution)
                        // Calculate mean and variance
                        double mean = calculateMean(classSamples, featureIndex);
                        double variance = calculateVariance(classSamples, featureIndex);
                        // Calculate Gaussian probability density function for each value
                        double featureProb = calculateGaussianProbability(X.get(0, featureIndex), mean, variance);
                        // Store the probability in featureProbs map
                        featureProbs.put(featureIndex, featureProb);
                    } else if (FEATURE_HEADER[featureIndex] == FeatureType.DISCRETE) {
                        // For discrete features (frequency-based probability)
                        // Calculate frequency distribution
                        double featureProb = calculateFrequencyProbability(classSamples, featureIndex);
                        // Store the probability in featureProbs map
                        featureProbs.put(featureIndex, featureProb);
                    }
                }
                featureProbabilities.put(classLabel, featureProbs);
            }
        }

        private double calculateMean(List<Matrix> samples, int featureIndex) {
            double sum = 0.0;
            for (Matrix sample : samples) {
                sum += sample.get(0, featureIndex);
            }
            return sum / samples.size();
        }
        
        private double calculateVariance(List<Matrix> samples, int featureIndex) {
            double mean = calculateMean(samples, featureIndex);
            double sumOfSquares = 0.0;
            for (Matrix sample : samples) {
                double diff = sample.get(0, featureIndex) - mean;
                sumOfSquares += diff * diff;
            }
            return sumOfSquares / samples.size();
        }
        
        private double calculateGaussianProbability(double value, double mean, double variance) {
            double exponent = -((value - mean) * (value - mean)) / (2 * variance);
            double numerator = Math.exp(exponent);
            double denominator = Math.sqrt(2 * Math.PI * variance);
            return numerator / denominator;
        }
        
        private double calculateFrequencyProbability(List<Matrix> samples, int featureIndex) {
            Map<Double, Integer> valueCounts = new HashMap<>();
            for (Matrix sample : samples) {
                double value = sample.get(0, featureIndex);
                valueCounts.put(value, valueCounts.getOrDefault(value, 0) + 1);
            }
            double totalSamples = samples.size();
            double totalUniqueValues = valueCounts.size();
            double featureProbability = 0.0;
            for (double value : valueCounts.keySet()) {
                double count = valueCounts.get(value);
                double probability = count / totalSamples;
                featureProbability += probability;
            }
            return featureProbability / totalUniqueValues;
        }

        // TODO: complete me!
        public int predict(Matrix x)
        {
            double maxProbability = Double.NEGATIVE_INFINITY;
            int predictedClass = -1;

            // For each class label
            for (int classLabel : classProbabilities.keySet()) {
                double classProbability = classProbabilities.get(classLabel);

                // Calculate probability of features given class
                double featureProbability = calculateFeatureProbability(x, classLabel);

                // Multiply class probability by feature probability
                double totalProbability = classProbability * featureProbability;

                // Update predicted class if total probability is greater than max probability
                if (totalProbability > maxProbability) {
                    maxProbability = totalProbability;
                    predictedClass = classLabel;
                }
            }
            return predictedClass;
        }

        private double calculateFeatureProbability(Matrix x, int classLabel) {
            double probability = 1.0;
    
            // For each feature
            for (int featureIndex = 0; featureIndex < x.getShape().getNumCols(); featureIndex++) {
                // Calculate probability of the feature given the class
                double featureValue = x.get(0, featureIndex);
                double featureProb = featureProbabilities.get(classLabel).get(featureIndex);
                // You need to implement how you calculate the probability here based on the distribution
                // For continuous features, you can use Gaussian distribution
                // For discrete features, you can use frequency-based probability
                probability *= featureProb;
            }
            return probability;
        }

    }
    
    private NaiveBayes model;

    public NaiveBayesAgent(int playerNum, String[] args)
    {
        super(playerNum, args);
        this.model = new NaiveBayes();
    }

    public NaiveBayes getModel() { return this.model; }

    @Override
    public void train(Matrix X, Matrix y_gt)
    {
        System.out.println(X.getShape() + " " + y_gt.getShape());
        this.getModel().fit(X, y_gt);
    }

    @Override
    public int predict(Matrix featureRowVector)
    {
        return this.getModel().predict(featureRowVector);
    }

}