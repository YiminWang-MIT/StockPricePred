#include <mlpack/methods/linear_regression/linear_regression.hpp>
#include <mlpack/core/data/load_csv.hpp>
#include <armadillo>

#include <string>
#include <iostream>

using namespace mlpack::regression;
using namespace arma;
using namespace std;

int main(){
  mat data = randu<mat>(4,5) ;
  cout << data << endl;
  cout << data.n_rows << ' ' << data.n_cols << endl;
  cout << data.row(0) << data.row(3) << endl;
  cout << data.cols(1,4) << endl;
  cout << data(span(1,2),span(2,3)) << endl;
}
