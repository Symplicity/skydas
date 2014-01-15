'use strict';

angular.module('callstatsApp')
    .controller('MainCtrl', function ($scope, $http, config) {
	   $scope.url = config.PANEL.API_URL;
	  
	  $scope.loadQueues = function () {
	  	(function(){
	  		var httpRequest = $http({
	  			method: 'GET',
	  			url: ($scope.url + '/queues/fetch'),
	  		}).success(function(data,status){
	  			$scope.queues = data.msg; 
	  		});
	  	}());
	  }
	  	  
      $scope.loadCalls = function() {

			  (function () {
			             var httpRequest = $http({
			                 method: 'GET',
			                 url: ($scope.url + '/queues/all'),     
			         }).success(function(data, status) {
							                 $scope.queues = data.msg;
			         });
			     }());
    	$scope.timeout = setTimeout(function(){
        	$scope.loadCalls();
    	}, 30000);		
      } 
    $scope.loadQueues();
	$scope.loadCalls();
  });
