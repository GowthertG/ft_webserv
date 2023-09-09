# we will write an infinite loop to test the server

import time

while True:
    print("Content-type: text/html\n\n")
    print("<html><head><title>Python Server Test</title></head>")
    print("<body><h1>Python Server Test</h1>")
    print("<p>This is a test of the Python server.</p>")
    print("<p>The time is: %s</p>" % time.ctime())
    print("</body></html>")
    time.sleep(1)