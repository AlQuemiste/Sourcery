from collections import namedtuple

Point = namedtuple("EvalPoint", 'x f i_next',
                   defaults=[None, None, None])

class State:
    """ computational state """
    class Pt:
        def __init__(self, i=None, x=None, f=None, i_next=None):
            self.reset(i, x, f, i_next)

        def reset(self, i, x, f, i_next):
            self.i:int = i
            self.x:float = x
            self.f:float = f
            self.i_next:int = i_next

    def __init__(self, depth_max):
        # left
        self.L = State.Pt()
        # right
        self.R = State.Pt()
        # stack of pointer to right limits (workspace)
        self.rightLim:'[int]' = [None] * (depth_max + 1)
        self.lv = None # level index
        self.n_feval:int = 0
        self.info = None

    def setL(self, i, x, f, i_next):
        self.L.reset(i, x, f, i_next)

    def setR(self, i, x, f, i_next):
        self.R.reset(i, x, f, i_next)

    def swapRL(self):
        self.L, self.R = self.R, self.L


def adaptive_fn(fn:'Function(x)', a, b, reld=1e-3, absd=1e-3,
                depth_max=1<<5, fevals_max=0):
    """ adaptive function evaluation """

    assert(depth_max > 0 and (b - a) > absd)

    s = State(depth_max)
    x0 = a; f0 = fn(a)
    x1 = b; f1 = fn(b)
    x2 = 0.5 * (x0 + x1); f2 = fn(x2)  # = xM
    # function evaluations
    fevals = [Point(x0, f0, 2), Point(x1, f1, None), Point(x2, f2, 1)]
    # nr of function evaluations
    n_fevals = 3
    s.setL(0, x0, f0, 2)
    s.setR(2, x2, f2, 1)
    s.rightLim[0] = 1 # refers to x1 = b
    s.rightLim[1] = 2 # refers to x2 = xM
    s.lv = 1
    while s.lv >= 0:
        s.R.i = s.rightLim[s.lv]
        _p = fevals[s.R.i]
        s.R.x, s.R.f, s.R.i_next = _p.x, _p.f, _p.i_next
        # assert(xL < xR)
        level_done = (
            # smoothness
            abs(s.L.f - s.R.f) <= absd + reld * abs(s.L.f)
            # limits reached
            or s.lv == depth_max
            or (fevals_max > 0 and n_fevals > fevals_max)
        )

        if not level_done:
            # bisect the interval and insert a point in the middle
            xM = 0.5 * (s.R.x + s.L.x)
            if abs(xM - s.L.x) >= absd:
                # set xR to xM, i_next_R to i_next_L
                s.setR(n_fevals, xM, fn(xM), s.L.i_next)
                fevals.append(Point(s.R.x, s.R.f, s.R.i_next))
                s.L.i_next = s.R.i # set nextL to iR
                fevals[s.L.i] = fevals[s.L.i]._replace(i_next = s.L.i_next)
                n_fevals += 1
                s.lv += 1 # go to the next level
                s.rightLim[s.lv] = s.R.i
                continue
        #END if

        # smoothness in the interval
        # set xL to xR
        s.swapRL()
        s.lv -= 1 # go to the previous level

    # ordered points
    pt_ordered = [None] * len(fevals)
    n_pts = len(pt_ordered)
    idx = fevals[0].i_next
    pt_ordered[0] = 0
    x0 = fevals[0].x
    cc = 1
    while cc < n_pts:
        pt_ordered[cc] = idx
        x1 = fevals[idx].x
        if (x0 >= x1):
            raise AssertionErrorassert("Error: at index = %d: x: %f <= %f"
                                       % (idx, x1, x0))
        x0 = x1
        idx = fevals[idx].i_next
        cc += 1

    return fevals, pt_ordered
#----------------------------------------

# test
def test_adaptive_fn():
    """ Test adaptive function evaluation """
    import math as ma, random as rnd
    from matplotlib import pyplot as plt

    fn = lambda x: ma.tanh(10 * x) # + 1e-2 * (rnd.random() - 0.5)
    reld = 1e-2
    absd = reld * 0.1
    a, b = -0.5, 0.5
    fevals, pt_ordered = adaptive_fn(fn, a, b, reld, absd, fevals_max=1e3)

    # plot
    Xs = [fevals[i].x for i in pt_ordered]
    Ys = [fevals[i].f for i in pt_ordered]
    _ymax = 1.05 * max(Ys)
    _ymin = min(Ys)
    for x in Xs:
        plt.vlines(x, _ymin, _ymax, linestyles='solid',
                   linewidth=0.5, color='gray')

    plt.plot(Xs, Ys, color='orangered', marker='',
             linestyle='solid',linewidth=1, markersize=1)
    #fig_metadata = '\n'.join(parameterStr(params))
    #plt.savefig(plotfile, dpi=300, format='svg', metadata={'Title': fig_metadata})
    plt.show()


if __name__ == "__main__":
    test_adaptive_fn()
