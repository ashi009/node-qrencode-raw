var QR = require('../');

describe('qrencode-raw', function() {

  describe('encode()', function() {

    it('should encode string', function() {
      QR.encode('abc').should.be.ok;
      QR.encode('1').should.be.ok;
      QR.encode('ABCabc').should.be.ok;
      QR.encode('汉字').should.be.ok;
    });

    it('should encode Buffer', function() {
      QR.encode(new Buffer('abc')).should.be.ok;
      QR.encode(new Buffer('1')).should.be.ok;
      QR.encode(new Buffer('ABCabc')).should.be.ok;
      QR.encode(new Buffer('汉字')).should.be.ok;
    });

    it('should encode with error correction level hint', function() {
      QR.encode('abc', QR.ECLEVEL_L).should.be.ok;
      QR.encode(new Buffer('1'), QR.ECLEVEL_M).should.be.ok;
      QR.encode('12', QR.ECLEVEL_Q).should.be.ok;
      QR.encode('汉字', QR.ECLEVEL_H).should.be.ok;
    });

    it('should encode with error correction level hint', function() {
      QR.encode('abc', QR.ECLEVEL_L).should.be.ok;
      QR.encode(new Buffer('1'), QR.ECLEVEL_M).should.be.ok;
      QR.encode('12', QR.ECLEVEL_Q).should.be.ok;
      QR.encode('汉字', QR.ECLEVEL_H).should.be.ok;
    });

    it('should encode with version hint', function() {
      QR.encode('abc', QR.ECLEVEL_M, 10).should.be.ok;
      QR.encode(new Buffer('1'), QR.ECLEVEL_Q, 5).should.be.ok;
      QR.encode('12', QR.ECLEVEL_L, 1).should.be.ok;
      QR.encode('汉字', QR.ECLEVEL_H, 4).should.be.ok;
    });

  });

});
