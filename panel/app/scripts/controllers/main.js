'use strict';

angular.module('callstatsApp',['config'])
  .controller('MainCtrl', function ($scope, $http) {	  
	  $scope.loadQueues = function () {

	  	(function(){
	  		var httpRequest = $http({
	  			method: 'GET',
	  			url: (config_data.PANEL.API_URL + '/queues/fetch'),
	  		}).success(function(data,status){
	  			$scopes.queues = data.msg; 
	  		});
	  	}());
	  }
	  	  
      $scope.loadCalls = function() {

			  (function () {
			             var httpRequest = $http({
			                 method: 'GET',
			                 url: (PANEL.API_URL + '/callcenter/queues/all'),     
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
