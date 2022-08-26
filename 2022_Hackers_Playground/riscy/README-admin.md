# Deploying

    $ cd ./deploy
    $ ./build.sh
    $ ./run.sh

    You might want to check if it's working:

    $ ./check.py
    [+] Starting local process './../src/exploit.py': pid 131865
    [+] Receiving all data: Done (1.17KB)
    [*] Process './../src/exploit.py' stopped with exit code 0 (pid 131865)
    [!] OK. (found: b'SCTF{Ropping RISCV is no difference!}')

    If you can see "OK. ..", you are good to launch!

# Release

  riscy.tar.gz