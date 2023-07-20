# ft_ping
my own ping

#Changes
- more simple code
- no norminette
- add flag 't'
- delete flag '6' : don't work IP6 in this network

## Use
- sudo ./ft_ping [options] <destination>

- add options:
	- -c <count>         stop after <count> replies
	- -D                 print timestamps
	- -h                 print help and exit
	- -i <interval>      seconds between sending each packet
	- -n                 no dns name resolution
	- -q                 quiet output
	- -v                 verbose output
	- -V                 print version and exit
	- -w <deadline>      reply wait <deadline> in seconds
	- -t <ttl>           define time to live

## References
- W.R. Stevens - UNIX Network Programming Volume 1

## ICMP-Header:
All ICMP packets have an 8-byte header and variable-sized data section. The first 4 bytes of the header have fixed format, while the last 4 bytes depend on the type/code of that ICMP packe

## Test
- blocks outgoing ICMP packets : sudo iptables -A OUTPUT -p icmp -j DROP
- allow sending ICMP packets   : sudo iptables -D OUTPUT -p icmp -j DROP
