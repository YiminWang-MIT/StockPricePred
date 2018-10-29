#include <mlpack/methods/linear_regression/linear_regression.hpp>
#include <mlpack/core/data/load_csv.hpp>
#include <armadillo>

#include <string>
#include <iostream>

using namespace mlpack::regression;
using namespace arma;

int main(){
  mat data;
  rowvec responses;
  mlpack::data::Load("data/dataA_train_pre.csv", data);
  mlpack::data::Load("data/dataA_train_res.csv", responses);

  std::cout << data.n_rows << ' ' << data.n_cols << std::endl;
  std::cout << responses.n_rows << ' ' << responses.n_cols << std::endl;


  LinearRegression lr(data, responses);
  std::cout << __LINE__ << std::endl;
  vec parameters = lr.Parameters();
  rowvec resprime;
  lr.Predict(data, resprime);
  
  rowvec delta=responses-resprime;
  double norm2=norm(delta,2)/delta.size();

  std::cout << norm2 << std::endl;



  std::cout << __LINE__ << std::endl;
  mat points;
  rowvec predictions;
  rowvec truevalue;
  mlpack::data::Load("data/dataA_test_pre.csv", points);
  mlpack::data::Load("data/dataA_test_res.csv", truevalue);

  std::cout << __LINE__ << std::endl;
  std::cout << truevalue.size() << ' ' << truevalue[0] << std::endl;
  double sum=0;
  for (int i=1; i<truevalue.size()-1; i++)
    sum+=std::pow(truevalue[i]-truevalue[i-1],2);
  std::cout << sum/truevalue.size() << std::endl;

  lr.Predict(points, predictions);
  
  mlpack::data::Save("data/dataA_test_result.csv", predictions);
  mlpack::data::Save("data/parameters.csv", parameters);

  rowvec delta2=truevalue-predictions;
  norm2=norm(delta2,2)/delta.size();

  std::cout << norm2 << std::endl;
}
