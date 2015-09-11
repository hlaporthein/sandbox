(function() {
	'use strict';

	var app = angular.module('myApp', ['ngRoute']);
	app.config(['$routeProvider', function($routeProvider) {

		$routeProvider
			.when('/', {
				templateUrl: 'list.html'
			})
			.when('/todo/:id', {
				templateUrl: 'object.html'
			})
			.when('/create', {
				templateUrl: 'create.html'
			})
			.otherwise({
				redirectTo: '/'
			});
	}]);

	app.run(['$injector', function($injector) {
		var $rootScope = $injector.get('$rootScope');
		var $location = $injector.get('$location');
		var $http = $injector.get('$http');
		var $q = $injector.get('$q');

		$rootScope.todo = {};

		$rootScope.todos = [];

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

			$http({
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

		$rootScope.init();
	}]);
})();
