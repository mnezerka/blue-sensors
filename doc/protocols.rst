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

The only mandatory fields on global level are ``device`` and
``readings``. Each item in ``readings`` array have to contain
``address``.

The minimal http chunk could look like::

    {
        "device": "Device123",
        "readings": [
            {
                "address": "SensorXYZ",
                "t": 23,
            }
        ]
    }

Encryption
----------

Server accepts both unecrypted and encrypted data. The only supported
algorithm is AES 128bit ECB with PKCS7 padding. Free implementation
in C is available here: https://github.com/mnezerka/blue-aes
