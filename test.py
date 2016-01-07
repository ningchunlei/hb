class KLinePeriod(object):
    One=(1,"1min",60)
    Five=(2,"5min",300)
    Thirty=(3,"30min",1800)
    Day=(4,"day",86400)


x = ((getattr(KLinePeriod,attr)) for attr in dir(KLinePeriod) if not attr.startswith("__"))

for t in x :
    print t

print("xxx")

for t1 in x:
    print t1