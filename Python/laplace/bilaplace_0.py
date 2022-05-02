# BiLaplace transformation

import numpy as np
from scipy.integrate import quad as Integrate
from scipy.special import erf
from math import fsum

class BiLaplace:
    ''' BiLaplace transformation within 64bit floating-point precision '''

    def __init__(self):

        # integration boundaries, [a, b]
        self.a = 0.
        self.b = 0.

        # number of segments
        self.seg_nr = 0

        # results of Laplace integrations on segments
        self.Laplace = np.array([], dtype=np.float64)

        # final result of the BiLaplace transform, (exponent, factor)
        self.result = (0., 0.)

        # function to be transformed
        self.F = None

        # factor of the exponential, as 's' in e^{- s x}
        self.s = 0.

        # max. exponent of e which determines sub-interval size and number
        self.expmax = np.log(1e16) # <-- e^{exp_max} = 10^16
        self.intvsize = 0.
        self.intvnr = 0
        self._intvidx_LIM = 0

        # relative and absolute tolerances
        self.RTOL = 1e-12
        self.ATOL = 1e-12
        
        self.prm_set = False

        return
    
    def Set(self):
        ''' verifies and sets the parametres for transformation '''
        
        # sets integration boundaries
        if self.b < self.a:
            print("BiLaplace: interval definition is incorrect; needs [a,b] with b .GE. a !")
            return False
        #END if
        
        #TODO: algorithm for negative exponential factor
        if not self.s > 0.:
            print("BiLaplace: exponential factor must be positive definite !")
            return False

        # determine size and number of sub-intervals
        self.intvsize = self.expmax / self.s
        intvNR = (self.b - self.a) / self.intvsize
        # array to store intermediate results
        self.res_ARR = np.empty(shape=(int(np.ceil(intvNR))), dtype=float)
        self.intvnr = len(self.res_ARR)
        self._intvidx_LIM = int(intvNR) # could be 1 less than length of the results array

        self.prm_set = True


    def _Do(self):
        ''' performs the sub-integrations '''

        if not self.prm_set:
            print("BiLaplace: parametres are not set properly !")
            return False
        
        x2 = x1 = self.a
        for n in range(0, self._intvidx_LIM):            
            x2 = x1 + self.intvsize
            L_n = self._Laplace(x1, x2)
            self.res_ARR[n] = L_n

            print("$ {}) [{}, {}] -> {}".format(n, x1, x2, L_n))

            x1 = x2
        #END for

        # if the whole region is not covered yet
        if x2 < self.b:
            L_n = self._Laplace(x2, self.b)
            print("$ {}) [{}, {}] -> {}".format(self.intvnr, x2, self.b, L_n))
            self.res_ARR[-1] = L_n
            
        return

    def _Laplace(self, x1, x2):
        ''' performs partial Laplace transforms '''

        res = Integrate(lambda x: np.exp(-self.s * (x - x1) ) * self.F(x),
                         a=x1, b=x2,
                         epsabs=self.ATOL, epsrel=self.RTOL)[0]
        return res
    
    def Reap(self):
        ''' yields the final result in float64 precision, if possible '''

        self._Do()
        RNG_ = np.arange(0, self.intvnr, dtype=float)
        EXP_ = np.exp(-self.s * RNG_)
        SUM_B = 0
        
        #TODO: prevent NaN or INF; use Kahan summation
        for e_, x_ in zip(EXP_, self.res_ARR):
            if e_ > self.ATOL and np.abs(x_) > self.ATOL:
                SUM_B += e_ * x_
            
        self.result = (SUM_B, -self.s * self.a)
           
    def __repr__(self):
        TXT = "BiLaplace transformation\n" \
            + "\int_{{{}}}^{{{}}} dx e^{{-{} * x}} F(x)\n".format(self.a, self.b, self.s) \
            + "max. exponent: {}\n".format(self.expmax) \
            + "{} intervals of size {}\n".format(self.intvnr, self.intvsize) \
            + "partial results:\n{}\n".format(self.res_ARR) \
            + "final result: {}".format(self.result)
              
        return TXT
        

#------------------------------------------------------------------------------80


def test_BiLaplace():
    ''' unit-test: BiLaplace transform '''

    #----------------------------------------
    X0 = 0.
    W0 = 1.
    def fnk(x):
        if np.abs(x - X0) <= W0: return 1.0
        return 0.
    #----------------------------------------
    
    BL = BiLaplace()
    BL.RTOL = BL.ATOL = 1e-12
    BL.a = -100.
    BL.b = 100.
    BL.s = 10.
    BL.F = fnk
    BL.Set()
    BL.Reap()

    fnk = lambda x: np.exp(-BL.s * x) * BL.F(x)
    # EXPECTED_RES = Integrate(fnk, a=BL.a, b=BL.b, epsabs=1e-12, epsrel=1e-12)[0]
    EXPECTED_RES = 2. * np.exp(-BL.s * X0) * np.sinh(BL.s * W0)
    BL_RES = BL.result[0] * np.exp(BL.result[1]) if BL.result[0] != 0. else 0.
    print("$", BL)
    MSG = "{} is not close to {}, abs. diff. = {:.3e}".format(EXPECTED_RES,
                                                        BL_RES, np.abs(EXPECTED_RES-BL_RES) )
    assert np.isclose([EXPECTED_RES], [BL_RES], atol=BL.ATOL, rtol=BL.RTOL), MSG


if __name__ == "__main__":
    test_BiLaplace()
    
