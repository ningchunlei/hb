import sqlite3
from datetime import datetime,timedelta
import time
import logging
from logging.handlers import RotatingFileHandler

logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
                    datefmt='%a, %d %b %Y %H:%M:%S',
                    filename='hb.log',
                    filemode='w')

console = logging.StreamHandler()
console.setLevel(logging.INFO)
formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
console.setFormatter(formatter)
logging.getLogger('').addHandler(console)


Rthandler = RotatingFileHandler('hb.log', maxBytes=500*1024*1024,backupCount=5)
Rthandler.setLevel(logging.INFO)
formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
Rthandler.setFormatter(formatter)
logging.getLogger('').addHandler(Rthandler)


table_prefix="ltccny"
cx = sqlite3.connect("./huobi.db")
cu = cx.cursor()


def check_consisten(rows,type):
    peroid = 1*60
    if type == "1min":
        peroid = 60
    elif type == "5min":
        peroid = 5*60
    elif type == "30min":
        peroid = 30*60

    end_time = -1;
    for row in rows:
        if end_time == -1:
            pass
        elif end_time-row==peroid:
            end_time = row;
        else:
            logging.info("check_consisten %s is error" % (type))
            exit(1)

def fenxing(rows,params):
    baohan = params["baohan"]
    quekou = 1+params["quekou"]
    tiaokongflag = params["tiaokong"]
    for index in range(1,len(rows)):
        higherThanBefore = True if rows[index].high >=rows[index-1].high else False
        higherThanAfter = 


while True:
    utcdt = datetime.utcnow()
    ktime = time.mktime((utcdt+timedelta(utcdt.second)).timetuple())
    k_one_line = cu.execute("select rtime,open,high,low,close from %s_%s order by rtime desc limit 2000" % (table_prefix,"1min"))
    k_five_line = cu.execute("select rtime,open,high,low,close from %s_%s order by rtime desc limit 2000" % (table_prefix,"5min"))
    k_half_line = cu.execute("select rtime,open,high,low,close from %s_%s order by rtime desc limit 2000" % (table_prefix,"30min"))
    check_consisten(k_one_line,'1min')
    check_consisten(k_five_line,'5min')
    check_consisten(k_half_line,'30min')

