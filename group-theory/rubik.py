from group import Cycle

# operations
# up, down, front, back, left, right
# 用两个字母表示棱块的面, 如 uf 表示上面的前面, fu 表示前面的上面
# 三个字母表示角块的面, 如 ufl 表示上面的前左角
op = {
    'u': Cycle('uf', 'ul', 'ub', 'ur') *
         Cycle('fu', 'lu', 'bu', 'ru') *
         Cycle('ufl', 'ubl', 'ubr', 'ufr') *
         Cycle('ful', 'lub', 'bur', 'ruf') *
         Cycle('luf', 'bul', 'rub', 'fur'),
    'd': Cycle('df', 'dr', 'db', 'dl') *
         Cycle('fd', 'rd', 'bd', 'ld') *
         Cycle('dfl', 'dfr', 'dbr', 'dbl') *
         Cycle('fdl', 'rdf', 'bdr', 'ldb') *
         Cycle('ldf', 'fdr', 'rdb', 'bdl'),
    'f': Cycle('fu', 'fr', 'fd', 'fl') *
         Cycle('uf', 'rf', 'df', 'lf') *
         Cycle('ful', 'fur', 'fdr', 'fdl') *
         Cycle('ufl', 'ruf', 'dfr', 'ldf') *
         Cycle('luf', 'ufr', 'rdf', 'dfl'),
    'b': Cycle('bu', 'bl', 'bd', 'br') *
         Cycle('ub', 'lb', 'db', 'rb') *
         Cycle('bul', 'bdl', 'bdr', 'bur') *
         Cycle('ubl', 'ldb', 'dbr', 'rub') *
         Cycle('lub', 'dbl', 'rdb', 'ubr'),
    'l': Cycle('lu', 'lf', 'ld', 'lb') *
         Cycle('ul', 'fl', 'dl', 'bl') *
         Cycle('luf', 'ldf', 'ldb', 'lub') *
         Cycle('ufl', 'fdl', 'dbl', 'bul') *
         Cycle('ful', 'dfl', 'bdl', 'ubl'),
    'r': Cycle('ru', 'rb', 'rd', 'rf') *
         Cycle('ur', 'br', 'dr', 'fr') *
         Cycle('ruf', 'rub', 'rdb', 'rdf') *
         Cycle('ufr', 'bur', 'dbr', 'fdr') *
         Cycle('fur', 'ubr', 'bdr', 'dfr')
}

def macro(s: str):
    p = Cycle()
    cycle = None
    for c in s:
        if c.islower():
            cycle = op[c]**(-1)
        elif c.isupper():
            cycle = op[c.lower()]
        elif c != '2':
            raise ValueError
        p *= cycle
    return p

def test1():
    p1 = macro('F2R2')
    # (fl fr br) (lf rf rb) (ur dr) (rub luf rdf) (fdr bur ful) (fu fd) (rdb ldf ruf) (ru rd) (uf df) (fur bdr fdl) (dfr ubr ufl) (ufr dbr dfl)
    print(p1.factor())
    print(p1.ord()) # 6

    p2 = p1**3
    # (ru rd) (fd fu) (ur dr) (uf df)
    print(p2.factor())
    print(p2.ord()) # 2

# 翻转一组相邻的棱
def test2():
    p1 = macro('FRB')
    p2 = macro('LUlU')
    p3 = macro('luLu')
    p = p2.conj(p1) * p3
    # (ful ubl ruf bur dbl) (ur fr lu ru rf ul) (fur rub bdl luf lub) (ub br fu bu rb uf) (ubr ldb ufl bul ufr) 不对劲
    print(p.factor())

# 翻转一组相邻的棱
def test3():
    m = macro('RlFFLrdRlFLr')
    u = macro('u')
    p = m * u * m.inv() * u.inv()
    # (bu ru fu ld ub ur uf dl) (lu df ul fd) 很不对劲
    print(p.factor())

# 翻转一组对棱
def test4():
    m = macro('FUL2R')
    p = m**18
    print(p.factor())
    print(p.ord()) # 绝对不对劲, 应该是 2

# 顶角块的三轮换
def test5():
    m = macro('LrDRdl')
    u = macro('U')
    p = m.commutor(u)
    print(p.factor())

# 平行棱块的三轮换
def test6():
    m = macro('lRFFrLDD')
    print(m.factor())
    print(m.ord()) # 不对, 应该是 3

print((op['b']**2).factor())
