# Ensemble-Classification-in-C++
Ensemble Classification in C++ for preprocessed data in .csv file. (base estimator is linear classifier, number of estimators is 10, and it predicts the class by voting.)
The dataset.csv contains two columns: Length and Width of petals and they are for three types of flowers. The classifier should  predict the type of flower by length and width of it's petals. 

In weight-vectors folder, you can find the coefficiets of linear classifiers for each of three classes.(The result of training model) 

score(Xi , k) = Betha(k, 0) * Length(i) + Betha(k, 1) * Width(i) + Bias(k) 

k is the number of base classifier and Xi is the ith sample.

Validation folder contains test data.

The accuracy of model is for the test data is: 97.2%

After compiling the code with g++ compiler you shoud type this command to give the dataset to the program: ./execuate.out Assets/validation Assets/weight_vector
