# What does the drone say?

## Description

```
Enemy's drones have been spotted again! Now, we need to know the drone's
destination by analyzing the remote controller signal. We successfully hijacked
one of them, and analyzed it. From the analysis, we found below information:
- Modulation: FHSS-FSK
- Frequency Band: 5.8 GHz
- Minimum Carrier Frequency: 5.8025 GHz
- Number of Hopping Channels: 20
- Bandwidth of Each Channel: 5 MHz

After getting this information we captured the live signal of the remote
controller. We downconverted the 5.8 GHz band signal to the baseband using
200 MHz sampling rate. Would you please decode the signal?

[Signal.gnuradio]()

```

## How to solve

All the necessary information is already given in the description. We only need
to load the data, demodulate the signal, and detect the hopping sequence. Before
start, you need to first look at
(FSK)[https://en.wikipedia.org/wiki/Frequency-shift_keying] and
(FHSS)[https://en.wikipedia.org/wiki/Frequency-hopping_spread_spectrum] .

By monitoring the signal, you can easily identify that it is binary-FSK which
only has two frequencies deviated from the carrier frequency. Therefore, you can
convert the frequency sequence to '1' or '0' to get the data.

Additionally, you need to identify the hopping sequence. Because it is given
that the number of hopping channels are 20, you can identify the hopping
sequence by monitoring the frequencies. Since FSK only deviates the carrier
frequency smaller than the channel bandwidth, you can easily identify the
hopping channels.


## Description
flag: SCTF{9cc184d92ce0adfb805875ed75ff7f10}
