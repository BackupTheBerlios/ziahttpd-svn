#!/usr/bin/perl

use IO::Socket::INET;

#########################################################
#							#
#		Variable declarations			#
#							#
#########################################################

my	$port_sv = 40000;
my	$addr_sv = "localhost";
my	$socket;
my	$request;
my	$nbr_req = 10;
my	$first = 0;
my	$i = 0;

#########################################################
#							#
#		Connection functions			#
#							#
#########################################################

sub	serv_connection()
{
    $socket = IO::Socket::INET->new(PeerAddr => $addr_sv,
				    PeerPort => $port_sv,
				    Proto => "tcp",
				    Type => SOCK_STREAM);
    if (!$socket)
    {
	if (!$first)
	{
	    print "Probleme de connection\n";
	    exit (1);
	}
	elsif ($first)
	{
	    print "Probleme de connection apres $i connections, l'une apres l'autre\n";
	    exit (-1);
	}
    }
    $first = 1;
}

sub	send_request()
{
    print $socket $request."\r\n";
}

#########################################################
#							#
#		Request functions			#
#							#
#########################################################

sub	create_request_post()
{
    $request = "POST /cgi-bin/script.cgi HTTP/1.0";
    $request = $request."\nContent-Type: application/x-www-form-urlencoded";
    $request = $request."\nContent-Length: 35";
}

sub	create_request_bad()
{
    $request = "j'envoie de la merde !!";
}

#########################################################
#							#
#		     Program start			#
#							#
#########################################################

while ($i < $nbr_req)
{
    serv_connection();
#    create_request_post();
    create_request_bad();
    send_request();
    close($socket);
    $i++;
}
exit(0);
