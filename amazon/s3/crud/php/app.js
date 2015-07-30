(function() {

	var app = angular.module('mainApp', ['ngFileUpload']);

	app.run(['$injector', function($injector) {
		console.log('run mainApp');

		var $http = $injector.get('$http');
		var $rootScope = $injector.get('$rootScope');

		$rootScope.refresh = function() {
			$http({
				url: 'myAmazon.php',
				method: 'GET',
				params: {
					retrieve: 1
				}
			}).then(function(response) {
				console.log('refreshed');
				$rootScope.fileList = response.data;
				console.log('response', response);
				$rootScope.myFile = $rootScope.fileList[0];
				$rootScope.myFile2 = $rootScope.fileList[0];
			}).catch(function(error) {
				console.error('error', error);
			});
		};

		$rootScope.fileList = [];
		$rootScope.myFile = $rootScope.fileList[0];
		$rootScope.myFile2 = $rootScope.fileList[0];
		$rootScope.refresh();

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
