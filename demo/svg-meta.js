var qrencode = require('../');
var util = require('util');
var Color = require('color');

var message = process.argv[2];
var scale = parseInt(process.argv[3]) || 1;
var color = Color(process.argv[4] || '#000');

console.error(color);

var buf = new Buffer(message);
var res = qrencode.encodeBuffer(buf, 0, qrencode.ECLEVEL_L);
var width = res.width;
var data = res.data;

console.log('<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="%d" fill="%s">',
    (width + 2) * scale,
    (width + 2) * scale,
    color
  );

for (var i = 0, idx = 0; i < width; i++) {
  for (var j = 0; j < width; j++, idx++) {
    var px = data.readUInt8(idx);
    var fill = Color('#fff');
    var type = [];
    if (px & 1) {
      fill = fill.mix(color);
      type.push('black');
    }
    if (px & 2) {
      fill = fill.mix(Color('red'));
      type.push('data/ecc');
    }
    if (px & 4) {
      fill = fill.mix(Color('yellow'));
      type.push('format');
    }
    if (px & 8) {
      fill = fill.mix(Color('green'));
      type.push('ver');
    }
    if (px & 16) {
      fill = fill.mix(Color('blue'));
      type.push('timing');
    }
    if (px & 32) {
      fill = fill.mix(Color('purple'));
      type.push('alignment');
    }
    if (px & 64) {
      fill = fill.mix(Color('pink'));
      type.push('finder');
    }
    console.error(fill);
    console.log('<rect y="%d" x="%d" width="%d" height="%d" fill="%s" title="%s"/>',
        (i + 1) * scale,
        (j + 1) * scale,
        scale,
        scale,
        fill.hexString(),
        type.join(',')
      );
  }
}

console.log('</svg>');
