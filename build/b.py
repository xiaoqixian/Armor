class A:
    def __init__(self,v):
        self.val = v

    def __add__(self,a):
        return A(a.val + self.val);
a = A(1)
b = A(2)
c = a + b
print c.val
