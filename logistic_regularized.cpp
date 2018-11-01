#include <mlpack/methods/logistic_regression/logistic_regression.hpp>
#include <mlpack/core/data/load_csv.hpp>
#include <armadillo>

#include <string>
#include <iostream>

using namespace mlpack::regression;
using namespace arma;
using namespace std;

int main(){
  mat dataA, dataB, train_features, test_features;
  mlpack::data::Load("data/dataB_int_parsed.csv", dataA);
  mlpack::data::Load("data/dataB_int_parsed.csv", dataB);

  int nrow=dataA.n_rows;
  int ncol=dataA.n_cols;
  int traincol=int(0.5*ncol);
  if (traincol>20000) { traincol=20000;}
  cout << "data size:" << nrow << '*' << ncol << endl;
  cout << "training sample:" << traincol << endl;

  // time bid ask bidsz asksz @ t_pred | bid ask bidsz asksz @ t0 ... t-n  

  //train_features=join_vert(dataA(span(3,nrow-1),span(0,traincol-1)),dataB(span(3,nrow-1),span(0,traincol-1)));
  train_features=dataA(span(5,nrow-1),span(0,traincol-1));
  //test_features=join_vert(dataA(span(3,nrow-1),span(traincol, ncol-1)),dataB(span(3,nrow-1),span(traincol, ncol-1)));
  test_features=dataA(span(5,nrow-1),span(traincol, ncol-1));

  //three outcomes: decrease/same/increase
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
  
  //one against others for each trend
  LogisticRegression<mat> *lg[3];
  mat trainv[3];
  mat testv[3];
  for (int i=0; i<=2; i++){
    for (int j=0; j<train_responses_bid.size(); j++){
      vector<bool> flag (3,0);
      flag[0]=(train_currentbid[j]<train_previousbid[j]); 
      flag[1]=(train_currentbid[j]==train_previousbid[j]); 
      flag[2]=(train_currentbid[j]>train_previousbid[j]); 
      //set the response
      if (flag[i])
        train_responses_bid[j]=1;
      else
        train_responses_bid[j]=0;
    }
    train_responses_bid0=arma::conv_to<arma::Row<size_t>>::from(train_responses_bid);
    //lg[i] =new LogisticRegression<mat>(train_responses_bid.size(),1);
    lg[i] =new LogisticRegression<mat>(train_features,train_responses_bid0,1);
    //lg[i]->Train(train_features,train_responses_bid);
    lg[i]->Classify(train_features, trainv[i]);
    lg[i]->Classify(test_features, testv[i]);
  }

  //classify based on maximum value 
  for (int i=0; i<3; i++){
    testv[i]=testv[i].row(1);
    trainv[i]=trainv[i].row(1);
  }

  for (int i=0; i<test_prediction.size(); i++){
    if ((testv[0].row(0)[i]>testv[1].row(0)[i]) and (testv[0].row(0)[i]>testv[2].row(0)[i])){
      test_prediction[i]=0;
      continue;
    }
    if ((testv[1].row(0)[i]>testv[0].row(0)[i]) and (testv[1].row(0)[i]>testv[2].row(0)[i])){
      test_prediction[i]=1;
      continue;
    }
    if ((testv[2].row(0)[i]>testv[0].row(0)[i]) and (testv[2].row(0)[i]>testv[1].row(0)[i])){
      test_prediction[i]=2;
      continue;
    }
  } 
  for (int i=0; i<train_prediction.size(); i++){
    if ((trainv[0].row(0)[i]>trainv[1].row(0)[i]) and (trainv[0].row(0)[i]>trainv[2].row(0)[i])){
      train_prediction[i]=0;
      continue;
    }
    if ((trainv[1].row(0)[i]>trainv[0].row(0)[i]) and (trainv[1].row(0)[i]>trainv[2].row(0)[i])){
      train_prediction[i]=1;
      continue;
    }
    if ((trainv[2].row(0)[i]>trainv[0].row(0)[i]) and (trainv[2].row(0)[i]>trainv[1].row(0)[i])){
      train_prediction[i]=2;
      continue;
    }
  } 

  cout << "Test Classify probabilities:" << endl;
  cout << testv[0].row(0).cols(0,10);
  cout << testv[1].row(0).cols(0,10);
  cout << testv[2].row(0).cols(0,10);
  cout << "Test Classify samples:" << endl;
  cout << test_prediction.cols(0,10) << endl;
  //error count
  vector<int> test_correct(3,0);
  vector<int> test_count(3,0);
  vector<int> train_correct(3,0);
  vector<int> train_count(3,0);

  int traintot=0, traincor=0, testtot=0, testcor=0;
  int traingain=0, testgain=0;
  for (int i=0; i<train_prediction.size(); i++){
      int flag=0;
      if (train_currentbid[i]<train_previousbid[i]) flag=0; 
      if (train_currentbid[i]==train_previousbid[i]) flag=1; 
      if (train_currentbid[i]>train_previousbid[i]) flag=2; 
      train_count[flag]++;
      traintot++;
      if (train_prediction[i]==flag){
        train_correct[flag]++;
        traincor++;
        traingain+=abs(flag-1);
      } else
        traingain-=abs(train_prediction[i]-flag);
  }

  for (int i=0; i<test_prediction.size(); i++){
      int flag=0;
      if (test_currentbid[i]<test_previousbid[i]) flag=0; 
      if (test_currentbid[i]==test_previousbid[i]) flag=1; 
      if (test_currentbid[i]>test_previousbid[i]) flag=2; 
      test_count[flag]++;
      testtot++;
      if (test_prediction[i]==flag){
        test_correct[flag]++;
        testcor++;
        testgain+=abs(flag-1);
      } else 
        testgain-=abs(test_prediction[i]-flag);
  }

  cout << traintot << ' ' << double(traincor)/traintot << endl;
  cout << testtot << ' ' << double(testcor)/testtot << endl;
  
  for (int i=0; i<3; i++)
    cout << train_count[i] << ' ' << double(train_correct[i])/train_count[i] << endl;
  
  for (int i=0; i<3; i++)
    cout << test_count[i] << ' ' <<  double(test_correct[i])/test_count[i] << endl;

  cout << traingain << ' ' << testgain << endl;
  
}
