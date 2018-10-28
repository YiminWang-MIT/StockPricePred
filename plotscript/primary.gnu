set terminal png
set term png size 2400,600
set datafile separator ","

#plot 1
set title ""

#set mytics 5

set autoscale
set yrange [4120:4160]
set out "plots/stockA.png"
#plot "data/dataA.csv" using 1:3 w l title "STOCKA bid",\
#  "data/dataA_int.csv" using 1:3 w p title "STOCKA bid int"
plot "<(sed -n '2000,3000p' data/dataA.csv)" using 1:3 w l title "STOCKA bid",\
  "<(sed -n '1000,2000p' data/dataA_int.csv)" using 1:3 w l title "STOCKA bid int"
  #"data/dataA.csv" using 1:4 w l title "STOCKA ask",\
  #"data/dataA_trd.csv" using 1:5 w p title "STOCKA trd"
unset yrange
unset out

set yrange [9060:9140]
set out "plots/stockB.png"
plot "data/dataB.csv" using 1:3 w l title "STOCKB bid",\
  "data/dataB.csv" using 1:4 w l title "STOCKB ask",\
  "data/dataB_trd.csv" using 1:5 w p title "STOCKA trd"
unset yrange
unset out

set autoscale
set out "plots/stockA_diff.png"
set yrange [-5:5]
plot "data/dataA.csv" using 1:($4-$3) w l title "STOCKA bid-ask diff"
unset yrange
unset out

set out "plots/stockB_diff.png"
set yrange [-5:5]
plot "data/dataB.csv" using 1:($4-$3) w l title "STOCKB bid-ask diff"
unset out
unset yrange

#unset autoscale
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
