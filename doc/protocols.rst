Communication Protocols
=======================

Data are sent over HTTP in POST request. Server replies with HTTP
status codes, where 200 OK means that readings are accepted.

Json format
-----------

Format of the raw http body::

    {
        "device": "DEVICE_ID",
        "ip": "device ip address (optional)",
        "wifi-ssid": "device wifi ssid (optional)",
        "wifi-strength": deivice wifi strength (optional),
        "time: unix_timestamp (optional),
        "readings": [
            {
                "address": "unique sensor address",
                "t": temperature,
                "h": humidity;
                "p": pressure;
            }
        ]
    }

The only mandatory fields on global level are ``device`` (short variant is
``d``) and ``readings`` (short variant is ``r``. Each item in ``readings``
array have to contain ``address`` (short variant is ``a``).

The minimal http chunk could look like::

    {
        "d": "Device123",
        "r": [
            {
                "a": "SensorXYZ",
                "t": 23,
            }
        ]
    }

Encryption
----------

Server accepts both unecrypted and encrypted data. The only supported
algorithm is AES 128bit ECB with PKCS7 padding. Free implementation
in C is available here: https://github.com/mnezerka/blue-aes

Testing
-------

This is simulation of device push executed in command line::

    curl -i -X POST http://your_server -d '{"d":"RED0002","r":[{"a":"28ff9180c1160434","t":22.6},{"a":"RED0002BME","t":22.6,"p":976.6,"h":57.0}]}'



