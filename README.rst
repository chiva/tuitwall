tuitwall
========

tuitwall is a project that will allow you to show tweets in a led matrix.

The backend is built with PHP and uses the ``twitteroauth`` library to manage all the authentication system with Twitter. It also serves as the configuration page, as the user can choose which tweets and from who, are shown.
The Arduino, using the ethernet shield, calls the backend that provides it with the last tweet valid, then shows it on the led matrix with a scroll effect.

Requisites
----------

**Web**

- HTTP Server (e.g. Apache)
- PHP 5

**Electronics**

- `Arduino board`_
- `Arduino Ethernet Shield`_
- `Ethernet cable`_
- `Sure Electronic's 32x08 Red Led 5mm Dot Matrix`_
- HT1632-for-Arduino_ library
- `Arduino IDE`_ v1.0 or newer

Note: this has been tested only with one led matrix, but it can be extended up to 4 boards (and further) with proper code.

.. _`Arduino board`: http://arduino.cc/en/Main/ArduinoBoardUno
.. _`Arduino Ethernet Shield`: http://www.arduino.cc/en/Main/ArduinoEthernetShield
.. _`Ethernet cable`: http://en.wikipedia.org/wiki/8P8C_modular_connector#8P8C
.. _`Sure Electronic's 32x08 Red Led 5mm Dot Matrix`: http://www.sureelectronics.net/goods.php?id=1121
.. _HT1632-for-Arduino: https://github.com/gauravmm/HT1632-for-Arduino
.. _`Arduino IDE`: http://arduino.cc/en/Main/Software

Configuration
-------------

**config.php**

- SERVER: server domain (excluding ``http://`` and folders). It is retrieved by default from PHP variable, if wrong, set it manually.
- CONSUMER_KEY & CONSUMER_SECRET: consumer application keys, get them from https://dev.twitter.com/apps.
- ANYWHERE_CONSUMER_KEY: key to make @Anywhere available at the page to auto-linkify @users, #hastags and http://links.
- OAUTH_CALLBACK: location of the PHP script that will receive the OAuth Callback. It should point to ``callback.php``.
- API_KEY: only devices with the same ``API_KEY`` will be able to get tweets through fetch.php. Leave empty (``""``) to disable the check.
- SHOW_TWEET: show current tweet at main page?
- SHOW_USER: include tweet owner when giving a tweet to a device through ``fetch.php``?
- STREAM: show a stream from twitch.tv at the main page? Useful when doing a demo of the project.
- TWITCH_ID: ID of the stream from twitch.tv you want to show.
- DISALLOW_LOGOUT: disallow logging out to keep tweets from a specific user being shown. To log out you have to delete ``auth.txt`` from the web server.

**Arduino**

- API_KEY: same as in ``config.php``.
- SERVER: same as in ``config.php``.
- GET_REQUEST: shouldn't be modified.
- INTERVAL: milliseconds to wait at least between tweet requests.
- VEC_LENGTH: length of the array where the tweet will be stored.
- SPEED: milliseconds to wait at least between led matrix refreshes.
- TIMEOUT: milliseconds in which the tweet should be received.

The reason there are so many options to limit access to tweets is because Twitter only allows clients to make a `limited number of requests in a given hour <https://dev.twitter.com/docs/rate-limiting>`_.

Usage
-----

1. Upload the contents of the ``web`` folder to a web server.
2. Adapt the configuration to your needs.

   a) Change the ``SERVER`` constant (both in the Arduino sketch and ``config.php``) to the appropiate one.
   b) Change the ``API_KEY`` to a different one. Remember to use the same in the Arduino sketch and ``config.php``.
   c) Create a `Twitter application`_ and point the *Callback URL* to the location of ``callback.php``.
   d) Copy the *Consumer key* and *Consumer secret* keys to ``config.php``.
   e) If you want the @Anywhere auto-linkify functions create another application in Twitter and copy the *Consumer key*. Callback URL just has to match up to the subdomain.
3. Mount the Arduino and Ethernet Shield and connect to the PC.
4. Upload ``tuitwall.ino`` to the Arduino.
5. Connect the led matrix board to the Arduino (continue reading for schematic).
6. Check that the led matrix board has its direction set to 1 (switches at the back).
7. Connect the Ethernet Shield to a router through an ethernet cable.
8. Reset the board.
9. Enjoy!

Remember that the sketch outputs debug messages to the serial port (at 115200bps), so you can follow the initialization and execution of the sketch.
The Ethernet Shield relies on a DHCP enabled network, if this is not the case, please modify the code as required to give an appropiate static IP.

.. _Twitter application: https://dev.twitter.com/apps

Schematic
---------

How to connect Arduino to Sure Electronic's led matrix using the provided IDC cable:

.. image:: https://github.com/chiva/tuitwall/raw/master/images/tuitwall-schematic.png

The hardware part of the project should look similar to this:

.. image:: https://github.com/chiva/tuitwall/raw/master/images/tuitwall-hardware.jpg

Other uses
----------

The Arduino part is mostly universal, you can change the php page it connects to to another one that returns whatever you want to appear on the led matrix. It could be the weather report, current playing song, rss feed, etc. Be creative!

Remember, what the php page returns should be plain text terminated by ``chr(0)``. No html, just the text you want to appear.