# Single Page Web Server

Sounds useless, doesn't it? This is a C implementation of a very simple HTTP/1.0 web server. Just for the fun of it.

On a side note, it's not finished yet. See TODO chapter.

## Features

* Configuration is done by command-line parameters.
* Listens to a socket on a given port. Both IPv4 and IPv6 are supported.
* Only the method of the incoming HTTP request is parsed. Only GET is supported.
* Returns only a single HTML page as a response. Maximum file size is 65536 characters.

## Requirements

* Linux
* GCC

## Potential use cases

Some potential use cases:

* It's very fast to setup.
* It doesn't require any extra libraries.
* It doesn't require Ruby or Python.
* Could work as a sorry-server for a load-balancer. Remember, HTTP/1.0 could not care less about Host headers. When multiple sites are pointed to this web servers, all HTTP responses are the same, no matter what is the Host header.

## Building the application

Source code is located in the `src` directory. Run `make` to build.

```
$ make
gcc -lpthread -c -o obj/util.o src/util.c
gcc -lpthread -c -o obj/server.o src/server.c
gcc -lpthread -c -o obj/http.o src/http.c
gcc -lpthread -o bin/single-page-web-server obj/util.o obj/server.o obj/http.o
```

This creates two directories: `obj` for objects and `bin` for the resulting binary file.

If you want to clean the object files, run `make clean`.

```
$ make clean
rm -f obj/util.o obj/server.o obj/sockets.o obj/http.o
```

## Running the application

Print out the help message.

```
$ bin/single-page-web-server -h
ingle Page Web Server
Options:
   -p <port>: TCP port number, which will be listened to. Defaults to port 80.
   -f <path>: Path to file, which will be served. Required.
   -6:        Use only IPv6 sockets. By default both IPv4 and IPv6 sockets are used.
   -d:        Enable debug logging.
   -h:        Display help.
```

An example run:

```
$ bin/single-page-web-server -p 8080 -f index.html
```

## TODO

* An option run this as a daemon instead of as a foreground process.
* Instead of spawning a new thread when a HTTP request arrives, a thread pool of some sort could be used.
* It's probably not compliant with the HTTP/1.0 specification, even though it only accepts GET requests. I should read the RFC once again.
* Bug hunt. I'm pretty sure the protocol handling isn't perfect.
* Timeouts should be used. If a client opens up a TCP connection but doesn't send any requests, we shouldn't waste our threads on that.

## Development ideas

* The server could serve any kind of file instead of a HTML page.

* SSL support. Again, with the same simplicity. Perhaps the web server could generate a SSL key and a bogus certificate on start-up, if an `-ssl` command-line parameter is given. A new key and a new certificate would be generated when the application is restarted. Just a thought.
