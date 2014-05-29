var QR = require('../');
var util = require('util');

var message = process.argv[2];
var scale = parseInt(process.argv[3]) || 1;
var color = process.argv[4] || '#000';

var buf = new Buffer(message);
var res = QR.encode(buf, QR.ECLEVEL_L);
var width = res.width;
var data = res.data;

var lines = [];
for (var i = 0, offset = 0; i < width; i++) {
  var line = lines[i] = [];
  for (var j = 0; j < width; j++, offset++) {
    var px = data.readUInt8(offset);
    if (px & QR.DOT_BLACK) {
      var last = line[line.length - 1];
      if (last && last[1] == j) {
        last[1]++;
      } else {
        line.push([j, j + 1]);
      }
    }
  }
}

var d = [];

for (var i = 0; i < lines.length; i++) {
  var line = lines[i];
  for (var j = 0; j < line.length; j++) {
    d.push(util.format('M %d %d h %d',
      (1 + line[j][0]) * scale,
      (1 + i + 0.5) * scale,
      (line[j][1] - line[j][0]) * scale
    ));
  }
}

console.log('<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="%d" ' +
    'stroke="%s" stroke-width="%d" fill="none"><path d="%s"/></svg>',
    (width + 2) * scale,
    (width + 2) * scale,
    color,
    scale,
    d.join(' ')
);
