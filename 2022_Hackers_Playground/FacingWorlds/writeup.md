Facing Worlds Writeup
===

##### Exploitation:

Given a sample of modified audio track created in Sonic-pi (original track is Foregone Destruction by Michiel van den Bos ) participants have to detect irregular left-right channel pattern of the base rhythm part.

Subtract waves in Audacity to cancel out other instruments in the track. Divide into similar time-intervals, put "1" when note is star playing, "0" otherwise. Group into bytes, decode as ASCII string to reveal the flag.
