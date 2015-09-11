(function() {
	'use strict';

	var app = angular.module('myApp', ['ngRoute']);
	app.config(['$routeProvider', function($routeProvider) {

		$routeProvider
			.when('/', {
				templateUrl: 'list.html',
				controller: 'ListCtrl',
				controllerAs: 'ctrl'
			})
			.when('/todo/:id', {
				templateUrl: 'object.html',
				controller: 'ObjectCtrl',
				controllerAs: 'ctrl'
			})
			.when('/create', {
				templateUrl: 'create.html'
			})
			.otherwise({
				redirectTo: '/'
			});
	}]);

	app.run(['$injector', function($injector) {
		console.log('run');
		var $rootScope = $injector.get('$rootScope');
		var $location = $injector.get('$location');
		var $http = $injector.get('$http');
		var $q = $injector.get('$q');

		$rootScope.new = function() {
			console.log('new');

			$location.path('/create');
		};

		$rootScope.delete = function() {
			console.log('delete');
		};

		$rootScope.add = function() {
			console.log('add');
			console.log('$rootScope.todo', $rootScope.todo);

			$http({
				url: '/create',
				method: 'post',
				data: $rootScope.todo
			}).then(function(response) {
				console.log('response', response);
				if (response.data.status != 0) {
					return $q.reject('Bad status');
				}
				$rootScope.todos = response.data.todos;
			}).then(function() {
				$location.path('/');
			}).catch(function(error) {
				console.error('error', error);
			});
		};

		$rootScope.init = function() {
			console.log('init');

			return $http({
				url: '/retrieveAll',
				method: 'get'
			}).then(function(response) {
				console.log('response', response);
				if (response.data.status != 0) {
					return $q.reject('Bad status');
				}
				$rootScope.todos = response.data.todos;
			}).catch(function(error) {
				console.error('error', error);
			});

		};


	}]);

	app.controller('ListCtrl', ['$scope', '$injector', function($scope, $injector) {
		console.log('instantiating ObjectCtrl');

		var $rootScope = $injector.get('$rootScope');
		$rootScope.init();

	}]);

	app.controller('ObjectCtrl', ['$scope', '$injector', function($scope, $injector) {
		console.log('instantiating ObjectCtrl');

		var $rootScope = $injector.get('$rootScope');
		var $routeParams = $injector.get('$routeParams');

		$rootScope.init().then(function() {
			$scope.todo = $rootScope.todos.find(function(n, i, array) {
				return n._id == $routeParams.id;
			});
		});

		console.log('$scope.todo', $scope.todo);

	}]);

	// polyfills
	if (!Array.prototype.find) {
		Array.prototype.find = function(predicate) {
		if (this == null) {
			throw new TypeError('Array.prototype.find a été appelé sur null ou undefined');
		}
		if (typeof predicate !== 'function') {
			throw new TypeError('predicate doit être une fonction');
		}
		var list = Object(this);
		var length = list.length >>> 0;
		var thisArg = arguments[1];
		var value;

		for (var i = 0; i < length; i++) {
			value = list[i];
			if (predicate.call(thisArg, value, i, list)) {
				return value;
			}
		}
		return undefined;
	};
}

})();
