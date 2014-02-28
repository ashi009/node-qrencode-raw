var qrencode = require('../');
var util = require('util');

var buf = new Buffer(process.argv[2]);
var res = qrencode.encodeBuffer(buf, 0, qrencode.ECLEVEL_L);
var width = res.width;
var data = res.data;

var lines = [];
for (var i = 0, offset = 0; i < width; i++) {
  var line = lines[i] = [];
  for (var j = 0; j < width; j++, offset++) {
    var px = data.readUInt8(offset);
    if (px & 1) {
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
    d.push(util.format('M %d %d h %d', line[j][0], i + .5, line[j][1] - line[j][0]));
  }
}

console.log('<svg xmlns="http://www.w3.org/2000/svg" width="'+ width +'" height="'+ width +'" stroke="#000" fill="none"><path d="' + d.join(' ') + '"/></svg>');

