'use strict';

describe('Directive: branding', function () {

  // load the directive's module
  beforeEach(module('callstatsApp'));

  var element,
    scope;

  beforeEach(inject(function ($rootScope) {
    scope = $rootScope.$new();
  }));

  it('should make hidden element visible', inject(function ($compile) {
    element = angular.element('<branding></branding>');
    element = $compile(element)(scope);
    expect(element.text()).toBe('this is the branding directive');
  }));
});
