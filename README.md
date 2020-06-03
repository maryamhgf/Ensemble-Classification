# Ensemble-Classification-in-C++
This project is an Ensemble Classification of different flower types written in C++.  The input features are the length and the width of flower petals, and the goal is to classify the flower in one of the three flower groups. Using this ensemble classifier, our model achieves a high accuracy of 97.2% on test data.
The base estimator for this ensemble classifier is a linear classifier, the number of estimators is 10, and the prediction is based on voting.
The dataset.csv contains two columns: Length and Width of petals and they are for three types of flowers. In the weight-vectors folder, you can find the coefficients of linear classifiers for each of the three classes (the result of the training model). The validation folder contains validation data.
You can compile the code with g++ compiler, and run it by the following command: ./execuate.out Assets/validation Assets/weight_vector
