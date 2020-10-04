#!/usr/bin/env python3

from functools import reduce

def check_type(value, expect):
    if not isinstance(value, expect):
        raise TypeError('expecting %s, got %s' % (expect, type(value)))

# mask for the highest bit
def begin(length):
    return 1 << (length-1)

class Bits:
    def __init__(self, *args):
        if len(args) == 1:
            if isinstance(args[0], int):
                self.init2(*args)
            else:
                self.init1(*args)
        elif len(args) == 2:
            self.init2(*args)

    # string: a str of 0's and 1's in big endian
    def init1(self, bit_str):
        check_type(bit_str, str)
        self.len = len(bit_str)
        self.data = 0
        mask = 1
        for b in reversed(bit_str):
            if b == '1':
                self.data |= mask
            elif b != '0':
                raise ValueError('invalid character %s' % b)
            mask <<= 1

    def init2(self, length, data=0):
        check_type(length, int)
        check_type(data, int)
        self.len = length
        self.data = data

    def __len__(self):
        return self.len

    def __str__(self):
        buf = []
        mask = 1
        for i in range(self.len):
            if self.data & mask:
                buf.append('1')
            else:
                buf.append('0')
            mask <<= 1
        return ''.join(reversed(buf))

    def __iter__(self):
        mask = self.begin()
        while mask:
            yield self.data & mask
            mask >>= 1

    # mask for the highest bit
    def begin(self):
        return 1 << (self.len-1)

    def __and__(self, other):
        if isinstance(other, Bits):
            other = other.data
        return Bits(self.len, self.data & other)

    def __or__(self, other):
        if isinstance(other, Bits):
            other = other.data
        return Bits(self.len, self.data | other)

    def __xor__(self, other):
        if isinstance(other, Bits):
            other = other.data
        return Bits(self.len, self.data ^ other)

    def __lshift__(self, other):
        return Bits(self.len, self.data << other)

    def __rshift__(self):
        return Bits(self.len, self.data >> other)

    __repr__ = __str__

def parity(bits):
    return reduce(
        lambda x, y: x ^ y,
        (1 if b else 0 for b in bits)
    )

# input: 00110001110
# output: 0111
def hamming_digest(bits):
    ret = reduce(
        lambda x, y: x ^ y,
        (i for (i, b) in enumerate(bits) if b)
    )
    return Bits(bits.len.bit_length(), ret)

# input: 00110001110
# output: 0010101110001110
def hamming_encode(bits):
    digest = hamming_digest(bits)
    length = 1 << digest.len
    ret = Bits(length)
    powof2 = 1
    dmask = digest.begin()
    rmask = bits.begin()
    wmask = ret.begin() >> 1
    for i in range(1, length):
        if i == powof2:
            if digest.data & dmask:
                ret |= wmask
            powof2 <<= 1
            dmask >>= 1
        else:
            if bits.data & rmask:
                ret |= wmask
            rmask >>= 1
        wmask >>= 1

    if parity(ret):
        ret ^= ret.begin()
    return ret

def hamming_decode(bits):
    digest = Bits(bits.len.bit_length()-1)
    msg = Bits(bits.len-digest.len-1)
    parity = 0
    powof2 = 1
    dmask = digest.begin()
    mmask = msg.begin()
    for (i, b) in enumerate(bits):
        if i == 0:
            parity = b
            continue
        if i == powof2:
            if b:
                digest |= dmask
            dmask >>= 1
            powof2 <<= 1
        else:
            if b:
                msg |= mmask
            mmask >>= 1
    return msg, digest, parity

bits = Bits('00110001110')
print('bits:', bits)
digest = hamming_digest(bits)
print('digest:', digest)
encoded = hamming_encode(bits)
print('encoded:', encoded)
decoded = hamming_decode(encoded)
print('decoded:', decoded)
