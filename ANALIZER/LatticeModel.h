//
//  LatticeModel.h
//  ANALIZER
//
//  Created by Pato Churro on 1/22/15.
//  Copyright (c) 2015 Nina Corp. All rights reserved.
//


#ifndef LATTICEMODEL_H_
#define LATTICEMODEL_H_

#include <string>
#include <vector>
#include <array>
#include <map>

class LatticeModel {
public:
    // Constructor for training a new model.
    // Inputs:
    //  n: number of features, i.e. dimensions.
    //  t: number of intervals per dimension. So, the number of lattice points per dimension is (t_+1).
    //  data: vector of vectors of dimensions [m][n+1]. Each data[i][0] is the value of the response variable
    //        associated with features data[i][1 thru n].
    //  alpha: gradient descent step size parameter
    //  lambda: regularization parameter
    //  e: convergence parameter: stop when the norm of the gradient falls below this value.
    LatticeModel(int n, int t, const std::vector<std::vector<double>>& data, double alpha, double lambda, double e);
    
    // Constructor for reading saved model from disk.
    LatticeModel(const std::string& filename);
    
    virtual ~LatticeModel();
    
    // Save the model in a serialized format on disk.
    void writeToDisk(const std::string& filename) const;
    
    // Returns predicted response given some features. The input array should have length n.
    double predict(const double features[]) const;
    
    // Returns root mean squared error of model on data which is formatted the same as in the ctor
    double getRMSE(const std::vector<std::vector<double>>& data) const;
    
private:
    // Number of features
    int n_;
    
    // Number of divisions per dimension. So, the number of lattice points per dimension is (t_+1).
    int t_;
    
    // Length of a division, i.e. 1.0/t.
    double div_len_;
    
    // Number of lattice points, (t_+1)^n_
    int num_lattice_points_;
    
    // Helper function called by both constructors to initialize adj_lattice_pts_ which holds indices
    // of lattice points adjacent to each lattice point. In particular, adj_lattice_pts_[i] will be a
    // vector of all points adjacent to point i.
    void initAdjLatticePoints();
    std::vector<std::vector<int>> adj_lattice_pts_;
    
    // Values of surface at lattice points.
    std::vector<double> b_;
    
    // m is the number of training examples.  Other inputs are as to the constructor.
    void trainModel(const std::vector<std::vector<double>>& data, double alpha, double lambda, double e);
    
    // Initialize supplementary weights and adj structures.
    // Inputs:
    //   m: number of training examples
    //   data: m-by-n+1 array as in constructor
    // Outputs:
    //   weights: vector of length data.size() containing maps from lattice point index to weight for each training example
    //   adjDataToLattice: vector of length data.size() containing arrays of lattice points adjacent to each training example
    //   adjLatticeToData: vector (of length num_lattice_points_) of vectors containing indices (into data) of examples adjacent
    //                     to each lattice point
    void makeTrainingWeightsAndAdj(const std::vector<std::vector<double>>& data, std::vector<std::map<int,double>>& weights, std::vector<std::vector<int>>& adjDataToLattice, std::vector<std::vector<int>>& adjLatticeToData);
    
    // Get non-zero weights (and corresponding adjacent lattice points) for a given set of inputs.
    std::map<int,double> getWeights(const double x[]) const;
    
    // Takes an int array of coordinates of length m_ describing a grid point and returns the corresponding index into b_.
    // NB: The "coordinates" here are integers from 0 to t_.  Multiply them by div_len_ to convert to cartesian.
    int coordsToIndx(const int c[]) const;
    
    // Takes an index into b_ and writes an int array of coordinates.
    void indxToCoords(int indx, int c[]) const;
};

#endif /* LATTICEMODEL_H_ */
