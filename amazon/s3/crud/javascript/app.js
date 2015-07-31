(function() {

	console.log('aws', AWS);

	//set the credentials
	AWS.config.update({accessKeyId: 'xxx', secretAccessKey: 'xxx'});
	AWS.config.update({region: 'eu-central-1'});

	//list the content of the bucket
	var bucket = new AWS.S3({params: {Bucket: 'jlg-test-bucket'}});

	console.log('bucket', bucket);

	bucket.listObjects(function (err, data) {
		if (err) {
			document.getElementById('status').innerHTML =
			'Could not load objects from S3';
		} else {
			for (var i = 0; i < data.Contents.length; i++) {

				//listing data on select
				document.getElementById('file-selector').innerHTML +=
				'<option value="' + data.Contents[i].Key + '">' + data.Contents[i].Key + '</option>';

				document.getElementById('delete-file-selector').innerHTML +=
				'<option value="' + data.Contents[i].Key + '">' + data.Contents[i].Key + '</option>';
			}
		}
	});

	//upload file into the bucket
	var fileChooser = document.getElementById('file-chooser');
	var button = document.getElementById('upload-button');
	var results = document.getElementById('results');

	button.addEventListener('click', function() {

		var file = fileChooser.files[0];
		if (file) {
			results.innerHTML = 'Uploading';
			var params = {Key: file.name, ContentType: file.type, Body: file};

			bucket.upload(params, function (err, data) {
				results.innerHTML = err ? 'ERROR!' : 'UPLOADED.';
			});
		} else {
			results.innerHTML = 'Nothing to upload.';
		}
	}, false);

	//upload data to a file in the bucket
	var dataSelector = document.getElementById('file-selector');
	var dataValue = document.getElementById('data-value');
	var dataButton = document.getElementById('upload-data');
	var dataResult = document.getElementById('data-result');

	console.log('dataSelector', dataSelector);
	console.log('dataValue', dataValue);

	dataButton.addEventListener('click', function() {
		dataResult.innerHTML = 'Uploading data...';
		var params = {Key: dataSelector.value, Body: dataValue.value};

		bucket.upload(params, function (err, data) {
			dataResult.innerHTML = err ? 'ERROR!' : 'SAVED.';
		});
	}, false);

	//delete an object from the bucket
	var deleteSelector = document.getElementById('delete-file-selector');
	var deleteButton = document.getElementById('delete-button');
	var deleteResult = document.getElementById('delete-result');

	deleteButton.addEventListener('click', function() {
		deleteResult.innerHTML = 'Deleting object...';
		var params = {Key: deleteSelector.value};

		bucket.deleteObject(params, function(err, data) {
			if (err) {
				deleteResult.innerHTML = 'Error';
			} else {
				deleteResult.innerHTML = 'Delete success';
			}
		});
	}, false);


})();
