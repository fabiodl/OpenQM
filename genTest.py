

def bigTest():
    out = open("bigTest.bin", "wb")
    for i in range(2**18):
        a, b, c, d, e, f, g, h = [1 if i & (1 << j) else 0 for j in range(8)]

        o7 = a & b | c & d
        o6 = a & c | d
        o5 = a | b | d
        o4 = h & g
        o3 = c & d | a & b
        o2 = a | b
        o1 = a & b & c | a & d
        o0 = 1-d

        outs = (o7 << 7) | (o6 << 6) | (o5 << 5) | (
            o4 << 4) | (o3 << 3) | (o2 << 2) | (o1 << 1) | o0

        out.write(bytes(outs.to_bytes(1, byteorder="big")))
    out.close()


def smallTest():
    out = open("smallTest.bin", "wb")
    for i in range(2**4):
        a, b, c, d = [1 if i & (1 << j) else 0 for j in range(4)]

        o7 = a & c
        o6 = a & b & c
        o5 = b | c
        o4 = a & b
        o3 = d
        o2 = c
        o1 = b
        o0 = a
        outs = (o7 << 7) | (o6 << 6) | (o5 << 5) | (
            o4 << 4) | (o3 << 3) | (o2 << 2) | (o1 << 1) | o0

        out.write(bytes(outs.to_bytes(1, byteorder="big")))
    out.close()


bigTest()
smallTest()
