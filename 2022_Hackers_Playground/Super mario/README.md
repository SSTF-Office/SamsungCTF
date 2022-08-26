# DESCRIPTION 
2022 SSTF super mario challenge

# INFO
- It is dirty pipe CVE challenge. so you have to set environment kernel (Linux 450a218e7d9b 5.13.0-19-generic #19-Ubuntu SMP Thu Oct 7 21:58:00 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux)
- dirty pipe can modify any binary even if it is readonly permission.
- stage-1: you can get a shell modify info.sh 
- stage-2: you can get root privilege escalation with dirtypipe poC. 

# INDEX
- SRC: challenge docker. 
- EXPLOIT: sample POC code.
- proxy: connection receiver. 
