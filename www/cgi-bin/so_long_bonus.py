import sys
import os
import subprocess

print("""
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
        <div class="section-heading">
        <h4><em>
""")

if len(sys.argv) != 1:
    print("Usage: python script.py")
    sys.exit(1)

if not os.path.exists("./so_long_bonus") or not os.access("./so_long_bonus", os.X_OK):
    print("The './so_long_bonus' executable does not exist or is not executable.")
    sys.exit(1)

process = subprocess.Popen(
    ["./so_long_bonus", "with_motobug.ber"],
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE
)

print("Sonic has been successfully executed.</em> You can back home</h4></div></body></html>")
