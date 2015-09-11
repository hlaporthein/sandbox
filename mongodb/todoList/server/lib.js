// Including mongoDB
var MongoClient = require('mongodb').MongoClient;
var ObjectId = require('mongodb').ObjectID;
var url = 'mongodb://localhost:27017/todo';

var myDb;

exports.getDBConnection = function() {
	return new Promise(function(fulfill, reject) {
		console.log('myDb', myDb);
		if (!myDb) {
			console.log('about to connect');
			MongoClient.connect(url, function(err, db) {
				if (err) {
					console.log('Mongo error:', err);
					reject('cannot connect');
					return;
				}
				myDb = db;
				console.log("Test connection successfull.");
				fulfill({db: myDb});
			});
		} else {
			fulfill({db: myDb});
		}

	});
};

exports.createTodo = function(db, document) {
	return new Promise(function(fulfill, reject) {
		console.log('document', document);
		db.collection('todo').insertOne(document, function(error, result) {
			if (error != null) {
				reject(error);
			} else {
				console.log('fulfill after createTodo');
				fulfill({db: db, result: result});
			}
		});
	});
};

exports.findAllTodos = function(db) {
	return new Promise(function(fulfill, reject) {
		db.collection('todo').find().toArray(function(error, result) {
			if (error != null) {
				reject(error);
			} else {
				console.log('fulfill after findAllTodos');
				fulfill({db: db, result: result});
			}
		});
	});
};

exports.releaseDBconnection = function() {
	return new Promise(function(fulfill, reject) {
		fulfill({});
	});
};

