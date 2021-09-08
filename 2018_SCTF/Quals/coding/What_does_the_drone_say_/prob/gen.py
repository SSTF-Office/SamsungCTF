import numpy as np
import bitarray
import struct


Fbase = 0
Fs = 200 * pow(10, 6)
Fbit = 10 * pow(10, 3)    # simulated bitrate of data
Fdev = 1 * pow(10, 5)     # frequency deviation, make higher than bitrate
A = 1                     # transmitted signal amplitude
Bw = 100 * pow(10, 5)     # bandwidth (bw)
NumCh = 20                # number of channels
BwPerCh = Bw // NumCh     # bandwidth per channel
Fch_min = Fbase + (BwPerCh // 2)
Fch_max = Bw - (BwPerCh // 2)
Ch_seqs = [8, 6, 4, 18, 2, 9, 0, 13, 7, 16,
           15, 5, 19, 10, 11, 12, 14, 3, 17, 1]


def to_bits(data):
    a = bitarray.bitarray()
    a.fromstring(data)
    return list(map(int,  a.tolist()))


def to_str(bitarr):
    a = bitarray.bitarray(bitarr)
    return a.tostring()


def modulate(data_in, ch_num):
    N = len(data_in)
    Fc = Fch_min + BwPerCh * (ch_num + 1)
    Fc = int(Fc)
    # sampling frequency must be higher than twice the carrier frequency

    t = np.arange(0, float(N)/float(Fbit), 1/float(Fs), dtype=np.float)
    m = np.zeros(0).astype(float)
    for bit in data_in:
        if bit == 1:
            m = np.hstack((m, np.multiply(np.ones(Fs/Fbit), Fc+Fdev)))
        else:
            m = np.hstack((m, np.multiply(np.ones(Fs/Fbit), Fc-Fdev)))
    y = A * np.sin(2*np.pi*np.multiply(m, t))
    return y


def generate_prob():
    flag = 'Welcome to SCTF 2018!!! If you see this message, ' \
        'you must have been successfully decode the signal.\n'\
        'You can get points by submitting: \n'\
        '   SCTF{md5.md5(\'\'.join(map(str, hopping_sequence))).hexdigest()}'

    print (len(flag))
    flag_bits = to_bits(flag)
    flag_len = len(flag_bits)

    data = np.zeros(0).astype(float)
    for idx, ch_num in enumerate(Ch_seqs):
        BitsPerCh = (flag_len // NumCh)
        flag_frag = flag_bits[BitsPerCh * idx:BitsPerCh * (idx+1)]
        data = np.hstack((data, modulate(flag_frag, ch_num)))

    with open('prob', 'w') as f:
        for d in data:
            f.write(struct.pack('<f', d))


if __name__ == '__main__':
    generate_prob()
