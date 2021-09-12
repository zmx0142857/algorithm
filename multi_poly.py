""" doc test
>>> Term('x', 1, -1)
-x
>>> MultiPoly(Term('x', 1, -1), Term('y', 2, 2), Term('z', 0, 3))
3 + 2y^2 + -x
>>> MultiPoly(
...    Term('x', 1, Term('y', 1, 2)), Term('y', 2), Term('x', 2)
... )
x^2 + y^2 + (2y)x
"""

from fractions import Fraction as Frac

# 单项式 coef * sym^exp
class Term:
    def __init__(self, sym, exp=1, coef=1, next=None):
        self.coef = coef
        self.sym = sym
        self.exp = exp
        self.next = next

    def __str__(self):
        coef = self.coef
        if coef == 1 or coef == 0:
            coef = ''
        elif coef == -1:
            coef = '-'
        elif isinstance(self.coef, int) or isinstance(self.coef, float) or isinstance(self.coef, Frac):
            pass
        else:
            coef = f'({self.coef})'

        sym = self.sym
        if self.coef == 0 or self.exp == 0:
            sym = ''

        exp = self.exp
        if exp == 0 or exp == 1 or self.coef == 0:
            exp = ''
        else:
            exp = f'^{exp}'

        return f'{coef}{sym}{exp}'

    __repr__ = __str__

# 多项式 (链表实现)
# 处理多元多项式, 使用主元法, 先看作第一个变量的一元多项式, 依此类推
class MultiPoly:
    def __init__(self, *args):
        if len(args) == 1 and isinstance(args[0], str):
            self.head = Term(1, arg, 1)
        else:
            self.head = None
            # 所有项次序反向
            for term in args:
                term.next = self.head
                self.head = term

    def __iter__(self):
        self._it = Term(0, '', 0, self.head)
        return self

    def __next__(self):
        self._it = self._it.next
        if self._it == None:
            raise StopIteration
        return self._it

    def __str__(self):
        def add_sign(i, term):
            return str(term) if i == 0 else f' + {term}'
        return ''.join(add_sign(*item) for item in enumerate(self))

    __repr__ = __str__

if __name__ == '__main__':
    import doctest
    doctest.testmod()
