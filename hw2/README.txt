CONTENTS OF THIS FILE
-------------------------------------------------------------------------------
* 1. Name and Project Information
* 2. Project Description
* 3. Other Comments
* 4. Known Problemes
* 5. Design

-------------------------------------------------------------------------------
SECTION 1 - NAME AND PROJECT DESCRIPTION
-------------------------------------------------------------------------------
Program Author: Mary Grace Glenn (mgglenn)
Program Name: CPSC 3600 Homework 2 - HTTP WebServer and Client
CUID: C10064065
Project Due: Sunday, November 8, 2015
Course: CPSC 3600 - 001
Instructor: Remy, Sekou

-------------------------------------------------------------------------------
SECTION 2 - PROJECT DESCRIPTION
-------------------------------------------------------------------------------
This project implements a simple web server and simple web client. The client
only sends HTTP GET requests and the server only replies successfully to GET
and HEAD requests.

The client - getfile
The server - webserver

Helper files - getfileHelper, webserverHelper, makefile

The client and server both allow the user to specify which port they would like
to use by using the -t flag. The client can specify what file she would like the 
output to be written to via the -f flag, otherwise it will print to stout. The 
server can specify what directory it would like to serve up files from, otherw-
ise the current directory will be assumed.

Sources used: tutorialspoint.com, stackoverflow.com
-------------------------------------------------------------------------------
SECTION 3 - OTHER COMMENTS
-------------------------------------------------------------------------------
It should be noted the getfile client only serves up the html information of a
given web page.

The 414 the server sends as an error (URI Too Long), did not allow for most
files to be served up during testing, even when they were in the home directory.
Is this problematic or how it is supposed to be?

The server returns the first error it encounters in a request that will make up
it's response. The default error is 400.

-------------------------------------------------------------------------------
SECTION 4 - KNOWN PROBLEMS
-------------------------------------------------------------------------------
* The server did not seem to be effective in terms of the absolute URI condition,
for example many files even in the local directory would throw this problem.

* One of the biggest problems associated with this project is that the client just 
writes the whole output to the file, in the future this should be implemented
to only write the file contents and not the HTTP response from the server aswell.

* For aesthetics, the helper functions could stand to be neater, as they throw
all the functions into two files. This is not a modular approach and would not 
be kind to reproducing code in the future, as it stands.

* The makefile could have other make versions that allow partial compilation, 
for example.The HTTPresponse struct is also messy.

* As it stands, this program is not robust in handling faulty or prolonged connections.
Often times it doesn't work over a certain port. A timer needs to be implemented to 
try to connect a certain number of times in a given period, perhaps, so as to not 
try multiple ports at one given time.

* There are some small inefficiences in mallocing strings that come from formatting
so many different strings, but getting the buffers and formats for the strings and 
in the reception and sending of content to be tighter was a difficult part of this 
project.

* There is only one HTTPresponse structure used in the program. It is zeroe'd out
with each new client, but I haven't tested extensively yet to make sure it isn't
carrying any information from residual clients in a given session.

* The server handles 403 exceptions theoretically but hasn't been tested with 
files on readability priveleges, only on directory mistakes.

* More testing needs to be done on the robustness of the server resolving
directories and requested pages.
-------------------------------------------------------------------------------
SECTION 5 - DESIGN
-------------------------------------------------------------------------------
The code was mainly designed for readability, with the main functions being
relatively short and being supported with most of the backstage functions handled
in the helper functions. 

The HTTPresponse struct, and making it universal, was the one thing that cut down
on code writing and made the design process more efficient (or at least simpler, 
ideally they are the same thing!). Tokenizing the HTTP request also was an
efficient way of parsing the different elements.

The client is fairly robust in parsing the page and host name from the user input.

I made the parsing of the user input for the server and client robust by allowing 
the user to input the flagged information and directories in any order they want,
which was accomplished just by iterating through the array of arguments.

Building the HTTPResponse before formulating the response itself also saved a lot of 
time in that I made sure all teh fields were addressed before having to check them
individually -- this allowed more robust testing to take place in my opinion.
