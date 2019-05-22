class node:
    def __init__(self, val):
        self.l = None
        self.r = None
        self.val = val



head = None
f1 = False
f2 = False

def lca(node, n1, n2):
    global f1
    global f2

    fl = None
    fr = None

    if node.l:
        fl = lca(node.l, n1, n2)

    if node.r:
        fr = lca(node.r, n1, n2)

    if node.val == n1:
        f1 = True
        return node.val
    if node.val == n2:
        f2 = True
        return node.val

    if fl and fr:
        return node.val

    if fl:
        return fl
    if fr:
        return fr

    return None