#!/usr/bin/perl

use IO::Socket::INET;
use threads;

#########################################################
#							#
#		Variable declarations			#
#							#
#########################################################

my	$port_sv = 40000;
my	$addr_sv = "localhost";
my	$socket;
my	$request;
my	$reponse;
my	$nbr_req = 300;
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
    print $socket $request;
}

sub	listen_request()
{
    my $reponse = <$socket>;
    print "$reponse\n";
}

#########################################################
#							#
#		Request functions			#
#							#
#########################################################

sub	create_request_bad()
{
    $request = "test !!\r\n";
    $request = $request."\r\n";
}

#########################################################
#							#
#		     Create Threads			#
#							#
#########################################################

sub	makeall()
{
    serv_connection();
    create_request_bad();
    send_request();
    close($socket);
}

sub	create_thread()
{
    $thr = threads->new(\&makeall);
    $thr->join;
}

#########################################################
#							#
#		     Program start			#
#							#
#########################################################

while ($i < $nbr_req)
{
    create_thread();
    $i++;
}
exit(0);
