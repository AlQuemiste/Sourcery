# BiLaplace transformation

import numpy as np
from scipy.integrate import quad as Integrate
from scipy.special import erf

class BiLaplace:
    ''' BiLaplace transformation within 64bit floating-point precision '''

    LOG2E = np.log2(np.e)
    
    def __init__(self):

        # integration boundaries, [a, b]
        self.a = 0.
        self.b = 0.

        # function to be transformed
        self.F = None

        # factor of the exponential, as 's' in e^{- s x}
        self.s = 0.

        # max. exponent of e which determines sub-interval size and number
        self.expmax = 10
        #Q? why 10 is a _magical_ number here?

        self.intvsize = 0.

        # relative and absolute tolerances
        self.RTOL = 1e-12
        self.ATOL = 1e-12

        return
      
    def Do(self):
        ''' performs the sub-integrations '''

        # verifying parametres for transformation
        
        # sets integration boundaries
        if self.b < self.a:
            print("BiLaplace: interval definition is incorrect; needs [a,b] with b .GE. a !")
            return np.nan
        #END if
        
        #TODO: algorithm for negative exponential factor
        if not self.s > 0.:
            print("BiLaplace: exponential factor must be positive definite !")
            return np.nan
        #----------------------------------------
        
        # determine size and number of sub-intervals
        # intervals start at x_i = X_0 + i * interval-size
        X_0 = self.expmax / self.s
        self.intvsize = 2.0 * X_0
        i_st = int( (self.a - X_0) / self.intvsize )
        
        i__ = 0                 #  DEBUG
        SUM_ = (0., 0.)
        
        # initial segment (if needed)
        x1 = self.a
        x2 = X_0 + i_st * self.intvsize
        if x1 < x2:
            i__ += 1                    # DEBUG
            L_n = self._Laplace(x1, x2) # DEBUG
            SUM_ = self.Add(SUM_, L_n)
            # print("$ {:3}) [{:5}, {:5}] -> {} , sum = {}".format(i__, x1, x2, L_n, SUM_))
                        
            
        # middle segment(s)
        x1 = x2
        x2 = x1 + self.intvsize

        while x2 <= self.b:
            i__ += 1
            L_n = self._Laplace(x1, x2)
            SUM_ = self.Add(SUM_, L_n)
            # print("$ {:3}) [{:5}, {:5}] -> {} , sum = {}".format(i__, x1, x2, L_n, SUM_))
            
            x1 = x2
            x2 += self.intvsize

        # final segment (if needed)
        if x1 < self.b:
            i__ += 1
            L_n = self._Laplace(x1, self.b)
            SUM_ = self.Add(SUM_, L_n)
            # print("$ {:3}) [{:5}, {:5}] -> {} , sum = {}".format(i__, x1, x2, L_n, SUM_))

        return SUM_

    def _Laplace(self, x1, x2):
        ''' performs partial Laplace transforms.

        NOTE:
        \int_{x1}^{x2} e^{-s x} f(x) = e^{-s x1} \int_{x1}^{x2} e^{-s (x - x1)} f(x)
        =: e^{-s x1} L ,
        which is represented as (L, p) := L * 2^p , p = -s * x1 * log_2(e)
        '''

        # partial Laplace transform
        intg = Integrate(lambda x: np.exp(-self.s * (x - x1) ) * self.F(x),
                         a=x1, b=x2,
                         epsabs=self.ATOL, epsrel=self.RTOL)[0]

        # result in form A * 2^a
        # NOTE: e^{-a} = 2^{-a * log2(e)}
        res = (intg, -x1 * self.s * self.LOG2E)
        
        return res

    def Add(self, N1, N2):
        ''' add two numbers given in 'exponential' representation
        N = (mantissa, exponent) := mantissa * 2^exponent

        NOTE: extraction of mantissa and exponent can be performed faster
        via bitwise manipulations.
        '''
        
        if np.abs(N1[0]) < self.ATOL: return N2
        if np.abs(N2[0]) < self.ATOL: return N1
        
        #ELSE:
        
        # extract mantissa and exponents for factors
        mnt1, exp1 = np.frexp(N1[0])
        mnt2, exp2 = np.frexp(N2[0])

        # add exp1, exp2 to the exponents of N1, N2
        exp1 += N1[1]
        exp2 += N2[1]

        # set a and b so that exponent of a >= exponent of b
        '''
        NOTE:
        for two numbers N1 = A * 2^a and N2 = B * 2^b,
        if a >= b, then
        A * 2^a + B * 2^b = 2^a ( A + B * 2^(b-a) ) .
        note that b-a <= 0, therefore 2^(b-a) <= 1 ;
        '''

        #CHECK: |A| < 1 and |B| < 1 always?

        if exp1 >= exp2:
            exp_a , mnt_a = exp1, mnt1
            exp_b , mnt_b = exp2, mnt2
        else:
            exp_a , mnt_a = exp2, mnt2
            exp_b , mnt_b = exp1, mnt1

        return (mnt_a + mnt_b * np.exp2(exp_b - exp_a), exp_a)
        #END if
        
    def __repr__(self):
        TXT = "BiLaplace transformation\n" \
            + "\int_{{{}}}^{{{}}} dx e^{{-{} * x}} F(x)\n".format(self.a, self.b, self.s) \
            + "max. exponent: {}\n".format(self.expmax) \
            + "interval size: {}".format(self.intvsize)
              
        return TXT
        

#------------------------------------------------------------------------------80


def test_BiLaplace():
    ''' unit-test: BiLaplace transform '''

    print("*** TEST: BiLaplace transform ***")

    #----------------------------------------
    X0 = 0.
    W0 = 1.
    def fnk(x):
        # Rectangle(x - x0, w0)
        if np.abs(x - X0) <= W0: return 1.0
        return 0.
    #----------------------------------------
    
    BL = BiLaplace()
    BL.RTOL = BL.ATOL = 1e-8
    BL.a = -100.
    BL.b = 100.
    BL.s = 10.
    BL.F = fnk
    BL_result = BL.Do()

    print("*** \n{}\n***".format(BL))

    # fnk = lambda x: np.exp(-BL.s * x) * BL.F(x)
    # EXPECTED_RES = Integrate(fnk, a=BL.a, b=BL.b, epsabs=1e-12, epsrel=1e-12)[0]
    EXPECTED_RES = 2./BL.s * np.exp(-BL.s * X0) * np.sinh(BL.s * W0)
    BL_RES = BL_result[0] * np.exp2(BL_result[1]) if BL_result[0] != 0. else 0.
    MSG = "expected result = {}\n" \
          "obtained result = {}\n".format(EXPECTED_RES, BL_RES)
    MSG += "abs. diff. = {:.3e},\n".format(np.abs(EXPECTED_RES-BL_RES))
    MSG += "rel. diff. = {:.3e}".format(np.abs((EXPECTED_RES-BL_RES)/EXPECTED_RES))
    print(MSG)
    assert np.isclose([EXPECTED_RES], [BL_RES], atol=BL.ATOL, rtol=BL.RTOL), \
        "results do _not_ match !"

#------------------------------------------------------------------------------80
if __name__ == "__main__":
    test_BiLaplace()

#==============================================================================80
