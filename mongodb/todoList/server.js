var express = require('express');
var app = express();
var path = require('path');
var bodyParser = require('body-parser');

var lib = require('./server/lib.js');

//to have req.body
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
//end req.body

app.use(express.static('app'));

app.post('/create', function(req, res) {
	console.log('req.body', req.body);
	lib.getDBConnection().then(function(obj) {
		return lib.createTodo(req.body);
	}).then(function(obj) {
		return lib.findAllTodos();
	}).then(function(obj) {
		res.json({status: 0, todos: obj.result});
	}).catch(function(error) {
		console.log('error', error);
		res.json({status: 1, error: error});
	}).then(function() {
		console.log('end /create');
	});
});

app.get('/retrieveAll', function(req, res) {
	lib.getDBConnection().then(function(obj) {
		return lib.findAllTodos();
	}).then(function(obj) {
		res.json({status: 0, todos: obj.result});
	}).catch(function(error) {
		console.log('error', error);
		res.json({status: 1, error: error});
	}).then(function() {
		console.log('end /retrieveAll');
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
