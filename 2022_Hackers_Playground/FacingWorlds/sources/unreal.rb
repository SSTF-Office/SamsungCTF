# -----------------------------------------------------------------------------
flag = "SCTF{Unr3aL_2_g0_b@ck_iN_Tim3}"

# -----------------------------------------------------------------------------
use_bpm 160
use_synth_defaults

d = 0.25
define :delay do |n|
  sleep n*d
end

# -----------------------------------------------------------------------------
bits = [0,0,0,0,0,0,0,0]
live_loop :pi1_loop do
  sync :piano1
  use_synth :sine
  
  nots = [:F5, :C5, :G4, :Gs4, :C5, :Gs4, :G4, :Gs4]
  for i in 0..7
    play nots[i], amp: 0.5, release: 1, pan: bits[i]
    delay 2
  end
end

# -----------------------------------------------------------------------------
define :plpi do |n,t|
  play n, sustain: 0.8, release: 3, amp: 0.3
  delay t
end

live_loop :pi2_loop do
  sync :piano2
  use_synth :dtri
  
  plpi [:F6,  :C6], 4
  plpi [:F6,  :C6], 2
  plpi [:As6, :F6], 4
  plpi [:As6, :F6], 2
  plpi [:C7, :G6], 4
  plpi [:C7, :G6], 2
end

# -----------------------------------------------------------------------------
bas = [
  [[:F2, :F3], 6], [[:F2, :F3], 4], [[:F2, :F3], 6],
  [[:F2, :F3], 6], [[:F2, :F3], 4], [[:F2, :F3], 6],
  [[:F2, :F3], 6], [[:F2, :F3], 4], [[:F2, :F3], 6],
  [[:F2, :F3], 6], [[:F2, :F3], 4], [[:F2, :F3], 6],
  [:F2, 6], [:C2, 4], [:F2, 6],
  [:Gs2, 16],
  [:F2, 10], [:F3, 2], [:C3, 2], [:Cs2, 2],
  [:F2, 16],
  [:F2, 10], [:F2, 6],
  [:Gs2, 16],
  [:F2, 10],
  [:F2, 2], [:Gs2, 2], [:C3, 2],
  [:F2, 16]
]

live_loop :bass_loop do
  sync :bass
  use_synth :dpulse
  
  for i in 0..bas.length-1
    play bas[i][0], amp: 0.5, attack: 0.1, release: 1
    delay bas[i][1]
  end
end

# -----------------------------------------------------------------------------
define :drum_C3 do
  sample :drum_heavy_kick, rate: 0.7, amp: 1.5
end

define :drum_D3 do
  sample :drum_snare_hard, amp: 0.8, rate: 0.8
end

define :drum_E3 do
  sample :drum_snare_soft, amp: 2
end

define :drum_B2 do
  sample :drum_heavy_kick, rate: 1, amp: 1.5
end

define :drum_F3 do
  sample :drum_bass_hard, rate: 0.8, release: 1, amp: 1.5
end

C3 = 0b01000000
D3 = 0b00010000
E3 = 0b00000100
B2 = 0b10000000
F3 = 0b00000010

drums = [C3, 0, C3, 0, D3+E3, 0, C3, 0,  C3,  0, D3+E3,  0,    C3, D3, C3, D3,
         C3, 0, C3, 0, D3+E3, 0, C3, D3,  0, D3,    C3, C3, D3+E3,  0, C3, D3,
         C3, 0, C3, 0, D3+E3, 0, C3, 0,  C3,  0, D3+E3,  0,     0, D3, C3, D3,
         C3, 0, C3, 0, D3,    0, C3, D3, C3, C3,     0,  0,     0,  0,  0,  0,
         B2+C3+F3, 0, C3, 0, B2+C3+D3+E3+F3, 0, C3, 0,  B2+C3+F3,  0, D3+E3,  0,       B2+C3+F3, D3, C3, D3,
         B2+C3+F3, 0, C3, 0, B2+C3+D3+E3+F3, 0, C3, D3, B2+C3+F3, D3,    C3, C3, B2+C3+D3+E3+F3,  0, C3, D3,
         B2+C3+F3, 0, C3, 0, B2+C3+D3+E3+F3, 0, C3, 0,  B2+C3+F3,  0, D3+E3,  0,       B2+C3+F3, D3,  0, D3,
         B2+C3+F3, 0, C3, 0, B2+C3+D3+E3+F3, 0, C3, D3, B2+C3+F3, C3,     0,  0,       B2+C3+F3,  0,  0,  0
         ].ring

live_loop :drum_loop do
  sync :drum
  
  for i in 0..drums.length-1
    m = drums[i]
    if true
      if (m & C3) != 0
        drum_C3
      end
      
      if (m & D3) != 0
        drum_D3
      end
      
      if (m & E3) != 0
        drum_E3
      end
      
      if (m & B2) != 0
        drum_B2
      end
    end
    if (m & F3) != 0
      drum_F3
    end
    
    delay 1
  end
end

# -----------------------------------------------------------------------------
define :cymbCs4 do
  sample :drum_splash_hard, amp: 0.5
end

define :cymbB4 do
  sample :drum_cymbal_open, amp: 0.2, rate: 1
end

define :cymbDs4 do
  sample :drum_snare_hard, amp: 0.3, rate: 1.2
end

live_loop :splash2_loop do
  sync :splash2
  4.times do
    delay 2
    
    cymbB4
    delay 4
    
    cymbB4
    delay 8
    
    cymbB4
    delay 2
  end
end

# -----------------------------------------------------------------------------
live_loop :splash1_loop do
  sync :splash1
  6.times do
    cymbDs4
    delay 2
  end
  
  5.times do
    cymbDs4
    delay 1
  end
  
  delay 1
  
  7.times do
    cymbDs4
    delay 2
  end
end

# -----------------------------------------------------------------------------
fidx = 0
exit = false

live_loop :main do
  cnt = 0
  if exit
    print "The End! ----------------------------------------"
    break
  end
  
  (16*16).times do
    
    if [0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240].include? cnt
      # ==============================================================
      if fidx < flag.length
        c = flag[fidx]
        o = c.ord
        for i in 0..7
          b = (o & 1)
          if b != 0
            bits[i] = choose([-1, 1])
          else
            bits[i] = 0
          end
          o = o >> 1
        end
        print fidx, c, bits
        fidx = fidx + 1
      else
        if !exit
          print "--------------------------------------------------"
          print "flag encoded, go exit ..."
          print "--------------------------------------------------"
          bits = [0,0,0,0,0,0,0,0]
          exit = true
        end
      end
      # ==============================================================
      cue :piano1
    end
    
    if [0, 64, 128, 192].include? cnt
      cue :piano2
    end
    
    if cnt == (4*16)
      cue :bass
    end
    
    if cnt == (8*16)
      cue :drum
    end
    
    if [11*16 + 10, 15*16 + 10].include? cnt
      cymbCs4
    end
    
    if cnt == (12*16)
      cue :splash2
    end
    
    if [64, 96, 128, 160, 192, 224].include? cnt
      cue :splash1
    end
    
    cnt = cnt + 1
    sleep 1*d
  end
end


