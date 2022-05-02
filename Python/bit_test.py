bitset_size = 1 << 5            # 32 bits
def bitstring (uInt):
    """ returns the bit-string corresponding to an unsigned integer """
    
    # format the binary string with
    # width = bitset-size, right-aligned, fill left with 0
    bitstr_ = '{:0>{width}b}'.format(uInt, width=bitset_size)
    return bitstr_

def print_bytes(bitstr):
    BYTESIZE = 8
    SIZE = len(bitstr)
    for pos in range(0, SIZE, BYTESIZE):
        endpos = pos + BYTESIZE
        print( "{}:".format(bitstr[pos:endpos], bitstr[endpos:]), end='')
    #END for
    print()

#------------------------------------------------------------------------------80

# some 32-bit integer
n_int32 = (1 << 32 ) - 4363663
# remove the leftmost 16 bits by right-shift
n_int16 = n_int32 >> 16

print("int32: {} -> int16: {}".format(n_int32, n_int16))
print_bytes(bitstring(n_int32))
print_bytes(bitstring(n_int16))

