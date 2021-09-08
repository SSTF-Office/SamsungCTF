#!/usr/bin/perl
use Socket;
$port = 7777;
@exec = ("su", "noleak", "-c", "/home/noleak/noleak");
socket(SERVER, PF_INET, SOCK_STREAM, 6);
setsockopt(SERVER, SOL_SOCKET, SO_REUSEADDR, pack("l", 1));
bind(SERVER, sockaddr_in($port, INADDR_ANY));
listen(SERVER,SOMAXCONN);
$SIG{"CHLD"} = "IGNORE";
while($addr = accept CLIENT, SERVER){
    $| = 1;
    ($port, $packed_ip) = sockaddr_in($addr); 
    $datestring = localtime();
    $ip = inet_ntoa($packed_ip);
    print "$ip: $port connected($datestring)\n";
    fork || do {
        $| = 1;
        close SERVER;
        open STDIN,  "<&CLIENT";
        open STDOUT, ">&CLIENT";
        open STDERR, ">&CLIENT";
        close CLIENT;
        exec @exec;
        exit 0;
    };
    close CLIENT;
}
close SERVER;
