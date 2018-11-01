#include <mlpack/methods/linear_regression/linear_regression.hpp>
#include <mlpack/methods/lars/lars.hpp>
#include <mlpack/core/data/load_csv.hpp>
#include <armadillo>

#include <string>
#include <iostream>

using namespace mlpack::regression;
using namespace arma;
using namespace std;

int main(){
  mat dataA, dataB, train_features, test_features, test_out;
  rowvec train_responses_bid, train_responses_ask;
  rowvec test_responses_bid, test_responses_ask, test_prediction, train_prediction;
  mlpack::data::Load("data/dataA_int_parsed.csv", dataA);
  mlpack::data::Load("data/dataB_int_parsed.csv", dataB);

  int nrow=dataA.n_rows;
  int ncol=dataA.n_cols;

  /*
   * addaptive prediction train with increasing information
   * start with 3 steps of data, increase training data for each step
   */

  const int step = 1800;//lines corresponding to an step

  for (int i=3; i<=5; i++){
    int train_lines=i*step;
    //train using the first i steps
    train_features=dataA(span(3,nrow-1),span(0,train_lines-1));
    train_responses_bid=dataA(span(1,1),span(0,train_lines-1)); //row0=time row1=bidprice row2=askprice
    //predict the next step
    test_features=dataA(span(3,nrow-1),span(train_lines, train_lines+step-1));
    test_responses_bid=dataA(span(1,1),span(train_lines, train_lines+step-1)); //row0=time row1=bidprice row2=askprice

    cout << "training dimensions: " << train_features.n_rows << '*' << train_features.n_cols << endl;

    /*
    LinearRegression lr(train_features, train_responses_bid);
    vec parameters = lr.Parameters();
    cout << parameters << endl;
    lr.Predict(test_features, test_prediction);
    lr.Predict(train_features, train_prediction);
    */
     
    LARS lars = new LARS(train_features, train_responses_bid, true, false, 10, 0.0, 1e-16);
    vec parameters;
    lars.Train(train_features, train_responses_bid, parameters);
    cout << parameters << endl;

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

    test_out=join_horiz(test_out,join_vert(dataA(span(0,0),span(train_lines, train_lines+step-1)),join_vert(test_responses_bid,test_prediction)));
  }

  /*
  train_features=join_vert(dataA(span(3,nrow-1),span(0,traincol-1)),dataB(span(3,nrow-1),span(0,traincol-1)));
  test_features=join_vert(dataA(span(3,nrow-1),span(traincol, ncol-1)),dataB(span(3,nrow-1),span(traincol, ncol-1)));

  //train_features=join_vert(dataA(span(3,nrow-1),span(0,traincol-1)),dataB(span(3,nrow-1),span(0,traincol-1)));
  train_responses_bid=dataA(span(1,1),span(0,traincol-1)); //row0=time row1=bidprice row2=askprice
  train_responses_ask=dataA(span(2,2),span(0,traincol-1)); //row0=time row1=bidprice row2=askprice

  //test_features=join_vert(dataA(span(3,nrow-1),span(traincol, ncol-1)),dataB(span(3,nrow-1),span(traincol, ncol-1)));
  test_responses_bid=dataA(span(1,1),span(traincol,ncol-1)); //row0=time row1=bidprice row2=askprice
  test_responses_ask=dataA(span(2,2),span(traincol,ncol-1)); //row0=time row1=bidprice row2=askprice
  */


  //test_features=train_features;
  //test_responses_bid=train_responses_bid;

  mlpack::data::Save("data/dataA_linear_reg.csv", test_out);
}
