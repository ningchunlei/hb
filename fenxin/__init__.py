__author__ = 'ningcl'


HIGH=1
LOW=1
FENING_QUEKOU=1.01
FENING_BAOHAN=0.01
LEFTQUEKOU=1
RIGHTQUEKOU=2


class KLine(object):
    def __init__(self):
        self.low=-1
        self.high=-1
        self.left=-1
        self.right=-1
        self.index
        self.avaliable=False
        self.top = -1;
        self.buttom=-1;
        self.lineType=-1;
        self.leftquekou = -1
        self.rightquekou = -1

def leftHighMerge(index,kdata,kline):
    if index == 0 :
        return;
    higherThanBefore=False
    smallThanBefore = False
    if kline.high >= kdata[index-1].high:
        higherThanBefore = True
    if kline.low <= kdata[index-1].low :
        smallThanBefore = True
    if higherThanBefore and smallThanBefore:
        kline.low = kdata[index-1].low
        leftHighMerge(index-1,kdata,kline)
    elif not higherThanBefore:
        kline.avaliable = False
    elif not smallThanBefore:
        kline.left = index - 1
        if kline.buttom == -1 or kline.buttom > kdata[index-1].low:
            kline.buttom = kdata[index-1].low
        kline.avaliable = True

def rightHighMerge(index,kdata,kline):
    if index == len(kdata)-1:
        return;

    higherThanAfter=False
    smallThanAfter=False
    if kline.high >= kdata[index].high:
        higherThanAfter = True
    if kline.low <= kdata[index].low:
        smallThanAfter = True

    if higherThanAfter and smallThanAfter:
        kline.low = kdata[index+1].low
        rightHighMerge(index+1,kdata,kline)
    elif not higherThanAfter:
        kline.avaliable = False
    elif not smallThanAfter:
        kline.right = index+1
        if kline.buttom == -1 or kline.buttom>kdata[index+1].low:
            kline.buttom = kdata[index+1].low
        kline.avaliable = True


def leftLowMerge(index,kdata,kline):
    if index == 0:
        return
    higherThanBefore=False
    smallThanBefore = False
    if kline.high >= kdata[index-1].high:
        higherThanBefore = True
    if kline.low <= kdata[index-1].low:
        smallThanBefore = True

    if higherThanBefore and smallThanBefore :
        kline.high = kdata[index-1].high
        leftLowMerge(index-1,kdata,kline)
    elif not smallThanBefore:
        kline.avaliable = False
    elif not higherThanBefore:
        kline.left = index -1
        if kline.top == -1 or kline.top < kdata[index-1].high :
            kline.top = kdata[index-1].top
        kline.avaliable = True

def rightLowMerge(index,kdata,kline):
    if index == len(kdata) -1 :
        return
    higherThanAfter = False
    smallThanAfter = False
    if kline.high >= kdata[index+1].high:
        higherThanAfter=True
    if kline.low <= kdata[index+1].low:
        smallThanAfter = True

    if higherThanAfter and smallThanAfter:
        kline.high = kdata[index+1].high
        rightLowMerge(index+1,kdata,kline)
    elif not smallThanAfter:
        kline.avaliable = False
    elif not higherThanAfter:
        kline.right = index+1
        if kline.top ==-1 or kline.top < kdata[index+1].high :
            kline.top = kdata[index+1].high
        kline.avaliable = True

def fenxin(kdata):
    gvec = []
    for index in range(1,len(kdata)):
        higherThanBefore=False
        higherThanAfter=False

        smallThanBefore=False
        smallThanAfter=False
        if kdata[index].high - kdata[index-1].high >=0:
            higherThanBefore = True
        if kdata[index].high - kdata[index+1].high >=0:
            higherThanAfter = True

        if kdata[index].low - kdata[index-1].low <=0:
            smallThanBefore = True
        if kdata[index].low - kdata[index+1].low <=0:
            smallThanAfter = True

        if higherThanAfter and higherThanBefore :
            kk = KLine()
            kk.high=kdata[index].high
            kk.low=kdata[index].low
            kk.index=index
            kk.type==HIGH
            kk.top=kdata[index].high
            klow = -1
            if kdata[index].low > kdata[index-1].high*FENING_QUEKOU:
                kk.leftquekou = LEFTQUEKOU
            if kdata[index].low > kdata[index+1].high*FENING_QUEKOU:
                kk.rightquekou = RIGHTQUEKOU

            if kk.leftquekou == LEFTQUEKOU:
                kk.avaliable=True
                kk.left = index
                kk.buttom = kdata[index].low
            else:
                leftHighMerge(index,kdata,kk)
                klow = kk.low
                kk.low = kdata[index].low

            if kk.avaliable==True:
                if kk.rightquekou == RIGHTQUEKOU:
                    kk.avaliable = True
                    kk.right=index;
                else:
                    rightHighMerge(index,kdata,kk)
                if kk.right != -1:
                    gvec.append(kk)
            if kk.low <= klow:
                kk.low = klow
        if smallThanAfter and smallThanBefore:
            kk = KLine
            kk.high=kdata[index].high
            kk.low=kdata[index].low
            kk.index=index
            kk.type==LOW
            kk.buttom=kdata[index].low
            shigh = kdata[index].high

            if kdata[index].high * FENING_QUEKOU < kdata[index-1].low:
                kk.leftquekou = LEFTQUEKOU
            if kdata[index].high * FENING_QUEKOU < kdata[index+1].low:
                kk.rightquekou = RIGHTQUEKOU

            if kk.leftquekou == LEFTQUEKOU:
                kk.avaliable = True
                kk.left = index
                kk.top = kdata[index].high
            else:
                leftLowMerge(index,kdata,kk)
                shigh = kk.high
                kk.high = kdata[index].high

            if kk.avaliable==True:
                if kk.rightquekou ==RIGHTQUEKOU:
                    kk.avaliable=True
                    kk.right = index
                else:
                    rightLowMerge(index,kdata,kk)
                if kk.right != -1 :
                    gvec.append(kk)
            if kk.high > shigh :
                kk.high = shigh


    if len(gvec) ==0 :
        return

    kvec = []
    kvec.append(gvec[0])
    lineType = gvec[0].type
    del gvec[0]

    for cur in gvec:
        kline = kvec[len(kvec)-1]
        if kline.type==HIGH:
            if cur.type == HIGH:
                if cur.high >= kline.high:
                    del kvec[len(kvec)-1]
                    if len(kvec)!=0:
                        kline = kvec[len(kvec)-1]
                        if kline.buttom >= cur.buttom:
                            continue
                    kvec.append(cur)
                continue
            if kline.top <= cur.top:
                continue
            if kline.rightquekou == RIGHTQUEKOU or cur.leftquekou == LEFTQUEKOU:
                kvec.append(cur)
                lineType = cur.type
                continue
            if kline.right > cur.left or cur.index - kline.index < 3 or (cur.index-kline.index == 3 and kdata[kline.right].low<=kdata[cur.left].high):
                continue

            stop = kline.top
            sbuttom = cur.buttom
            if kline.top > cur.top:
                stop = cur.top
            if kline.buttom > cur.buttom:
                sbuttom = cur.buttom
            diff = stop - sbuttom
            ldiff = abs(diff)/(kline.top - kline.buttom)
            rdiff = abs(diff)/(cur.top - cur.buttom)
            if (diff<=0 or (ldiff <= FENING_BAOHAN and rdiff <=FENING_BAOHAN ) ) and kline.high>cur.high and kline.low > cur.low:
                kvec.append(cur)
                lineType = cur.type
        else:
            if cur.type == LOW:
                if cur.low <= kline.low:
                    del kvec[len(kvec)-1]
                    if len(kvec)!=0:
                        kline = kvec[len(kvec)-1]
                        if kline.top <= cur.top:
                            continue
                    kvec.append(cur)
                continue
            if kline.buttom >= cur.buttom:
                continue
            if kline.rightquekou == RIGHTQUEKOU or cur.leftquekou == LEFTQUEKOU:
                kvec.append(cur)
                lineType = cur.type
                continue
            if kline.right > cur.left or cur.index - kline.index < 3 or (cur.index-kline.index ==3 and kdata[kline.right].high >= kdata[cur.left].low):
                continue
            stop = kline.top
            sbuttom = cur.buttom
            if kline.top > cur.top :
                stop = cur.top
            if kline.buttom > cur.buttom:
                sbuttom = kline.buttom
            diff = stop - sbuttom
            ldiff = abs(diff)/(kline.top - kline.buttom)
            rdiff = abs(diff)/(cur.top - cur.buttom)
            if (diff<=0 or (ldiff <= FENING_BAOHAN and rdiff <= FENING_BAOHAN)) and cur.high > kline.high and cur.low > kline.low:
                kvec.push(cur)
                lineType = cur.type

