var qr = require('../');
var crypto = require('crypto');

function sha1(buf) {
  var hasher = crypto.createHash('sha1');
  hasher.update(buf);
  return hasher.digest('hex');
}

describe('qrencode-raw', function() {

  describe('encode()', function() {

    it('should encode string', function() {
      qr.encode('abc').should.be.ok;
      qr.encode('1').should.be.ok;
      qr.encode('ABCabc').should.be.ok;
      qr.encode('汉字').should.be.ok;
    });

    it('should encode Buffer', function() {
      qr.encode(new Buffer('abc')).should.be.ok;
      qr.encode(new Buffer('1')).should.be.ok;
      qr.encode(new Buffer('ABCabc')).should.be.ok;
      qr.encode(new Buffer('汉字')).should.be.ok;
    });

    it('should encode with error correction level hint', function() {
      qr.encode('abc', qr.ECLEVEL_L).should.be.ok;
      qr.encode(new Buffer('1'), qr.ECLEVEL_M).should.be.ok;
      qr.encode('12', qr.ECLEVEL_Q).should.be.ok;
      qr.encode('汉字', qr.ECLEVEL_H).should.be.ok;
    });

    it('should encode with error correction level hint', function() {
      qr.encode('abc', qr.ECLEVEL_L).should.be.ok;
      qr.encode(new Buffer('1'), qr.ECLEVEL_M).should.be.ok;
      qr.encode('12', qr.ECLEVEL_Q).should.be.ok;
      qr.encode('汉字', qr.ECLEVEL_H).should.be.ok;
    });

    it('should encode with version hint', function() {
      qr.encode('abc', qr.ECLEVEL_M, 10).should.be.ok;
      qr.encode(new Buffer('1'), qr.ECLEVEL_Q, 5).should.be.ok;
      qr.encode('12', qr.ECLEVEL_L, 1).should.be.ok;
      qr.encode('汉字', qr.ECLEVEL_H, 4).should.be.ok;
    });

    var tests = {
      'https://github.com/ashi009/node-qrencode-raw':
          'bf21d89437d602599333c80bfe61f699171048b2'
    };

    for (var text in tests) {
      it('should encode "' + text + '" correctly', function() {
        sha1(qr.encode(text, qr.ECLEVEL_M).data)
            .should.eql(tests[text]);
      });
      it('should encode Buffer<' + text + '> correctly', function() {
        sha1(qr.encode(new Buffer(text), qr.ECLEVEL_M).data)
            .should.eql(tests[text]);
      });
    }

  });

});
