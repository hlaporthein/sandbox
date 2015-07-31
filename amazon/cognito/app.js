(function() {

	var app = angular.module('mainApp', ['ngFileUpload']);

	app.run(['$injector', function($injector) {
		console.log('run mainApp');

		var $http = $injector.get('$http');
		var $rootScope = $injector.get('$rootScope');

		$rootScope.isConnected = false;

		$rootScope.authenticate = function() {
			console.log('authenticate');
			console.log('$rootScope.login', $rootScope.login);
			$http({
				url: 'myAmazon.php',
				method: 'POST',
				data: {
					'login': $rootScope.login,
					'password': $rootScope.password
				}
			}).then(function(response) {
				console.log('response', response);
				if (response.data.status == 'ok') {
					console.log('Good login');
				} else {
					console.log('Bad login: ', response.data.error);
				}
			}).catch(function(error) {
				console.error('error', error);
			});
		};

		$rootScope.requestToken = function() {
			return $http({
				url: 'myAmazon.php',
				method: 'GET',
				params: {
					requestToken: 1
				}
			}).then(function(response) {
//				console.log('token', response);
//
//				var params = {
//				    IdentityPoolId: "YOUR_COGNITO_IDENTITY_POOL_ID"
//				};
//
//				AWS.config.region = 'eu-central-1';
//				AWS.config.credentials = new AWS.CognitoIdentityCredentials(params);
//				AWS.config.credentials.get(function(err) {
//				    if (err) {
//				        console.log("Error: "+err);
//				        return;
//				    }
//				    console.log("Cognito Identity Id: " + AWS.config.credentials.identityId);
//				});

				//AWS.config.credentials = new AWS.TemporaryCredentials();

				//AWS.config.update({credentials: response.data});

				//var creds = new AWS.Credentials(response.data);

				//AWS.config.update(response.data);
				//AWS.config.update({region: 'eu-central-1'});

			}).catch(function(error) {
				console.error('error', error);
			});
		};



		$rootScope.refresh = function() {
			var bucket = new AWS.S3({params: {Bucket: 'jlg-test-bucket'}});
			console.log('bucket', bucket);

			bucket.listObjects(function(err, data) {
				if (err) {
					console.log('error', err);
					return;
				}
				console.log('data.Contents', data.Contents);
				$rootScope.fileList = data.Contents.map(function(n) {
					console.log('n', n);
					return {
						name: n.Key
					};
				});
				console.log('$rootScope.fileList', $rootScope.fileList);
				$rootScope.myFile = $rootScope.fileList[0];
				$rootScope.myFile2 = $rootScope.fileList[0];
				$rootScope.$apply();
			});

		};

		$rootScope.fileList = [];
		$rootScope.myFile = $rootScope.fileList[0];
		$rootScope.myFile2 = $rootScope.fileList[0];


		$rootScope.retrieve = function() {
			console.log('retrieve');
			window.open('myAmazon.php?retrieve-data=' + $rootScope.myFile.name, '_blank');
		};

		$rootScope.delete = function() {
			console.log('delete');
			$http({
				url: 'myAmazon.php',
				method: 'GET',
				params: {
					'delete-data': $rootScope.myFile2.name
				}
			}).finally(function() {
            	$rootScope.refresh();
            });
		};


	}]);

	app.controller('UploadCtrl', ['$scope', 'Upload', '$rootScope', function ($scope, Upload, $rootScope) {
	    $scope.$watch('files', function () {
	    	console.log('watch', $scope.files);
	        $scope.upload($scope.files);
	    });

	    $scope.$watch('file', function () {
	    	console.log('watch', $scope.file);
	    	if ($scope.file) {
	    		$scope.upload([$scope.file]);
	    	}

	    });

	    // set default directive values
	    // Upload.setDefaults( {ngf-keep:false ngf-accept:'image/*', ...} );
	    $scope.upload = function(files) {
	        if (!(files && files.length)) {
	        	return;
	        }
            for (var i = 0; i < files.length; i++) {
                var file = files[i];
                Upload.upload({
                    url: 'myAmazon.php',
                    fields: {'upload': ''},
                    file: file
                }).progress(function (evt) {
                    var progressPercentage = parseInt(100.0 * evt.loaded / evt.total);
                    console.log('progress: ' + progressPercentage + '% ' + evt.config.file.name);
                }).success(function (data, status, headers, config) {
                	console.log('config', config);
                    console.log('file ' + config.file.name + ' uploaded. Response: ', data);
                }).error(function (data, status, headers, config) {
                    console.log('error status: ' + status);
                }).finally(function() {
                	console.log('upload finally');
                	$rootScope.refresh();
                });
            }

	    };
	}]);


})();
