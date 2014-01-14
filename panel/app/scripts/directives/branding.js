'use strict';

angular.module('callstatsApp')
  .directive('branding', function () {
    return {
      templateUrl: 'views/branding.html',
      restrict: 'E',
      link: function postLink(scope, element, attrs) {
        
      }
    };
  });
