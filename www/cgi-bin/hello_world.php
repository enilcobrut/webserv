<!DOCTYPE html>
<html>
<head>
    <title>Hello World Animation</title>
    <style>
        @keyframes rainbow {
            0% { color: red; }
            14% { color: orange; }
            28% { color: yellow; }
            42% { color: green; }
            57% { color: blue; }
            71% { color: indigo; }
            85% { color: violet; }
            100% { color: red; }
        }
    </style>
</head>
<body>
    <?php
    echo '<h1 id="hello">Hello World</h1>';
    ?>
    <script>
        var hello = document.getElementById("hello");
        hello.style.animation = "rainbow 5s infinite";
    </script>
</body>
</html>
