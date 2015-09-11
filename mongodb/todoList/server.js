var express = require('express');
var app = express();
var path = require('path');

var lib = require('./server/lib.js');

app.use(express.static('app'));

app.post('/create', function(req, res) {
	console.log(req.body);
	lib.getDBConnection().then(function(obj) {
		return lib.createTodo(obj, req.body);
	}).then(function(obj) {
		res.json({status: 0});
	}).catch(function(error) {
		console.log('error', error);
		res.json({status: 1, error: error});
	}).then(function(obj) {
		lib.releaseDBconnection(obj);
	});
});

//404
app.use(function(req, res, next) {
	res.status(404);
	res.send('Sorry not found');
});


var server = app.listen(8080, function() {
	var host = 'localhost';
	var port = server.address().port;
	console.log('server listening at http://%s:%s', host, port);
});
