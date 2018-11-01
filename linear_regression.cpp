#include <mlpack/methods/linear_regression/linear_regression.hpp>
#include <mlpack/core/data/load_csv.hpp>
#include <armadillo>

#include <string>
#include <iostream>

using namespace mlpack::regression;
using namespace arma;
using namespace std;

int main(){
  mat data;
  mat train_features;
  mat test_features;
  rowvec train_responses_bid, train_responses_ask;
  rowvec test_responses_bid, test_responses_ask, test_prediction;
  mlpack::data::Load("data/dataA_int_parsed.csv", data);

  int nrow=data.n_rows;
  int ncol=data.n_cols;
  int traincol=int(0.7*ncol);
  if (traincol>20000) { traincol=20000;}
  cout << nrow << ' ' << ncol << endl;
  cout << traincol << endl;

  train_features=data(span(3,nrow-1),span(0,traincol-1));
  train_responses_bid=data(span(1,1),span(0,traincol-1)); //row0=time row1=bidprice row2=askprice
  train_responses_ask=data(span(2,2),span(0,traincol-1)); //row0=time row1=bidprice row2=askprice

  test_features=data(span(3,nrow-1),span(traincol,ncol-1));
  test_responses_bid=data(span(1,1),span(traincol,ncol-1)); //row0=time row1=bidprice row2=askprice
  test_responses_ask=data(span(2,2),span(traincol,ncol-1)); //row0=time row1=bidprice row2=askprice

  cout << train_features.n_rows << ' ' << test_features.n_cols << endl;

  //bid 
  LinearRegression lr(train_features, train_responses_bid);
  vec parameters = lr.Parameters();
  cout << parameters << endl;

  lr.Predict(test_features, test_prediction);
  
  rowvec delta=test_responses_bid-test_prediction;
  cout << test_responses_bid.cols(0,10) << ' ' << test_prediction.cols(0,10) << endl;
  double norm2=norm(delta,2)/delta.size();
  double r2=1- norm2/var(test_responses_bid);
  cout << "bid prediction norm2 = " << norm2 << endl;
  cout << "bid R2= " << r2 << endl;
  double sum=0;
  for (int i=1; i<test_responses_bid.size()-1; i++)
    sum+=std::pow(test_responses_bid[i]-test_responses_bid[i-1],2);
  cout << sum/test_responses_bid.size() << endl;

}
