var Q = require("q");

var MongoClient = require('mongodb').MongoClient;
var assert = require('assert');
var ObjectId = require('mongodb').ObjectID;
var url = 'mongodb://localhost:27017/test';

var connectP = function(url) {
	var deferred = Q.defer();
	MongoClient.connect(url, function(error, db) {
		console.log('error', error);
		if (error != null) {
			deferred.reject({db: db, error: error});
		} else {
			deferred.resolve({db: db});
		}
	});

	return deferred.promise;
};

var insertOneP = function(db, collection, document) {
	var deferred = Q.defer();
	db.collection(collection).insertOne(document, function(error, result) {
		console.log('result', result, 'END');
		console.log('error', error, 'END');
		//error = 'coucou';
		if (error != null) {
			var obj = {db: db, error: error};
			console.log('obj', obj, 'END');
			deferred.reject(obj);
		} else {
			console.log('fulfill after insertOneP');
			deferred.resolve({db: db, result: result});
		}
	});

	return deferred.promise;
};

connectP(url).then(function(obj) {
	console.log('about to insert');
	return insertOneP(obj.db, 'restaurants', {
      "address" : {
         "street" : "2 Avenue",
         "zipcode" : "10075",
         "building" : "1480",
         "coord" : [ -73.9557413, 40.7720266 ],
      },
      "borough" : "Manhattan",
      "cuisine" : "Italian",
      "grades" : [
         {
            "date" : new Date("2014-10-01T00:00:00Z"),
            "grade" : "A",
            "score" : 11
         },
         {
            "date" : new Date("2014-01-16T00:00:00Z"),
            "grade" : "B",
            "score" : 17
         }
      ],
      "name" : "Vella",
      "restaurant_id" : "41704620"
   });
}).catch(function(obj) {
	console.log('obj.error', obj.error);
	return obj;
}).then(function(obj) {
	console.log('finally');
	obj.db.close();
	console.log('end');
});
