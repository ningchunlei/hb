__author__ = 'ningcl'

import logging;
import os;
import sqlite3
import time

class KLinePeriod(object):
    One=(1,"1min",60)
    Five=(2,"5min",300)
    Thirty=(3,"30min",1800)
    Day=(4,"day",86400)

def _checkPeriod(cur,pre,diff):
    if cur-pre == diff :
        return True
    elif time.localtime(cur).tm_hour==3 and time.localtime(pre).tm_hour==9 :
        return True
    return False

def listAvaliableStock(config,period):
    error=[]
    success = []
    for fp in os.listdir(config.dataDir):
        cx = sqlite3.connect(os.path.join(config.dataDir,fp))
        cu = cx.cursor()
        kdata = cu.execute("select rtime,open,high,low,close from %s order by rtime desc limit 2000" % (period[1]))
        end_time==-1
        ret = False
        for row in kdata:
            if end_time == -1 :
                end_time = row[0]
            else:
                ret = _checkPeriod(end_time,row[0],period[2])
                if ret == True:
                    end_time = row[0]
                else:
                    break

        if ret==True :
            success.append(fp)
        else:
            error.append(fp)
        cu.close()
        cx.close()
    return (success,error)

class Stock(object):
    logger = logging.getLogger(__name__)

    def __init__(self,datadir):
        self.datadir = datadir

    def __listattr__(self,obj):
        ((getattr(obj,attr)) for attr in dir(obj) if not attr.startswith("__"))

    def data(self,stock):
        kdata={}
        cx = sqlite3.connect(os.path.join(self.datadir,stock))
        cu = cx.cursor()

        for period in self.__listattr__(KLinePeriod):
            rows = cu.execute("select rtime,open,high,low,close from %s order by rtime desc limit 2000" % (period[1]))
            kdata[period[1]]=rows

        cu.close()
        cx.close()
        return kdata




