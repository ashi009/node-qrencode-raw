# qrencode-raw [![NPM version](https://badge.fury.io/js/qrencode-raw.png)](http://badge.fury.io/js/qrencode-raw) [![Dependencies](https://david-dm.org/ashi009/qrencode-raw.png)](https://david-dm.org/ashi009/qrencode-raw)

A node.js wrapper for libqrencode, but instead of encoding data to image, it
returns raw QR data in buffer, which allows you to do amazing things.

## Install

You may need to install libqrencode-dev (>= 3.2) before install this package.

    # Ubuntu
    sudo apt-get install libqrencode-dev

    # OS X
    brew install libqrencode

    npm install --save qrencode-raw

## Usage

    var QR = require('qrencode-raw');
    var res = QR.encode(data, level, version);

- **data** _string_ | _Buffer_ data to encode, cannot be empty
- **level** _QRecLevel_ error correction level:
  - `QR.ECLEVEL_L` Lowest
  - `QR.ECLEVEL_M`
  - `QR.ECLEVEL_Q`
  - `QR.ECLEVEL_H` Highest
- **version** _int_ QR code version
  - `0` auto
  - `1` ~ `40` manually set

### Result

    {
      version: 1,
      width: 21,
      data: <SlowBuffer ...>
    }

- **version** _int_ qr code version
- **width** _int_ qr code width/height
- **data** _Buffer_ qr code actual data, each byte represent a dot,
  width * height bytes in total.<br>
  You can use bitwise AND with flags on each byte to extract info for that dot:
  - `QR.DOT_BLACK` dot is black.
  - `QR.DOT_DATA_ECC` dot is part of data or ecc code
  - `QR.DOT_FORMAT` dot is format info
  - `QR.DOT_VERSION` dot is version info
  - `QR.DOT_TIMING` dot is part of timing pattern
  - `QR.DOT_ALIGN` dot is part of alignment pattern
  - `QR.DOT_FINDER` dot is part of finder pattern
  - `QR.DOT_NON_DATA` dot is non-data
