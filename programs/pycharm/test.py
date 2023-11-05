   #count

class MyCircularQueue:
    def __init__(self,k):
        self.size = k
        self.array = [None] * k
        self.b = 0
        self.count = 0

        
    def enQueue(self, value):
        if self.count == self.size:
            return False
        
        self.array[self.b] = value
        if self.b == self.size-1:
            self.b = 0
        else:
            self.b += 1

        self.count += 1
        return True

    def deQueue(self):
        if self.count == 0:
            return False
        self.count -= 1
        return True


q = MyCircularQueue(3)
print q.deQueue()
print q.enQueue(1)
print q.enQueue(1)
print '=========='
print q.deQueue()
print q.deQueue()
print q.deQueue()
print '=========='
print q.enQueue(1)
print q.deQueue()
print q.deQueue()
print q.enQueue(1)
print q.enQueue(1)
print q.enQueue(1)
print q.enQueue(1)