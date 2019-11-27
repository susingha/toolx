class tree:
    def __init__(self, val):
        self.val = val
        self.l = self.r = None

def trim(node):
    ret1 = ret2 = True

    if node.l:
        ret1 = trim(node.l)
        if ret1 == True:
            node.l = None

    if node.r:
        ret2 = trim(node.r)
        if ret2 == True:
            node.r = None

    if ret1 and ret2 and node.val == 0:
        return True
    return False


