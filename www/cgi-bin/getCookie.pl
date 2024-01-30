#!/usr/bin/perl

use strict;
use warnings;

my $cookie = $ENV{'HTTP_COOKIE'};

print <<HTML;
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

if ($cookie) {
    print "        Cookies avalaible:</em> $cookie</h4></div>";
} else {
    print "        The cookie is</em> not defined.</h4></div>";
}

print <<HTML;
    </body>
</html>
HTML
