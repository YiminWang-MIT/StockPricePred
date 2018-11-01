#include <mlpack/methods/lars/lars.hpp>
#include <mlpack/core/data/load_csv.hpp>
#include <armadillo>

#include <string>
#include <iostream>

using namespace mlpack::regression;
using namespace arma;
using namespace std;

int main(){
  mat dataA, dataB, train_features, test_features;
  rowvec train_responses_bid, train_responses_ask;
  rowvec test_responses_bid, test_responses_ask, test_prediction, train_prediction;
  mlpack::data::Load("data/dataA_int_parsed.csv", dataA);
  mlpack::data::Load("data/dataB_int_parsed.csv", dataB);

  int nrow=dataA.n_rows;
  int ncol=dataA.n_cols;
  int traincol=int(0.7*ncol);
  if (traincol>20000) { traincol=20000;}
  cout << nrow << ' ' << ncol << endl;
  cout << traincol << endl;

  //train_features=join_vert(dataA(span(3,nrow-1),span(0,traincol-1)),dataB(span(3,nrow-1),span(0,traincol-1)));
  train_features=dataA(span(3,nrow-1),span(0,traincol-1));
  train_responses_bid=dataA(span(1,1),span(0,traincol-1)); //row0=time row1=bidprice row2=askprice
  train_responses_ask=dataA(span(2,2),span(0,traincol-1)); //row0=time row1=bidprice row2=askprice

  //test_features=join_vert(dataA(span(3,nrow-1),span(traincol, ncol-1)),dataB(span(3,nrow-1),span(traincol, ncol-1)));
  test_features=dataA(span(3,nrow-1),span(traincol, ncol-1));
  test_responses_bid=dataA(span(1,1),span(traincol,ncol-1)); //row0=time row1=bidprice row2=askprice
  test_responses_ask=dataA(span(2,2),span(traincol,ncol-1)); //row0=time row1=bidprice row2=askprice

  cout << train_features.n_rows << ' ' << train_features.n_cols << endl;

  //bid 
  vec parameters;
  LARS lars = new LARS(train_features, train_responses_bid, true, false, 10, 0.0, 1e-16);
  lars.Train(train_features, train_responses_bid, parameters);
  cout << parameters << endl;
  auto lp = lars.LambdaPath() ;
  for (auto lm: lp)
    cout << lm << endl;

  //test_features=train_features;
  //test_responses_bid=train_responses_bid;

  lars.Predict(test_features, test_prediction);
  lars.Predict(train_features, train_prediction);
  
  rowvec delta=test_responses_bid-test_prediction;
  double norm2=norm(delta,2)*norm(delta,2)/delta.size();
  double r2=1- norm2/var(test_responses_bid);
  cout << "Prediction MSE = " << norm2 << endl;
  cout << "bid R2= " << r2 << endl;

  delta=train_responses_bid-train_prediction;
  norm2=norm(delta,2)*norm(delta,2)/delta.size();
  r2=1- norm2/var(train_responses_bid);
  cout << "Training MSE = " << norm2 << endl;
  cout << "bid R2= " << r2 << endl;

  double sum=0;
  rowvec naive=test_features.row(2);
  for (int i=0; i<test_responses_bid.size(); i++)
    sum+=pow(naive[i]-test_responses_bid[i],2);
  cout << "naive MSE =" << sum/test_responses_bid.size() << endl;

  mat test_out=join_vert(join_horiz(train_responses_bid,test_responses_bid),join_horiz(train_prediction,test_prediction));
  mlpack::data::Save("data/dataA_linear_reg.csv", test_out);
}
