var MongoClient = require('mongodb').MongoClient;
var assert = require('assert');
var ObjectId = require('mongodb').ObjectID;
var url = 'mongodb://localhost:27017/test';

var connectP = function(url) {
	return new Promise(function(fulfill, reject) {
		MongoClient.connect(url, function(error, db) {
			console.log('error', error);
			if (error != null) {
				reject({db: db, error: error});
			} else {
				fulfill({db: db});
			}
		});
	});
};

var insertOneP = function(db, collection, document) {
	return new Promise(function(fulfill, reject) {
		db.collection(collection).insertOne(document, function(error, result) {
			console.log('result', result, 'END');
			console.log('error', error, 'END');
			//error = 'coucou';
			if (error != null) {
				reject({db: db, error: error});
			} else {
				console.log('fulfill after insertOneP');
				fulfill({db: db, result: result});
			}
		});
	});
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
	console.log('obj', obj);
	obj.db.close();
	console.log('end');
});
