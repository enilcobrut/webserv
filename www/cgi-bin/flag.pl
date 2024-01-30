#!/usr/bin/perl

use strict;
use warnings;
use MIME::Base64;

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
        <div class="section-heading">
            <h4><em>
HTML

print <<HTML;
        <form class="accent-form" method="get">
            <label for="flag">Enter your flag {AsciiCharacters} : </label>
            <input type="text" id="flag" name="flag"><br>
            <button class="accent-button button">Check your flag</button>
        </form>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/jsSHA/2.0.2/sha_dev.js"></script>
        <script type="module">

        const button = document.querySelector('button');
        console.log(button);
        button.addEventListener('click', e => {
                console.log(e);
                e.preventDefault();
                checkFlag();
        });
            

            function checkFlag() {
        var flagValue = document.getElementById('flag').value;

        // hashing user input
        var shaObj = new jsSHA("SHA-256", "TEXT");
        shaObj.update(flagValue);
        var encodedFlag = shaObj.getHash("HEX");
        console.log(encodedFlag);

        // replace these with the SHA-256 hashes of the correct flags
        var answers = [
            "3563c2fa21d9af69c4b48d52404bbea06b194d9e54b63f716ee99cc2895be0eb",
            "932ed077dbda62cdd8833c74867af5445ecb3f08253a0c4cef1cc7e9538b3460",
            "6c840339af55dc0cbb45996aad91359861f5112c47973b4198ed9f9c53f7c502",
            "f9635cb4164d89efcfbe2a7c2693f5137bd3571c85e954ccd5ccfebf6741d910",
            "679db3e7a322da89faf9a36f4dc0ce8b919e3d8908c2848d62031464c98a68e1",
            "9ef28a8749c181a6bc8b1d3f3068939e8be983f43bb55252630a3d2ed5764f70",
            "9aaafc682da95e7906bce2762a2e7ab653a4e831b5ff57842bdb8d8353ffce40",
            "f1e82b412936c51ea9314a2765526e047f781e8b855fcf487a38ac84c6fa88dd",
            "81749c0c601a8a5d6e9e8776626a3c55ee9c822b932a3c4bae52a71d23417d10",
            "6dbb83e9d26b8a3bd5ba3c6828ba866fc151bfa402b3e67ebc54539e94f5ac94",
            "03a4d4552b649390fcbcdd197a06d83afa1af8a98cdb504ca6dc351896a36d37",
            "83a0649fd3c2f0856b73876299105d55019e4fef92c62ad6c454e8d18b553148",
            "7dbd03cc9e43e29f472d2cd326e623d85fd6eb73c3ddfe65e1c9e7789a56a085",
            "267e22e747f3a89d625ed0140d6d25d13dcb7cc385a7c11025fb250c60b7d2d8",
            "13b19aa30857296cc5ba9960260a46263d8cb23fbcc08f783f426a42b7973328",
            "22b0bda808388f99cd3824fe83fb33a375b53008bc4b6d9dc598a7ff8e19b294"
        ];
        var isCorrect = false;
        for (var i = 0; i < answers.length; i++) {
            if (encodedFlag === answers[i]) {
                isCorrect = true;
                break;
            }
        }

        if (isCorrect) {
            window.open("/congratulations.html");
        } else {
            alert("Sorry, wrong flag.");
        }
    }
        </script>
    </body>
</html>
HTML