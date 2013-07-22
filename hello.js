var addon = require('./build/Release/hello');
// var http = require('http');
// var fs = require('fs');

// http.createServer(function(req, res) {
// 	console.log(req.url);
// 	var data = addon.hello(new Buffer(req.url.substr(1))).data.toString('hex');
// 	var html = fs.readFileSync('./test.html').toString();
// 	res.end(html.replace(/DATA/, data));
// }).listen(8000);

console.log(addon);