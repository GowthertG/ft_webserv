<?php
// Retrieve the cookie value
$message = '';
if (isset($_COOKIE["mycookie"])) {
    $message = "Cookie 'mycookie' is set.<br>";
} else {
    $message = "Cookie 'mycookie' is not set.";
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Increment Counter Example</title>
    <script>
    function setCookie(name, value, days) {
        var expires = "";
        if (days) {
            var date = new Date();
            date.setTime(date.getTime() + (days*24*60*60*1000));
            expires = "; expires=" + date.toUTCString();
        }
        document.cookie = name + "=" + (value || "")  + expires + "; path=/";
    }

    function setMyCookie() {
        setCookie('mycookie', 'cookie ___ value', 30);
        location.reload(); // Reload the page to update the PHP message
    }
    function delMyCookie() {
        setCookie('mycookie', '', -1);
        location.reload(); // Reload the page to update the PHP message
    }
    </script>
</head>
<body>
    <!-- print whether the cookie is set or not -->
    <p><?php echo $message; ?></p>

    <!-- button to set the cookie -->
    <button onclick="setMyCookie()">Set Cookie</button>
    <button onclick="delMyCookie()">Clear Cookie</button>
</body>
</html>
