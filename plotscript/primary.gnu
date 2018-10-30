set terminal png
set term png size 2400,600
set datafile separator ","

#plot 1
set title ""

#set mytics 5

set autoscale
set yrange [4120:4160]
#set xrange [19e8:20e8]
set out "plots/stockA.png"
#plot "data/dataA_train.csv" using 1:2 w p title "STOCKA bid",\
#  "data/dataA_train.csv" using 1:3 w p title "STOCKA ask"
#  "data/dataA_int.csv" using 1:3 w p title "STOCKA bid int"
  #"data/dataA_trd.csv" using 1:5 w p title "STOCKA trd"
#plot 'data/dataA.csv' using 1:($1<2000000000 && $1>1900000000?$3:1/0) w p title "STOCKA bid",\
#  'data/dataA_int.csv' using 1:($1<2000000000 && $1>1900000000?$3:1/0) w p title "STOCKA int"
plot "data/dataA.csv" using 1:3 w l title "STOCKA bid",\
  "data/dataA.csv" using 1:4 w l title "STOCKA ask",\
  "data/dataA_trd.csv" using 1:5 w p title "STOCKA trd"
unset yrange
unset out

unset xrange
set yrange [9060:9140]
set out "plots/stockB.png"
plot "data/dataB.csv" using 1:3 w l title "STOCKB bid",\
  "data/dataB.csv" using 1:4 w l title "STOCKB ask",\
  "data/dataB_trd.csv" using 1:5 w p title "STOCKB trd"
unset yrange
unset out

set autoscale
set out "plots/stockA_diff.png"
set logscale y
set boxwidth 1
bw = 1
bin(x,width)=width*floor(x/width) + bw/2.0 
set yrange [1:200000] 
plot 'data/dataB.csv' using (bin($4-$3,bw)):(1.0) smooth freq with boxes title 'STOCKA bid-ask diff'
unset yrange
unset out
unset logscale

set out "plots/stockB_diff.png"
set yrange [-5:5]
plot "data/dataB.csv" using 1:($4-$3) w l title "STOCKB bid-ask diff"
unset out
unset yrange

#unset autoscale
set term png size 1200,600
set xlabel "Delta t [microsec]"
set ylabel "relative number of entries"
set out "plots/time_diff.png"
set logscale y
set boxwidth 100000
bw = 100000
bin(x,width)=width*floor(x/width) + bw/2.0 
set xrange [0:10000000] 
set yrange [1:200000] 
plot 'data/dataB.csv' using (bin($5,bw)):(1.0) smooth freq with boxes title 'timediffB',\
  'data/dataA.csv' using (bin($5,bw)):(1.0) smooth freq with boxes title 'timediffA'
unset out
unset logscale
unset xrange
unset yrange

set term png size 1200,600
#set xlabel "Delta t [microsec]"
#set ylabel "relative number of entries"
set out "plots/size.png"
set logscale y
set boxwidth 1
bw = 1
bin(x,width)=width*floor(x/width) + bw/2.0 
set xrange [0:100] 
set yrange [1:200000] 
plot 'data/data2.csv' using (bin($4,bw)):(1.0) smooth freq with boxes title 'bid',\
  'data/data2.csv' using (bin($6,bw)):(1.0) smooth freq with boxes title 'ask'
unset out
unset logscale
unset xrange
unset yrange
