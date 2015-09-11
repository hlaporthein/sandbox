// Including mongoDB
var MongoClient = require('mongodb').MongoClient;
var ObjectId = require('mongodb').ObjectID;
var url = 'mongodb://localhost:27017/todo';

var myDb;
MongoClient.connect(url, function(err, db) {
	if (err) {
		console.log('Mongo error:', err);
		return;
	}
	myDb = db;
	console.log("Test connection successfull.");
});

exports.getDBConnection = function() {
	return new Promise(function(fulfill, reject) {
		console.log('myDb', myDb);
		if (!myDb) {
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
		}

	});
};

exports.createTodo = function(db, document) {
	return new Promise(function(fulfill, reject) {
		db.collection('todo').insertOne(document, function(error, result) {
			if (error != null) {
				reject({db: db, error: error});
			} else {
				console.log('fulfill after insertOneP');
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

