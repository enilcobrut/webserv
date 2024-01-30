#!/usr/bin/perl

use strict;
use warnings;
#Y2gwYzBsNHQzX2NoMXBfY28wa2kz
#NF9jaDBjMGw0dDNfY2gxcF9jbzBraTM=
#NG5fMGF0bTM0bF9yYTE1MW5fY28wa2kz
#NF9wMzRudXRfYnV0dDNyX2NvMGtpMw==
#NF81dWc0cl9jbzBraTM=
#NF9tNHI1aG00bGwwd19zbTByM19jbzBraTM=
#NF9tYWM0ZGFtMTRfbnV0X2NvMGtpMw==
#NF9kMHVibDNfY2gwYzBsNHQzX2NvMGtpMw==
#NF93aDF0M19jaDBjMGw0dDNfbWFjNGRhbTE0X251dF9jbzBraTM=
#NG5fNGxtMG5kX2NvMGtpMw==
#NF9nMW5nM3JicmVhZF9jbzBraTM=
#NF81aDBydGJyMzRkX2NvMGtpMw==
#NF9jMGMwbnV0X21hYzRyMDBuX2NvMGtpMw==
#NF9tJm0nc19jbzBraTM=
#NF9yM2RfdjNsdjN0X2NvMGtpMw==
#NF9sM20wbl9jbzBraTM=
#NF81NGx0M2RfYzRyNG0zbF9jbzBraTM=

my @phrases = (
    "Y2gwYzBsNHQzX2NoMXBfY28wa2kz",
    "NF9jaDBjMGw0dDNfY2gxcF9jbzBraTM=",
    "NG5fMGF0bTM0bF9yYTE1MW5fY28wa2kz",
    "NF9wMzRudXRfYnV0dDNyX2NvMGtpMw==",
    "NF81dWc0cl9jbzBraTM=",
    "NF9tNHI1aG00bGwwd19zbTByM19jbzBraTM=",
    "NF9tYWM0ZGFtMTRfbnV0X2NvMGtpMw==",
    "NF9kMHVibDNfY2gwYzBsNHQzX2NvMGtpMw==",
    "NF93aDF0M19jaDBjMGw0dDNfbWFjNGRhbTE0X251dF9jbzBraTM=",
    "NG5fNGxtMG5kX2NvMGtpMw==",
    "NF9nMW5nM3JicmVhZF9jbzBraTM=",
    "NF81aDBydGJyMzRkX2NvMGtpMw==",
    "NF9jMGMwbnV0X21hYzRyMDBuX2NvMGtpMw==",
    "NF9tJm0nc19jbzBraTM=",
    "NF9yM2RfdjNsdjN0X2NvMGtpMw==",
    "NF9sM20wbl9jbzBraTM=",
    "NF81NGx0M2RfYzRyNG0zbF9jbzBraTM="
);

my $random_index = int(rand(scalar(@phrases)));
my $random_phrase = $phrases[$random_index];

print <<HTML;
<!--SetCookie-->
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
        <title>Welcome on Webserv</title>
        <link rel="icon" type="image/png" href="/favicon.ico">
        <link rel="stylesheet" href="/css/templatemo-style.css">
        <link href="https://fonts.googleapis.com/css?family=Open+Sans:300,400,600,700,800" rel="stylesheet">
    </head>
    <body>
        <div class=\"section-heading\">
        <h4><em>
HTML

print "        The cookie has been set: </em>$random_phrase</div>";

print <<HTML;
    </body>
</html>
HTML
