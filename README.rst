tuitwall
========

tuitwall is a project that will allow you to show tweets in a led matrix.
It has a web interface build with PHP that allows people to log in with their twitter account to display their tweets.

Requisites
----------

**Web**

- HTTP Server (e.g. Apache)
- PHP 5 (will probably work with v4 too)

**Electronics**

- `Arduino board`_
- `Arduino Ethernet Shield`_
- `Ethernet cable`_
- `Sure Electronic's 32x08 Red Led 5mm Dot Matrix`_
- HT1632-for-Arduino_ library
- `Arduino IDE`_ v1.0 or newer

Note: this has been tested only with one led matrix, but it can be extended up to 4 boards with proper code.

.. _`Arduino board`: http://arduino.cc/en/Main/ArduinoBoardUno
.. _`Arduino Ethernet Shield`: http://www.arduino.cc/en/Main/ArduinoEthernetShield
.. _`Ethernet cable`: http://en.wikipedia.org/wiki/8P8C_modular_connector#8P8C
.. _`Sure Electronic's 32x08 Red Led 5mm Dot Matrix`: http://www.sureelectronics.net/goods.php?id=1121
.. _HT1632-for-Arduino: https://github.com/gauravmm/HT1632-for-Arduino
.. _`Arduino IDE`: http://arduino.cc/en/Main/Software

Configuration
-------------

**config.php**

- SERVER: root direction of the web interface (excluding ``http://``)
- CONSUMER_KEY & CONSUMER_SECRET: consumer application keys, get them from https://dev.twitter.com/apps
- ANYWHERE_CONSUMER_KEY: key to make @Anywhere available at the page to auto-linkify @users, #hastags and http://links
- OAUTH_CALLBACK: location of the PHP script that will receive the OAuth Callback. It should point to ``callback.php``
- API_KEY: only devices with the same ``API_KEY`` will be able to get tweets through fetch.php. Leave empty (``""``) to disable the check
- SHOW_TWEET: show current tweet at main page?
- SHOW_USER: include tweet owner when giving a tweet to a device through `fetch.php`?
- STREAM: show a stream from UStream at the main page? Useful when doing a demo of the project
- USTREAM_ID: ID of the stream from UStream you want to show

**Arduino**

- API_KEY: same as ``config.php``
- SERVER: same as ``config.php``
- GET_REQUEST: shouldn't be modified
- INTERVAL: milliseconds to wait at least between tweet requests
- VEC_LENGTH: length of the array where the tweet will be stored
- SPEED: milliseconds to wait between

The reason there are so many options to limit access to tweets is because Twitter only allows clients to make a `limited number of calls in a given hour <https://dev.twitter.com/docs/rate-limiting>`_.

Usage
-----

1. Upload the contents of the ``web`` folder to a web server
2. Adapt the configuration to your needs

   a) Change the ``SERVER`` constant (both in the Arduino sketch and ``config.php``) to the appropiate one
   b) Change the ``API_KEY`` to a different one. Remember to use the same in the Arduino sketch and ``config.php``
   c) Create a `Twitter application`_ and point the *Callback URL* to the location of ``callback.php``
   d) Copy the *Consumer key* and *Consumer secret* keys to ``config.php``
   e) If you want the @Anywhere auto-linkify functions create another application in Twitter and copy the *Consumer key*. Callback URL just has to match up to the subdomain.
3. Mount the Arduino and Ethernet Shield and connect to the PC
4. Upload ``tuitwall.ino`` to the Arduino
5. Connect the led matrix board to the Arduino (schematic comming soon!)
6. Check that the led matrix board has its direction set to 1 (switches at the back)
7. Connect the Ethernet Shield to a router through an ethernet cable
8. Reset the board
9. Enjoy!

Remember that the sketch outputs debug messages to the serial port, so you can follow the initialization and execution of the sketch.
The Ethernet Shield relies on a DHCP enabled network, if this is not the case, please modify the code as required to give an appropiate static IP.

.. _Twitter application: https://dev.twitter.com/apps