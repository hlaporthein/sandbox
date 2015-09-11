// Including mongoDB
var MongoClient = require('mongodb').MongoClient;
var ObjectId = require('mongodb').ObjectID;
var url = 'mongodb://localhost:27017/todo';

var db;

exports.getDBConnection = function() {
	return new Promise(function(fulfill, reject) {
		console.log('db', db);
		if (!db) {
			console.log('about to connect');
			MongoClient.connect(url, function(err, myDb) {
				if (err) {
					console.log('Mongo error:', err);
					reject('cannot connect');
					return;
				}
				db = myDb;
				console.log("Test connection successfull.");
				fulfill({});
			});
		} else {
			fulfill({});
		}

	});
};

exports.createTodo = function(document) {
	return new Promise(function(fulfill, reject) {
		console.log('document', document);
		db.collection('todo').insertOne(document, function(error, result) {
			if (error != null) {
				reject(error);
			} else {
				console.log('fulfill after createTodo');
				fulfill({result: result});
			}
		});
	});
};

exports.findAllTodos = function() {
	return new Promise(function(fulfill, reject) {
		db.collection('todo').find().toArray(function(error, result) {
			if (error != null) {
				reject(error);
			} else {
				console.log('fulfill after findAllTodos');
				fulfill({result: result});
			}
		});
	});
};

