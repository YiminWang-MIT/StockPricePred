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
            tstep=100000
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

price_interpolate('A')
