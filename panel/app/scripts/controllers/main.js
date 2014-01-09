'use strict';

angular.module('callstatsApp')
  .controller('MainCtrl', function ($scope, $http) {	  
	  $scope.queues = {
		  'queue1' : {
			  'agents':{},
			  'callers':{}
		  },
		  'queue2' : {
		      'agents':{},
			  'callers':{}
		  },
		  'queue3' : {
			  'agents':{},
			  'callers':{}
		  },
		  'queue4' : {
			  'agents':{},
			  'callers':{}
		  }
	  }
	  	  
      $scope.loadCalls = function() {

			  (function () {
			             var httpRequest = $http({
			                 method: 'GET',
			                 url: 'http://freeswitch.example.com/callcenter/queues/all',     

			         }).success(function(data, status) {
							                 $scope.queues = data.msg;
			         });
			     }());
    	$scope.timeout = setTimeout(function(){
        	$scope.loadCalls();
    	}, 30000);		
      } 

	$scope.loadCalls();
  });
