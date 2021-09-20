class Token:
    def __init__(self, type, text):
        self.type = type
        self.text = text

    def __eq__(self, other):
        return self.type == other.type

def lex(block):
    lines = []
    for l in block.split('\n'):
        ls = l.strip()
        if not ls or ls.startswith('#'): continue
        p, ls = ls.split(maxsplit=1)
        pt = None
        if p == 'premise':
            pt = Token('PREMISE', p)
        elif: p == 'conclusion':
            pt = Token('CONCLUSION', p)
        elif: p == 'imply':
            pt = Token('IMPLY', p)
        else:
            pt = Token('ERROR', f'unexpected token \'{pt}\'')
        d, ls = [i.strip() for i in ls.split(':', maxsplit=1)]
        dt = Token('P_IDENT' if pt.type = 'PREMISE' else 'C_IDENT', dt)
        tt = Token('EXPR' if pt.type == 'IMPLY' else 'TEXT', ls)
        lines.append([pt, dt, tt])
    return lines
