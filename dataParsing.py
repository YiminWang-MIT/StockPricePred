import csv

def basic_price(sym):
    with open('data/data2.csv') as csv_infile:
        with open('data/data'+sym+'.csv', mode='w') as csv_outfile:
            csv_reader=csv.reader(csv_infile, delimiter=',')
            csv_writer=csv.writer(csv_outfile, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
            line_count=0
            lasttime=-1
            tottime=0
            totline=0
            for row in csv_reader:
                if line_count>=1:
                    [time, symbol, bid, bidsz, ask, asksz, trdsz, trdsd, trdpx]=row
                    if bid!='0' and ask!='0' and symbol=='STOCK'+sym:
                        if lasttime==-1:
                            lasttime=long(time)
                        else:
                            timedif=long(time)-lasttime
                            lasttime=long(time)
                            tottime+=timedif
                            csv_writer.writerow([time,symbol,bid,ask,timedif])
                        totline+=1
                line_count+=1
            print tottime, totline

def basic_trd(sym):
    with open('data/data2.csv') as csv_infile:
        with open('data/data'+sym+'_trd.csv', mode='w') as csv_outfile:
            csv_reader=csv.reader(csv_infile, delimiter=',')
            csv_writer=csv.writer(csv_outfile, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
            line_count=0
            lasttime=-1
            tottime=0
            totline=0
            for row in csv_reader:
                if line_count>=1:
                    [time, symbol, bid, bidsz, ask, asksz, trdsz, trdsd, trdpx]=row
                    if trdsz!='0' and trdsd!='0' and symbol=='STOCK'+sym:
                        if lasttime==-1:
                            lasttime=long(time)
                        else:
                            timedif=long(time)-lasttime
                            lasttime=long(time)
                            tottime+=timedif
                            #csv_writer.writerow([time,symbol,bid,ask,timedif])
                            #csv_writer.writerow([time,symbol,trdsz,trdsd,trdpx])
                        totline+=1
                line_count+=1
            print tottime, totline

def price_interpolate(sym):
    with open('data/data2.csv') as csv_infile:
        with open('data/data'+sym+'_int.csv', mode='w') as csv_outfile:
            csv_reader=csv.reader(csv_infile, delimiter=',')
            csv_writer=csv.writer(csv_outfile, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
            line_count=0
            t0=-1
            tstep=1000
            count=1000
            lasttime=-1
            lastprice=0
            for row in csv_reader:
                if line_count>=1:
                    [time, symbol, bid, bidsz, ask, asksz, trdsz, trdsd, trdpx]=row
                    if bid!='0' and ask!='0' and symbol==('STOCK'+sym):
                        if t0==-1:
                            t0=long(time)
                            lasttime=long(time)
                            lastprice=long(bid)
                            csv_writer.writerow([time,symbol,bid,ask])
                        else:
                            if long(time)-t0<count*tstep:
                                lasttime=long(time)
                                lastprice=long(bid)
                            else:
                                currenttime=t0+count*tstep
                                slope=float(int(bid)-lastprice)/(long(time)-lasttime)
                                while long(time)>currenttime:
                                    #print time,bid,lasttime,lastprice,slope,currenttime
                                    value=lastprice+(currenttime-lasttime)*slope
                                    csv_writer.writerow([currenttime,symbol,value,ask])
                                    count+=1
                                    currenttime=t0+count*tstep
                                lasttime=long(time)
                                lastprice=long(bid)
                line_count+=1

def training(sym):
    '''
    predictors: long term and short term trend
        t-1: bidprice deltaT1
        t-2: bidprice deltaT2
        t-3: bidprice deltaT3
        t-4: bidprice deltaT4
        t-20: bidprice deltaT20
        t-50: bidprice deltaT50
        t-100: bidprice deltaT100
        t-200: bidprice deltaT200
        t-500: bidprice deltaT500
        t-1000: bidprice deltaT1000

    response:
        t: bidprice
    '''
    with open('data/data2.csv') as csv_infile:
        with open('data/data'+sym+'_pre.csv', mode='w') as csv_outfile_pre:
            with open('data/data'+sym+'_res.csv', mode='w') as csv_outfile_res:
                csv_reader=csv.reader(csv_infile, delimiter=',')
                csv_writer_pre=csv.writer(csv_outfile_pre, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
                csv_writer_res=csv.writer(csv_outfile_res, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
                basename='STOCK'+sym
                tm1=[]
                tm2=[]
                csv_reader.next() # get rid fo title line
                line_count=0
                #dump the first 2500 lines (too much fluctuations)
                for i in xrange(2500):
                    csv_reader.next()
                old_data=[]
                #collect the first 1001 entries 
                while len(old_data)<1001:
                    row=csv_reader.next()
                    [time, symbol, bid, bidsz, ask, asksz, trdsz, trdsd, trdpx]=row
                    if symbol==basename:
                        if bid!='0' and ask!='0':
                            bidprice=long(bid)
                            askprice=long(ask)
                        else:
                            bidprice=long(trdpx)
                            askprice=long(trdpx)
                        statuslist=[time, bidprice, askprice]
                        if tm1 and tm2:
                            #if different from previous time
                            if statuslist[1]!=tm1[1] or statuslist[2]!=tm1[2]:
                                #if tm1==tm2, tm1 end of same price, need to store
                                if tm2[1]==tm1[1] and tm2[2]==tm1[2]:
                                    old_data.append(tm1)
                                #otherwise tm1 already written in file
                                old_data.append(statuslist)
                        else:
                            old_data.append(statuslist)
                        tm2=list(tm1)
                        tm1=list(statuslist)
                #parsing predictors
                for row in csv_reader:
                    [time, symbol, bid, bidsz, ask, asksz, trdsz, trdsd, trdpx]=row
                    if symbol==basename:
                        if bid!='0' and ask!='0':
                            bidprice=long(bid)
                            askprice=long(ask)
                        else:
                            bidprice=long(trdpx)
                            askprice=long(trdpx)
                        statuslist=[time, bidprice, askprice]
                        #if different from previous time
                        if statuslist[1]!=tm1[1] or statuslist[2]!=tm1[2]:
                            #if tm1==tm2, tm1 end of same price, need to store
                            if tm2[1]==tm1[1] and tm2[2]==tm1[2]:
                                csv_writer_pre.writerow([tm1[0]]+old_data[-2]+old_data[-3]+old_data[-4]+old_data[-5]+old_data[-11]+old_data[-21]+old_data[-51]+old_data[-101]+old_data[-201]+old_data[-501]+old_data[-1001])
                                csv_writer_res.writerow([tm1[1]])

                            #otherwise tm1 already written in file
                            old_data=old_data[1:]
                            old_data.append(statuslist)
                            csv_writer_pre.writerow([statuslist[0]]+old_data[-2]+old_data[-3]+old_data[-4]+old_data[-5]+old_data[-11]+old_data[-21]+old_data[-51]+old_data[-101]+old_data[-201]+old_data[-501]+old_data[-1001])
                            csv_writer_res.writerow([statuslist[1]]);
                        tm2=list(tm1)
                        tm1=list(statuslist)
                #parsing predictors

def output(time, trueresult, predictions):
    with open('data/'+time+'.csv') as csv_infile1:
        with open('data/'+trueresult+'.csv') as csv_infile2:
            with open('data/'+predictions+'.csv') as csv_infile3:
                with open('data/'+predictions+'_combined.csv', mode='w') as csv_outfile:
                    csv_reader1=csv.reader(csv_infile1, delimiter=',')
                    csv_reader2=csv.reader(csv_infile2, delimiter=',')
                    csv_reader3=csv.reader(csv_infile3, delimiter=',')
                    csv_writer=csv.writer(csv_outfile, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
                    for row in csv_reader1:
                        timevalue=row[0] # get rid fo title line
                        truev=csv_reader2.next()[0] # get rid fo title line
                        prev=csv_reader3.next()[0] # get rid fo title line
                        csv_writer.writerow([timevalue,truev,prev]);

output('dataA_test_pre','dataA_test_res','dataA_test_result')
