def map(func, iterable):
    l = []
    for i in iterable:
        l.append(func(i))
    return l

def sum(iterable, i):
    temp = i
    for e in iterable:
        temp = temp + e
    return temp

def range(*args):
    start = 0
    step = 1

    if len(args) == 1:
        end = args[0]
    elif len(args) == 2:
        start = args[0]
        end = args[1]
    else:
        start = args[0]
        end = args[1]
        step = args[2]
    ls = []
    if (start < end and step > 0):
        while start < end:
            ls.append(start)
            start += step
    else:
        print "Error"
    return ls

class Exception(object):
    def __init__(self, *args):
        self.info = args
    def __repr__(slef):#__repr__ reprents all the attributes about the object
        return " ".join(self.info)

def ord(char):
    return 0
