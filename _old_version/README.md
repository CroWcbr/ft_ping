# ft_ping
my own ping

## Mandatory part
- sudo ./ft_ping [options] <destination>
- only ping IPv4 with simple parsing
- manage the -v -h options

## Bonus part
- sudo ./ft_ping [options] <destination>
- add IPv6
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
	- -4                 use IPv4
	- -6                 use IPv6

## References
- W.R. Stevens - UNIX Network Programming Volume 1

## ICMP-Header:
All ICMP packets have an 8-byte header and variable-sized data section. The first 4 bytes of the header have fixed format, while the last 4 bytes depend on the type/code of that ICMP packe

## Docker (for check without root access)
- docker build -t docker_for_ping .
- docker run --user=root -it -v "$(pwd):/workspace" docker_for_ping
- connect using vs code
- exit
- docker rm $(docker ps -aq)
- docker rmi $(docker images -aq)

## Test
- blocks outgoing ICMP packets : sudo iptables -A OUTPUT -p icmp -j DROP
- allow sending ICMP packets   : sudo iptables -D OUTPUT -p icmp -j DROP
