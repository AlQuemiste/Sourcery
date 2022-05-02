from pyparsing import Literal, CaselessLiteral, Word,\
                      Combine, Group, Optional,\
                      ZeroOrMore, Forward, nums, alphas, oneOf
import numpy as np

'''
code is based on < http://pyparsing.wikispaces.com/file/view/fourFn.py >
copyright 2003-2006 by Paul McGuire
and < http://stackoverflow.com/a/2371789 >
'''

class NumericStringParser(object):
    ''' evaluates a string as numerical expression '''

    def pushFirst(self, strg, loc, toks):
        self.exprStack.append(toks[0])

    def pushUMinus(self, strg, loc, toks):
        if toks and toks[0] == '-':
            self.exprStack.append('unary -')
            
    def insertArrayIndex(self, strg, loc, toks):
        self.exprStack.append(toks[0])

    def __init__(self, Array):

        self.exprStack = []
        self.Array = Array
        #------------------------------
        """
        expop   :: '^'
        multop  :: '*' | '/'
        addop   :: '+' | '-'
        integer :: ['+' | '-'] '0'..'9'+
        atom    :: PI | E | real | fn '(' expr ')' | '(' expr ')'
        factor  :: atom [ expop factor ]*
        term    :: factor [ multop factor ]*
        expr    :: term [ addop term ]*
        $i      :: (i: integer) ith element of the given array
        $$      :: all elements of the given array
        """

        # numbers: real/complex
        point = Literal(".")
        e = CaselessLiteral("E")
        IJ = CaselessLiteral("I") | CaselessLiteral("J") # = sqrt(-1)
        # real number
        rNumber = Combine(Word("+-" + nums, nums) +
                          Optional(point + Optional(Word(nums)))
                          + Optional(e + Word("+-" + nums, nums)))
        # complex number
        cNumber = Combine(rNumber + Word("+-")
                          + IJ + rNumber)

        # real or complex number
        Number = rNumber | cNumber
        
        ident = Word(alphas)              # function names
        array_idx = Word("$" + nums)      # array index
        array_all = Literal("$$")         # all arrays
        plus = Literal("+")
        minus = Literal("-")
        mult = Literal("*")
        div = Literal("/")
        lpar = Literal("(").suppress()
        rpar = Literal(")").suppress()
        addop = plus | minus
        multop = mult | div
        expop = Literal("^")
        pi = CaselessLiteral("PI")
        expr = Forward()
        atom = (
                ( Optional(oneOf("- +")) +
                 (ident + lpar + expr + rpar | pi | e | Number).setParseAction(self.pushFirst)
                )
                | ( Optional(oneOf("- +")) + Group(lpar + expr + rpar) )
                | array_idx.setParseAction(self.insertArrayIndex)
                | array_all.setParseAction(self.insertArrayIndex)
                ).setParseAction(self.pushUMinus) 
        # by defining exponentiation as "atom [ ^ factor ]..." instead of
        # "atom [ ^ atom ]...", we get right-to-left exponents, instead of left-to-right
        # that is, 2^3^2 = 2^(3^2), not (2^3)^2.
        factor = Forward()
        factor << atom + \
            ZeroOrMore((expop + factor).setParseAction(self.pushFirst))
        term = factor + \
            ZeroOrMore((multop + factor).setParseAction(self.pushFirst))
        expr << term + \
            ZeroOrMore((addop + term).setParseAction(self.pushFirst))
        # addop_term = ( addop + term ).setParseAction( self.pushFirst )
        # general_term = term + ZeroOrMore( addop_term ) | OneOrMore( addop_term)
        # expr <<  general_term
        self.bnf = expr
        
        # map operator symbols to corresponding arithmetic operations
        epsilon = 1e-12
        self.opn = {"+": np.add,
                    "-": np.subtract,
                    "*": np.multiply,
                    "/": np.true_divide,
                    "^": np.power}
        self.fn = {"sin": np.sin,
                   "cos": np.cos,
                   "tan": np.tan,
                   "exp": np.exp,
                   "abs": np.abs
                   }
                   # "trunc": lambda a: int(a),
                   # "round": round,
                   # "sgn": lambda a: abs(a) > epsilon and cmp(a, 0) or 0}

        return
        
    def evaluateStack(self, s):
        op = s.pop()
        print("$ op = {}".format(op))
        
        if op == 'unary -':
            return -self.evaluateStack(s)
        if op in "+-*/^":
            op2 = self.evaluateStack(s)
            op1 = self.evaluateStack(s)
            return self.opn[op](op1, op2)
        elif op == "PI":
            return math.pi  # 3.1415926535
        elif op == "E":
            return math.e  # 2.718281828
        elif op == "$$":
            return float(0)
        elif op[0] == "$":
            return self.Array[int(op[1:])]
        elif op in self.fn:
            return self.fn[op](self.evaluateStack(s))
        # elif op[0].isalpha():
        #     return 0
        else:
            return float(op)

    def eval(self, num_string, parseAll=True):
        self.exprStack = []

        # parsing
        results = self.bnf.parseString(num_string, parseAll)
        print("$ numerical string:\n'{}'".format(num_string))
        print("$ parse results:\n", results)
        print("$ expression stack:\n", self.exprStack)
        
        # evaluation
        val = self.evaluateStack(self.exprStack[:])
        # NOTE: only a _copy_ of `exprStack` is sent as argument
        
        return val

#----------------------------------------
ARR0 = -np.ones(shape=(20), dtype=float)
nsp = NumericStringParser(ARR0)
result = nsp.eval('+2^4 / $1')
print(result)
# 16.0

result = nsp.eval('- exp(2^4)')
print(result)
# 8886110.520507872

result = nsp.eval('1+ 2 + $12 + 0 + exp($$)')
print(result)
