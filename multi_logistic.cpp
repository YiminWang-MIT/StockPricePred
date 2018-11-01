#include <mlpack/methods/softmax_regression/softmax_regression.hpp>
#include <mlpack/core/data/load_csv.hpp>
#include <armadillo>

#include <string>
#include <iostream>

using namespace mlpack::regression;
using namespace arma;
using namespace std;

int main(){
  mat dataA, dataB, train_features, test_features;
  mlpack::data::Load("data/dataA_int_parsed.csv", dataA);
  mlpack::data::Load("data/dataB_int_parsed.csv", dataB);

  int nrow=dataA.n_rows;
  int ncol=dataA.n_cols;
  int traincol=int(0.7*ncol);
  if (traincol>20000) { traincol=20000;}
  cout << nrow << ' ' << ncol << endl;
  cout << traincol << endl;

  // time bid ask bidsz asksz @ t_pred | bid ask bidsz asksz @ t0 ... t-n  

  //train_features=join_vert(dataA(span(3,nrow-1),span(0,traincol-1)),dataB(span(3,nrow-1),span(0,traincol-1)));
  train_features=dataA(span(5,nrow-1),span(0,traincol-1));
  //test_features=join_vert(dataA(span(3,nrow-1),span(traincol, ncol-1)),dataB(span(3,nrow-1),span(traincol, ncol-1)));
  test_features=dataA(span(5,nrow-1),span(traincol, ncol-1));

  cout << train_features.n_rows << ' ' << train_features.n_cols << endl;

  //three outcomes: decrease/same/increase at the same
  rowvec train_responses_bid = zeros<rowvec>(traincol);
  Row<size_t> train_responses_bid0;
  rowvec train_prediction = zeros<rowvec>(traincol);
  Row<size_t> train_prediction0;
  rowvec train_currentbid=dataA(span(1,1),span(0,traincol-1)); //row0=time row1=bidprice row2=askprice
  rowvec train_previousbid=dataA(span(5,5),span(0,traincol-1)); //row0=time row1=bidprice row2=askprice

  rowvec test_responses_bid = zeros<rowvec>(ncol-traincol);
  Row<size_t> test_responses_bid0;
  rowvec test_prediction = zeros<rowvec>(ncol-traincol);
  Row<size_t> test_prediction0; 
  rowvec test_currentbid=dataA(span(1,1),span(traincol,ncol-1)); //row0=time row1=bidprice row2=askprice
  rowvec test_previousbid=dataA(span(5,5),span(traincol,ncol-1)); //row0=time row1=bidprice row2=askprice
  //test_responses_ask=dataA(span(2,2),span(traincol,ncol-1)); //row0=time row1=bidprice row2=askprice
  //train_responses_ask=dataA(span(2,2),span(0,traincol-1)); //row0=time row1=bidprice row2=askprice
  //
  //
  cout<< __LINE__ << endl;
  
  //multi-class logistic-regression
  for (int j=0; j<train_responses_bid.size(); j++){
    if (train_currentbid[j]<train_previousbid[j])
      train_responses_bid[j]=0;
    if (train_currentbid[j]==train_previousbid[j])
      train_responses_bid[j]=1;
    if (train_currentbid[j]>train_previousbid[j])
      train_responses_bid[j]=2;
  }
  cout<< __LINE__ << endl;
  train_responses_bid0=arma::conv_to<arma::Row<size_t>>::from(train_responses_bid);
  //lg[i] =new LogisticRegression<mat>(train_responses_bid.size(),1);
  cout<< __LINE__ << endl;
  SoftmaxRegression *lg=new SoftmaxRegression(train_features,train_responses_bid0,3,1);
  cout<< __LINE__ << endl;
  //lg[i]->Train(train_features,train_responses_bid);
  lg->Classify(train_features, train_prediction0);
  lg->Classify(test_features, test_prediction0);
  cout<< __LINE__ << endl;
  cout << train_prediction0.n_cols << endl;
  cout << train_prediction0.n_rows << endl;
  cout << train_prediction0.cols(0,10) << endl;

  //error count
  vector<int> test_correct(3,0);
  vector<int> test_count(3,0);
  vector<int> train_correct(3,0);
  vector<int> train_count(3,0);
  cout<< __LINE__ << endl;

  for (int i=0; i<train_prediction0.size(); i++){
      int flag=0;
      if (train_currentbid[i]<train_previousbid[i]) flag=0; 
      if (train_currentbid[i]==train_previousbid[i]) flag=1; 
      if (train_currentbid[i]>train_previousbid[i]) flag=2; 
      train_count[flag]++;
      if (train_prediction[i]==flag) train_correct[flag]++;
  }
  for (int i=0; i<test_prediction0.size(); i++){
      int flag=0;
      if (test_currentbid[i]<test_previousbid[i]) flag=0; 
      if (test_currentbid[i]==test_previousbid[i]) flag=1; 
      if (test_currentbid[i]>test_previousbid[i]) flag=2; 
      test_count[flag]++;
      if (test_prediction[i]==flag) test_correct[flag]++;
  }

  
  for (int i=0; i<3; i++){
    cout << train_count[i] << endl;
    cout << double(train_correct[i])/train_count[i] << endl;
    cout << test_count[i] << endl;
    cout << double(test_correct[i])/test_count[i] << endl;
  }

}
